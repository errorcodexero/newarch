using System;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using System.Collections.Generic;
using Newtonsoft.Json;
using System.ComponentModel;
using System.Diagnostics;

namespace PathViewer
{
    public partial class PathViewerForm : Form
    {
        #region private member variables
        /// <summary>
        /// The set of games that are available
        /// </summary>
        private GameManager m_games;

        /// <summary>
        /// THe set of path generators that are available
        /// </summary>
        private GeneratorManager m_generators;

        /// <summary>
        /// The current path file we are editing
        /// </summary>
        private PathFile m_file;

        /// <summary>
        /// The control for editing path group names and path names in place
        /// </summary>
        private TextBox m_text_editor;

        /// <summary>
        /// The node we are currently editing in the path group and path tree control
        /// </summary>
        private TreeNode m_editing_pathtree;

        /// <summary>
        /// The list view item we are editing in the waypoint window or robot params window
        /// </summary>
        private ListViewItem m_waypoint_editing;

        /// <summary>
        /// The robot view item we are editing
        /// </summary>
        private ListViewItem m_robot_param_editing;

        /// <summary>
        /// The currently selected generator
        /// </summary>
        private PathGenerator m_generator;

        /// <summary>
        /// The message logger for the application
        /// </summary>
        private Logger m_logger;

        /// <summary>
        /// If true, igonre the lost focus event on the editing text control
        /// </summary>
        private bool m_ignore_lost_focus;

        /// <summary>
        /// The old selected node during a context menu event on the path tree view
        /// </summary>
        private TreeNode m_old_node;

        /// <summary>
        /// The undo stack
        /// </summary>
        private List<UndoState> m_undo_stack;

        /// <summary>
        /// The length of the undo stack
        /// </summary>
        private int m_undo_length;

        /// <summary>
        /// The number of the undo state generated
        /// </summary>
        private int m_undo_serial;

        #endregion

        #region public constructor
        public PathViewerForm()
        {
            m_logger = new Logger();

            m_games = new GameManager();
            m_generators = new GeneratorManager();

            if (m_games.Count == 0)
            {
                MessageBox.Show("No games are installed - cannot start path generator program", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                m_logger.LogMessage(Logger.MessageType.Error, "no games are installed");
                throw new Exception("No games installed");
            }

            if (m_generators.Count == 0)
            {
                MessageBox.Show("No path generators are installed - cannot start path generator program", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                m_logger.LogMessage(Logger.MessageType.Error, "no generators are installed");
                throw new Exception("No generators installed");
            }

            m_file = new PathFile();
            Text = "Path Editor - Unsaved";

            m_editing_pathtree = null;
            m_text_editor = null;

            InitializeComponent();

            m_field.File = m_file;
            m_field.WaypointSelected += WayPointSelectedOrChanged;
            m_field.WaypointChanged += WayPointSelectedOrChanged;
            m_field.FieldMouseMoved += FieldMouseMoved;
            m_field.PreviewKeyDown += FieldKeyPreview;
            m_field.KeyDown += FieldKeyDown;

            m_pathfile_tree.DoubleClick += DoPathTreeDoubleClick;
            m_pathfile_tree.CheckBoxes = true;
            m_pathfile_tree.AfterCheck += PathTreeCheckBoxChanged;
            m_pathfile_tree.MouseUp += PathTreeMouseUp;

            m_waypoints.DoubleClick += WaypointDoubleClick;
            m_robot.DoubleClick += RobotParamDoubleClick;

            m_ignore_lost_focus = false;
            m_undo_stack = new List<UndoState>();
            m_undo_serial = 0;
            m_undo_length = 10;

            PopulateGameMenu();
            PopulateGeneratorMenu();
            UpdateRobotWindow();
        }

        #endregion

        #region event handlers for the field child control
        private void FieldKeyPreview(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Up || e.KeyCode == Keys.Down || e.KeyCode == Keys.Left || e.KeyCode == Keys.Right)
                e.IsInputKey = true;
        }

        private void FieldKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode != Keys.Delete && e.KeyCode != Keys.Insert &&
                e.KeyCode != Keys.Up && e.KeyCode != Keys.Down &&
                e.KeyCode != Keys.Left && e.KeyCode != Keys.Right &&
                e.KeyCode != Keys.Add && e.KeyCode != Keys.Subtract &&
                e.KeyCode != Keys.Z)
                return;

            e.Handled = true;

            if (e.KeyCode == Keys.Z)
            {
                if (e.Control)
                    PopUndoStack();

                return;
            }

            if (m_field.SelectedWaypoint == null)
                return;

            PathGroup gr;
            RobotPath path;
            if (!m_file.FindPathByWaypoint(m_field.SelectedWaypoint, out gr, out path))
                return;

            bool gen = false;

            if (e.KeyCode == Keys.Delete)
            {
                if (path.RemovePoint(m_field.SelectedWaypoint))
                    gen = true;
            }
            else if (e.KeyCode == Keys.Insert)
            {
                if (path.InsertPoint(m_field.SelectedWaypoint, m_file.Robot.MaxVelocity))
                    gen = true;
            }
            else if (e.KeyCode == Keys.Up)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Y += 0.1;
                else
                    m_field.SelectedWaypoint.Y += 1.0;
                gen = true;
            }
            else if (e.KeyCode == Keys.Down)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Y -= 0.1;
                else
                    m_field.SelectedWaypoint.Y -= 1.0;
                gen = true;
            }
            else if (e.KeyCode == Keys.Left)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.X -= 0.1;
                else
                    m_field.SelectedWaypoint.X -= 1.0;
                gen = true;
            }
            else if (e.KeyCode == Keys.Right)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.X += 0.1;
                else
                    m_field.SelectedWaypoint.X += 1.0;
                gen = true;
            }
            else if (e.KeyCode == Keys.Add)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Heading += 0.5;
                else
                    m_field.SelectedWaypoint.Heading += 5.0;
                gen = true;
            }
            else if (e.KeyCode == Keys.Subtract)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Heading -= 0.5;
                else
                    m_field.SelectedWaypoint.Heading -= 5.0;
                gen = true;
            }

            if (gen)
            {
                m_field.Invalidate();
                GenerateSplines(path);
            }
        }

        private void FieldMouseMoved(object sender, FieldMouseMovedArgs e)
        {
            string pos = "Position: " + e.Point.X.ToString() + ", " + e.Point.Y.ToString();
            m_pos_status.Text = pos;
        }

        private void WayPointSelectedOrChanged(object sender, WaypointEventArgs e)
        {
            if (e.Path != null)
            {
                if (e.Reason == WaypointEventArgs.ReasonType.StartChange)
                    PushUndoStack();

                GenerateSplines(e.Path);
            }
            UpdateWaypointPropertyWindow(e.Point);
        }
        #endregion

        #region event handlers for the path tree control
        private void PathTreeMouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Right)
                return;

            if (m_old_node != null)
                return;

            Point pt = new Point(e.X, e.Y);
            TreeNode tn = m_pathfile_tree.GetNodeAt(pt);
            if (tn == null)
                return;

            m_old_node = m_pathfile_tree.SelectedNode;
            m_pathfile_tree.SelectedNode = tn;

            ContextMenu menu = new ContextMenu();
            MenuItem mi;

            if (tn.Parent == null)
            {
                mi = new MenuItem("Add Path Group", NewPathGroupToolStripMenuItem_Click);
                menu.MenuItems.Add(mi);

                mi = new MenuItem("Delete Path Group", RemovePathGroupToolStripMenuItem_Click);
                menu.MenuItems.Add(mi);
            }

            mi = new MenuItem("Add Path", NewPathToolStripMenuItem_Click);
            menu.MenuItems.Add(mi);

            if (tn.Parent != null)
            {
                mi = new MenuItem("Delete Path", RemovePathToolStripMenuItem_Click);
                menu.MenuItems.Add(mi);
            }

            menu.Show(m_pathfile_tree, pt);

            m_pathfile_tree.SelectedNode = m_old_node;
            m_old_node = null;
        }
        private void PathTreeCheckBoxChanged(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null)
            {
                //
                // A group was checked
                //
                foreach (TreeNode tn in e.Node.Nodes)
                    tn.Checked = e.Node.Checked;
            }
            else
            {
                //
                // A path was checked
                //
                RobotPath path = m_file.FindPathByName(e.Node.Parent.Text, e.Node.Text);
                if (path != null)
                {
                    if (e.Node.Checked)
                        m_field.RobotPaths.Add(path);
                    else
                        m_field.RobotPaths.Remove(path);

                    m_field.Invalidate();
                }
            }
        }

        private void DoPathTreeDoubleClick(object sender, EventArgs e)
        {
            TreeNode tn = m_pathfile_tree.SelectedNode;
            if (tn != null)
            {
                EditGroupOrPathName(tn);
            }
        }
        #endregion

        #region event handlers for the robot params window
        private void RobotParamDoubleClick(object sender, EventArgs e)
        {
            if (m_text_editor != null || m_robot.SelectedItems.Count != 1)
                return;

            ListViewItem item = m_robot.SelectedItems[0];

            m_robot_param_editing = item;
            Rectangle b = new Rectangle(item.SubItems[1].Bounds.Left, item.SubItems[1].Bounds.Top, item.SubItems[1].Bounds.Width, item.SubItems[1].Bounds.Height);

            m_text_editor = new TextBox();
            m_text_editor.Text = item.SubItems[1].Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_robot;
            m_text_editor.Enabled = true;
            m_text_editor.Visible = true;
            m_text_editor.LostFocus += FinishedEditingProperty;
            m_text_editor.PreviewKeyDown += PreviewEditorKeyProperty;
            m_text_editor.Focus();
        }
        #endregion

        #region event handlers for the waypoints window
        private void WaypointDoubleClick(object sender, EventArgs e)
        {
            if (m_text_editor != null || m_waypoints.SelectedItems.Count != 1 || m_field.SelectedWaypoint == null)
                return;

            ListViewItem item = m_waypoints.SelectedItems[0];
            if (item.Text == "Group" || item.Text == "Path")
                return;

            m_waypoint_editing = item;
            Rectangle b = new Rectangle(item.SubItems[1].Bounds.Left, item.SubItems[1].Bounds.Top, item.SubItems[1].Bounds.Width, item.SubItems[1].Bounds.Height);

            m_text_editor = new TextBox();
            m_text_editor.Text = item.SubItems[1].Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_waypoints;
            m_text_editor.Enabled = true;
            m_text_editor.Visible = true;
            m_text_editor.LostFocus += FinishedEditingProperty;
            m_text_editor.PreviewKeyDown += PreviewEditorKeyProperty;
            m_text_editor.Focus();
        }
        #endregion

        #region event handlers for the top level form

        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);

            if (m_file.IsDirty)
            {
                if (MessageBox.Show("The paths file has been changed.  Closing now will discard these changes.  Do you really want to quit?",
                        "Really Quit?", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
                {
                    e.Cancel = true;
                }
            }
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            //
            // Resize the three controls on the right side
            //
            m_right_vertical.SplitterDistance = m_right_vertical.Height / 3;
            m_right_bottom.SplitterDistance = m_right_bottom.Height / 2;
        }

        private void PreviewEditorKeyProperty(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyData == Keys.Cancel || e.KeyData == Keys.Escape)
            {
                StopEditing();
            }
            else if (e.KeyData == Keys.Return)
            {
                FinishedEditingProperty(sender, EventArgs.Empty);
            }
        }

        private void FinishedEditingProperty(object sender, EventArgs args)
        {
            bool stop = true;

            if (m_ignore_lost_focus)
                return;

            if (m_robot_param_editing != null)
                stop = FinishedEditingRobotParam();
            else if (m_waypoint_editing != null)
                stop = FinishedEditingWaypoint();

            if (stop)
                StopEditing();
            else
            {
                m_ignore_lost_focus = false;
                m_text_editor.Focus();
            }
        }


        private void PreviewEditorKeyPathTree(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyData == Keys.Cancel || e.KeyData == Keys.Escape)
            {
                StopEditing();
            }
            else if (e.KeyData == Keys.Return)
            {
                FinishedEditingNamePathTree(sender, EventArgs.Empty);
            }
        }

        private void FinishedEditingNamePathTree(object sender, EventArgs e)
        {
            string name = m_text_editor.Text;

            if (m_editing_pathtree.Parent == null)
            {
                PushUndoStack();
                if (m_file.RenameGroup(m_editing_pathtree.Text, m_text_editor.Text))
                {
                    m_editing_pathtree.Text = m_text_editor.Text;
                    StopEditing();
                }
            }
            else
            {
                PushUndoStack();
                if (m_file.RenamePath(m_editing_pathtree.Parent.Text, m_editing_pathtree.Text, m_text_editor.Text))
                {
                    m_editing_pathtree.Text = m_text_editor.Text;
                    StopEditing();
                }
            }
        }
        #endregion

        #region event handlers for the menus

        private void UndoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PopUndoStack();
        }

        private void GeneratorSelected(object sender, EventArgs args)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;

            //
            // Uncheck all generators
            //
            ToolStrip strip = item.GetCurrentParent();
            if (strip != null)
            {
                foreach (var vitem in strip.Items)
                {
                    ToolStripMenuItem mitem = vitem as ToolStripMenuItem;
                    if (mitem != null)
                        mitem.Checked = false;
                }
            }

            //
            // Check the one generator selected
            //
            item.Checked = true;

            //
            // Initialize the generator
            //
            InitializeGenerator(item.Tag as PathGenerator);
        }

        private void GameSelected(object sender, EventArgs args)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;

            //
            // Uncheck all games
            //
            ToolStrip strip = item.GetCurrentParent();
            if (strip != null)
            {
                foreach (var vitem in strip.Items)
                {
                    ToolStripMenuItem mitem = vitem as ToolStripMenuItem;
                    if (mitem != null)
                        mitem.Checked = false;
                }
            }

            item.Checked = true;
            InitializeGame(item.Tag as Game);
        }

        private void LoadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_file != null && m_file.IsDirty)
            {
                DialogResult result = MessageBox.Show(
                    "The current path file has unsaved changes.  Do you want to save this file first?",
                    "Unsaved Changes", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

                if (result == DialogResult.Yes)
                {
                    if (!SaveChanges())
                    {
                        MessageBox.Show("Could not save changes, open operation aborted", "Open Aborted",
                            MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }
                }
                else if (result == DialogResult.Cancel)
                    return;
            }

            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = "Open a Path File";
            dialog.Filter = "Path Files (*.json)|*.json|All Files (*.*)|*.*";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string json = File.ReadAllText(dialog.FileName);
                m_file = JsonConvert.DeserializeObject<PathFile>(json);
                m_file.PathName = dialog.FileName;
                m_field.File = m_file;
                Text = "Path Editor - " + m_file.PathName;
                m_undo_stack = new List<UndoState>();
                GenerateAllSplines();
                UpdatePathTree();
                UpdateRobotWindow();
            }
        }

        private void SaveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Title = "Save a Path File";
            dialog.Filter = "Path Files (*.json)|*.json|All Files (*.*)|*.*";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_file.PathName = dialog.FileName;
                Text = "Path Editor - " + m_file.PathName;
                SaveChanges();
            }
        }

        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_file.PathName))
                SaveAsToolStripMenuItem_Click(sender, e);
            else
                SaveChanges();
        }

        private void NewPathGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string newname = GetNewPathGroupName();
            TreeNode node = new TreeNode(newname);
            m_pathfile_tree.Nodes.Add(node);

            PushUndoStack();
            m_file.AddPathGroup(newname);
        }

        private void NewPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode node = m_pathfile_tree.SelectedNode;
            if (node == null)
                return;

            if (node.Parent != null)
                node = node.Parent;

            string newname = GetNewPathName(node);
            TreeNode nnode = new TreeNode(newname);
            node.Nodes.Add(nnode);
            m_file.AddPath(node.Text, newname);
        }

        private void RemovePathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode node = m_pathfile_tree.SelectedNode;
            if (node == null)
                return;

            PushUndoStack();

            string grname = node.Parent.Text;
            node.Parent.Nodes.Remove(node);
            RobotPath path = m_file.FindPathByName(grname, node.Text);
            if (m_field.RobotPaths.Contains(path))
            {
                m_field.RobotPaths.Remove(path);
                m_field.Invalidate();
            }

            m_file.RemovePath(grname, node.Text);
        }

        private void RemovePathGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode node = m_pathfile_tree.SelectedNode;
            if (node == null)
                return;

            m_pathfile_tree.Nodes.Remove(node);
            PushUndoStack();
            m_file.RemoveGroup(node.Text);
        }
        #endregion

        #region private methods

        #region undo stack
        private void PushUndoStack()
        {
            PathFile pf = new PathFile(m_file);
            UndoState st = new UndoState(m_undo_serial++, pf);

            m_undo_stack.Add(st);

            foreach(RobotPath path in m_field.RobotPaths)
            {
                PathGroup gr = m_file.FindGroupByPath(path);
                st.AddPath(gr.Name, path.Name);
            }

            if (m_field.SelectedWaypoint != null)
            {
                PathGroup gr;
                RobotPath path;
                int index;
                m_file.FindPathByWaypoint(m_field.SelectedWaypoint, out gr, out path, out index);
                st.AddSelectedWaypoint(gr.Name, path.Name, index);
            }

            while (m_undo_stack.Count > m_undo_length)
                m_undo_stack.RemoveAt(m_undo_stack.Count - 1);
        }

        private void PopUndoStack()
        {
            if (m_undo_stack.Count > 0)
            {
                UndoState st = m_undo_stack[m_undo_stack.Count - 1];
                m_file = st.File;
                m_undo_stack.RemoveAt(m_undo_stack.Count - 1);

                UpdateRobotWindow();
                UpdatePathTree();
                m_field.File = m_file;
                m_field.Invalidate();
                GenerateAllSplines();

                foreach(Tuple<string, string> pathname in st.SelectedPaths)
                {
                    foreach(TreeNode group in m_pathfile_tree.Nodes)
                    {
                        if (group.Text == pathname.Item1)
                        {
                            foreach(TreeNode path in group.Nodes)
                            {
                                if (path.Text == pathname.Item2)
                                {
                                    path.Checked = true;
                                }
                            }
                        }
                    }
                }

                if (st.HasSelectedWaypoint)
                {
                    RobotPath path = m_file.FindPathByName(st.SelectedGroup, st.SelectedPath);
                    if (path != null && st.SelectedIndex < path.Points.Length)
                        m_field.SelectedWaypoint = path.Points[st.SelectedIndex];
                }
            }
        }
        #endregion

        private bool FinishedEditingRobotParam()
        {
            double d;

            if (m_robot_param_editing.Text == "Type")
            {
                if (!RobotParams.IsValidDriveType(m_text_editor.Text))
                {
                    string list = string.Empty;
                    foreach (string dtype in RobotParams.DriveTypes)
                    {
                        if (!String.IsNullOrEmpty(list))
                            list += ", ";
                        list += "'" + dtype + "'";
                    }
                    string msg = "invalid drive type '" + m_text_editor.Text + "', must be one of ";
                    msg += list;
                    m_ignore_lost_focus = true;
                    MessageBox.Show(msg, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                PushUndoStack();
                m_file.Robot.DriveType = m_text_editor.Text;
            }
            else
            {
                if (!double.TryParse(m_text_editor.Text, out d))
                    return false;

                PushUndoStack();
                if (m_robot_param_editing.Text == "Width")
                {
                    m_file.Robot.Width = d;
                }
                else if (m_robot_param_editing.Text == "Length")
                {
                    m_file.Robot.Length = d;
                }
                else if (m_robot_param_editing.Text == "Max Velocity")
                {
                    m_file.Robot.MaxVelocity = d;
                }
                else if (m_robot_param_editing.Text == "Max Acceleration")
                {
                    m_file.Robot.MaxAcceleration = d;
                }
                else if (m_robot_param_editing.Text == "Max Jerk")
                {
                    m_file.Robot.MaxJerk = d;
                }
            }
            UpdateRobotWindow();
            m_field.Invalidate();
            return true;
        }

        private bool FinishedEditingWaypoint()
        {
            double d;

            if (!double.TryParse(m_text_editor.Text, out d))
                return false;

            if (m_waypoint_editing.Text == "X")
            {
                m_field.SelectedWaypoint.X = d;
                m_field.Invalidate();
            }
            else if (m_waypoint_editing.Text == "Y")
            {
                m_field.SelectedWaypoint.Y = d;
                m_field.Invalidate();
            }
            else if (m_waypoint_editing.Text == "Heading")
            {
                m_field.SelectedWaypoint.Heading = d;
                m_field.Invalidate();
            }
            else if (m_waypoint_editing.Text == "Velocity")
            {
                m_field.SelectedWaypoint.Velocity = d;
                m_field.Invalidate();
            }

            PathGroup group;
            RobotPath path;
            m_file.FindPathByWaypoint(m_field.SelectedWaypoint, out group, out path);
            UpdateWaypointPropertyWindow(m_field.SelectedWaypoint);

            return true;
        }
        private ToolStripMenuItem FindItem(string title)
        {
            ToolStripMenuItem mitem = null;

            foreach (var item in m_menu.Items)
            {
                ToolStripMenuItem testitem = item as ToolStripMenuItem;
                if (testitem != null && testitem.Text == title)
                {
                    mitem = testitem;
                    break;
                }
            }

            return mitem;
        }

        private void PopulateGameMenu()
        {
            ToolStripMenuItem select = null ;

            ToolStripMenuItem item = FindItem("Games");
            if (item != null)
            {
                foreach (Game g in m_games.Games)
                {
                    ToolStripMenuItem mitem = new ToolStripMenuItem(g.Name, null, GameSelected);
                    mitem.Tag = g;
                    item.DropDownItems.Add(mitem);

                    if (select == null)
                        select = mitem;
                }
            }

            GameSelected(select, EventArgs.Empty);
        }

        private void PopulateGeneratorMenu()
        {
            ToolStripMenuItem select = null;

            ToolStripMenuItem item = FindItem("Generators");
            if (item != null)
            {
                foreach (PathGenerator g in m_generators.Generators)
                {
                    string name = g.Name + " (" + g.Version.ToString() + ")";
                    ToolStripMenuItem mitem = new ToolStripMenuItem(name, null, GeneratorSelected);
                    mitem.Tag = g;
                    item.DropDownItems.Add(mitem);

                    if (select == null)
                        select = mitem;
                }
            }

            GeneratorSelected(select, EventArgs.Empty);
        }

        private void InitializeGame(Game g)
        {
            m_field.FieldGame = g;
        }

        private void InitializeGenerator(PathGenerator g)
        {
            m_generator = g;
            GenerateAllSplines();
        }

        private void UpdateWaypointPropertyWindow(WayPoint pt)
        {
            ListViewItem item;

            m_waypoints.Items.Clear();

            item = new ListViewItem("X");
            item.SubItems.Add(pt.X.ToString());
            m_waypoints.Items.Add(item);

            item = new ListViewItem("Y");
            item.SubItems.Add(pt.Y.ToString());
            m_waypoints.Items.Add(item);

            item = new ListViewItem("Heading");
            item.SubItems.Add(pt.Heading.ToString());
            m_waypoints.Items.Add(item);

            item = new ListViewItem("Velocity");
            item.SubItems.Add(pt.Velocity.ToString());
            m_waypoints.Items.Add(item);
        }
        
        private void UpdateRobotWindow()
        {
            if (m_file!= null && m_file.Robot != null)
            {
                ListViewItem item;

                m_robot.Items.Clear();

                item = new ListViewItem("Type");
                item.SubItems.Add(m_file.Robot.DriveType);
                m_robot.Items.Add(item);

                item = new ListViewItem("Width");
                item.SubItems.Add(m_file.Robot.Width.ToString());
                m_robot.Items.Add(item);

                item = new ListViewItem("Length");
                item.SubItems.Add(m_file.Robot.Length.ToString());
                m_robot.Items.Add(item);

                item = new ListViewItem("Max Velocity");
                item.SubItems.Add(m_file.Robot.MaxVelocity.ToString());
                m_robot.Items.Add(item);

                item = new ListViewItem("Max Acceleration");
                item.SubItems.Add(m_file.Robot.MaxAcceleration.ToString());
                m_robot.Items.Add(item);

                item = new ListViewItem("Max Jerk");
                item.SubItems.Add(m_file.Robot.MaxJerk.ToString());
                m_robot.Items.Add(item);

            }
        }

        private void UpdatePathTree()
        {
            m_pathfile_tree.Nodes.Clear();
            foreach(PathGroup gr in m_file.Groups)
            {
                TreeNode group = new TreeNode(gr.Name);
                foreach(RobotPath pa in gr.Paths)
                {
                    TreeNode path = new TreeNode(pa.Name);
                    group.Nodes.Add(path);
                }
                m_pathfile_tree.Nodes.Add(group);
            }
            m_pathfile_tree.ExpandAll();
        }

        private bool SaveChanges()
        {
            string json = JsonConvert.SerializeObject(m_file);
            try
            {
                File.WriteAllText(m_file.PathName, json);
                m_file.IsDirty = false;
            }
            catch(Exception ex)
            {
                MessageBox.Show("Could not write output file - " + ex.Message, "Error Saving File",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            return true;
        }

        private bool PathGroupExists(string name)
        {
            foreach(TreeNode node in m_pathfile_tree.Nodes)
            {
                if (node.Text == name)
                    return true;
            }

            return false;
        }

        private string GetNewPathGroupName()
        {
            int index = 0;
            string newname;

            while (true)
            {
                if (index == 0)
                    newname = "NewPathGroup";
                else
                    newname = "NewPathGroup " + index.ToString();

                if (!PathGroupExists(newname))
                    break;

                index++;
            }

            return newname;
        }

        private bool PathExists(TreeNode tn, string name)
        {
            foreach (TreeNode node in tn.Nodes)
            {
                if (node.Text == name)
                    return true;
            }

            return false;
        }

        private string GetNewPathName(TreeNode tn)
        {
            int index = 0;
            string newname;

            while (true)
            {
                if (index == 0)
                    newname = "NewPath";
                else
                    newname = "NewPath " + index.ToString();

                if (!PathExists(tn, newname))
                    break;

                index++;
            }

            return newname;
        }

        private void EditGroupOrPathName(TreeNode tn)
        {
            if (m_text_editor != null)
                return;

            Rectangle b = new Rectangle(tn.Bounds.Left, tn.Bounds.Top, m_pathfile_tree.Width, tn.Bounds.Height);

            m_editing_pathtree = tn;
            m_text_editor = new TextBox();
            m_text_editor.Text = tn.Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_pathfile_tree;
            m_text_editor.Enabled = true;
            m_text_editor.Visible = true;
            m_text_editor.LostFocus += FinishedEditingNamePathTree;
            m_text_editor.PreviewKeyDown += PreviewEditorKeyPathTree;
            m_text_editor.Focus();
        }

        private void StopEditing()
        {
            m_text_editor.Dispose();
            m_text_editor = null;
            m_editing_pathtree = null;
            m_waypoint_editing = null;
            m_robot_param_editing = null;
        }

        private void GenerateSplines(RobotPath p)
        {
            p.ClearSplines();
            p.Segments = null;
            if (m_generator != null)
            {
                try
                {
                    p.GenerateSegments(m_file.Robot, m_generator);
                    p.GenerateSplines(m_generator);
                }
                catch(Exception ex)
                {
                    string msg = "In path generator '";
                    msg += m_generator.Name;
                    msg += "' spline generation failed - " + ex.Message;
                    m_logger.LogMessage(Logger.MessageType.Warning, msg);
                    p.ClearSplines();
                }
            }
        }

        private void GenerateAllSplines()
        {
            foreach(PathGroup group in m_file.Groups)
            {
                foreach(RobotPath path in group.Paths)
                {
                    GenerateSplines(path);
                }
            }
        }
        #endregion

    }
}
