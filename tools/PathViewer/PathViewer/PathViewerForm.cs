using System;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using System.Collections.Generic;
using Newtonsoft.Json;
using System.ComponentModel;
using System.Threading;
using System.Reflection;
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
        /// The selected path
        /// </summary>
        private RobotPath m_selected_path;

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
        /// The path view item we are editing
        /// </summary>
        private ListViewItem m_path_view_editing;

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

        #region private delegates
        private delegate void HighlightDelegate(RobotPath path, WayPoint pt);
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

            m_plot.TimeCursorMoved += PlotWindowTimeChanged;

            m_pathfile_tree.DoubleClick += DoPathTreeDoubleClick;
            m_pathfile_tree.MouseUp += PathTreeMouseUp;
            m_pathfile_tree.AfterSelect += PathTreeSelectionChanged;

            m_waypoint_view.DoubleClick += WaypointDoubleClick;
            m_robot_view.DoubleClick += RobotParamDoubleClick;
            m_path_view.DoubleClick += PathViewDoubleClick;

            m_ignore_lost_focus = false;
            m_undo_stack = new List<UndoState>();
            m_undo_serial = 0;
            m_undo_length = 10;

            PopulateGameMenu();
            PopulateGeneratorMenu();
            UpdateRobotWindow();

            m_plot.Generator = m_generator;
            m_plot.Robot = m_file.Robot;
            m_detailed.Robot = m_file.Robot;

            m_plot.HighlightTime = Double.MaxValue;

            m_logger.OutputAvailable += LoggerOutputAvailable;

            OutputCopyright();
            WheelsToolStripMenuItem_Click(m_wheels_menu_item, null);

        }
        #endregion

        #region event handlers for the logger window
        private void LoggerOutputAvailable(object sender, LoggerOutputEventArgs e)
        {
            if (e.MessageType != Logger.MessageType.Info)
                m_logger_window.Text += e.MessageType.ToString() + ": " + e.MessageText + "\r\n";
            else
                m_logger_window.Text += e.MessageText + "\r\n";
        }
        #endregion

        #region event handlers for the plot child control

        private void PlotWindowTimeChanged(object sender, TimeCursorMovedArgs e)
        {
            Nullable<PointF> wpt = FindPointAtTime(m_selected_path, e.Time);
            if (wpt == null)
                return;

            m_field.HighlightPoint = wpt;
            m_field.SelectedWaypoint = null;
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
                e.KeyCode != Keys.PageDown && e.KeyCode != Keys.PageUp &&
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
                {
                    gen = true;
                    m_file.IsDirty = true;
                }
            }
            else if (e.KeyCode == Keys.Insert)
            {
                WayPoint pt = path.InsertPoint(m_field.SelectedWaypoint, m_selected_path.MaxVelocity);
                if (pt != null)
                {
                    gen = true;
                    m_file.IsDirty = true;
                    m_field.SelectedWaypoint = pt;
                }
            }
            else if (e.KeyCode == Keys.Up)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Y += 0.1;
                else
                    m_field.SelectedWaypoint.Y += 1.0;

                m_file.IsDirty = true;
                gen = true;
            }
            else if (e.KeyCode == Keys.Down)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Y -= 0.1;
                else
                    m_field.SelectedWaypoint.Y -= 1.0;

                m_file.IsDirty = true;
                gen = true;
            }
            else if (e.KeyCode == Keys.Left)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.X -= 0.1;
                else
                    m_field.SelectedWaypoint.X -= 1.0;

                m_file.IsDirty = true;
                gen = true;
            }
            else if (e.KeyCode == Keys.Right)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.X += 0.1;
                else
                    m_field.SelectedWaypoint.X += 1.0;

                m_file.IsDirty = true;
                gen = true;
            }
            else if (e.KeyCode == Keys.Add || e.KeyCode == Keys.PageUp)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Heading = MathUtils.BoundDegrees(m_field.SelectedWaypoint.Heading + 0.5);
                else
                    m_field.SelectedWaypoint.Heading = MathUtils.BoundDegrees(m_field.SelectedWaypoint.Heading + 5.0);

                m_file.IsDirty = true;
                gen = true;
            }
            else if (e.KeyCode == Keys.Subtract || e.KeyCode == Keys.PageDown)
            {
                if (e.Shift)
                    m_field.SelectedWaypoint.Heading = MathUtils.BoundDegrees(m_field.SelectedWaypoint.Heading - 0.5);
                else
                    m_field.SelectedWaypoint.Heading = MathUtils.BoundDegrees(m_field.SelectedWaypoint.Heading - 5.0);

                m_file.IsDirty = true;
                gen = true;
            }

            if (gen)
            {
                m_field.Invalidate();
                m_detailed.Invalidate();
                GenerateSplines(path);
                GenerateSegments(path);
                if (m_field.SelectedWaypoint != null)
                    UpdateWaypointPropertyWindow();
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
                if (e.Reason == WaypointEventArgs.ReasonType.Selected)
                    HighlightTime(e.Path, e.Point);

                if (e.Reason == WaypointEventArgs.ReasonType.StartChange)
                    PushUndoStack();

                if (e.Reason == WaypointEventArgs.ReasonType.Changing)
                    GenerateSplines(e.Path);

                if (e.Reason == WaypointEventArgs.ReasonType.EndChange)
                {
                    GenerateSplines(e.Path);
                    GenerateSegments(e.Path);
                    HighlightTime(e.Path, e.Point);
                }
            }

            if (e.Point != null)
                UpdateWaypointPropertyWindow();

            if (e.Reason == WaypointEventArgs.ReasonType.Unselected)
                m_plot.HighlightTime = Double.MaxValue;
        }
        #endregion

        #region event handlers for the path tree control

        private void PathTreeSelectionChanged(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null)
            {
                // A group is selected
                m_plot.Path = null;
                m_selected_path = null;
            }
            else
            {
                RobotPath path = m_file.FindPathByName(e.Node.Parent.Text, e.Node.Text);
                m_selected_path = path;
                m_plot.Path = path;
                m_field.DisplayedPath = path;
                m_detailed.DisplayedPath = path;
            }
            m_field.SelectedWaypoint = null;
            UpdatePathWindow();
        }

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

        #region event handlers for the path view window

        private void PathViewDoubleClick(object sender, EventArgs e)
        {
            if (m_text_editor != null || m_path_view.SelectedItems.Count != 1)
                return;

            ListViewItem item = m_path_view.SelectedItems[0];

            m_path_view_editing = item;
            Rectangle b = new Rectangle(item.SubItems[1].Bounds.Left, item.SubItems[1].Bounds.Top, item.SubItems[1].Bounds.Width, item.SubItems[1].Bounds.Height);

            m_text_editor = new TextBox();
            m_text_editor.Text = item.SubItems[1].Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_path_view;
            m_text_editor.Enabled = true;
            m_text_editor.Visible = true;
            m_text_editor.LostFocus += FinishedEditingProperty;
            m_text_editor.PreviewKeyDown += PreviewEditorKeyProperty;
            m_text_editor.Focus();
        }
        #endregion

        #region event handlers for the robot params window
        private void RobotParamDoubleClick(object sender, EventArgs e)
        {
            if (m_text_editor != null || m_robot_view.SelectedItems.Count != 1)
                return;

            ListViewItem item = m_robot_view.SelectedItems[0];

            m_robot_param_editing = item;
            Rectangle b = new Rectangle(item.SubItems[1].Bounds.Left, item.SubItems[1].Bounds.Top, item.SubItems[1].Bounds.Width, item.SubItems[1].Bounds.Height);

            m_text_editor = new TextBox();
            m_text_editor.Text = item.SubItems[1].Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_robot_view;
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
            if (m_text_editor != null || m_waypoint_view.SelectedItems.Count != 1 || m_field.SelectedWaypoint == null)
                return;

            ListViewItem item = m_waypoint_view.SelectedItems[0];
            if (item.Text == "Group" || item.Text == "Path")
                return;

            m_waypoint_editing = item;
            Rectangle b = new Rectangle(item.SubItems[1].Bounds.Left, item.SubItems[1].Bounds.Top, item.SubItems[1].Bounds.Width, item.SubItems[1].Bounds.Height);

            m_text_editor = new TextBox();
            m_text_editor.Text = item.SubItems[1].Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_waypoint_view;
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
            m_right_one.Width = m_flow.Width - m_flow.Margin.Left - m_flow.Margin.Right;
            m_right_two.Width = m_flow.Width - m_flow.Margin.Left - m_flow.Margin.Right;
            m_right_three.Width = m_flow.Width - m_flow.Margin.Left - m_flow.Margin.Right;
            m_right_four.Width = m_flow.Width - m_flow.Margin.Left - m_flow.Margin.Right;

            int height = m_flow.Height - m_flow.Margin.Top - m_flow.Margin.Bottom;
            height -= m_right_one.Margin.Top + m_right_one.Margin.Bottom;
            height -= m_right_two.Margin.Top + m_right_two.Margin.Bottom;
            height -= m_right_three.Margin.Top + m_right_three.Margin.Bottom;
            height -= m_right_four.Margin.Top + m_right_four.Margin.Bottom;

            m_right_one.Height = (int)(height * 0.40);
            m_right_two.Height = (int)(height * 0.20) ;
            m_right_three.Height = (int)(height * 0.20);
            m_right_four.Height = (int)(height * 0.20);

            m_vertical.SplitterDistance = m_vertical.Height * 5/ 8;
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
            else if (m_path_view_editing != null)
                stop = FinishedEditingPathView();

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

        private void CloseMenuItemEventHandler(object sender, EventArgs e)
        {
            if (m_file.IsDirty)
            {
                DialogResult dr = MessageBox.Show("You have unsaved changes. Are you sure you want to close this path file", "Really Close", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (dr == DialogResult.No)
                    return;
            }

            m_file = new PathFile();
            m_field.File = m_file;
            m_field.DisplayedPath = null;
            m_field.SelectedWaypoint = null;
            m_field.HighlightPoint = null;
            m_detailed.DisplayedPath = null;
            m_plot.Path = null;
            m_selected_path = null;
            UpdateRobotWindow();
            UpdateWaypointPropertyWindow();
            UpdatePathWindow();
            UpdatePathTree();
        }

        private void GeneratePathsAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_file.IsDirty)
            {
                DialogResult dr = MessageBox.Show("You have unsaved changes. Do you want to save before generating the paths", "Save?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Cancel)
                    return;

                if (dr == DialogResult.Yes)
                    SaveAsToolStripMenuItem_Click(sender, e);
            }

            FolderBrowserDialog dialog = new FolderBrowserDialog();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_file.OutputDirectory = dialog.SelectedPath;
                GeneratePaths();
            }
        }

        private void GeneratePathsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_file.IsDirty)
            {
                DialogResult dr = MessageBox.Show("You have unsaved changes. Do you want to save before generating the paths", "Save?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Cancel)
                    return;

                if (dr == DialogResult.Yes)
                    SaveAsToolStripMenuItem_Click(sender, e);
            }

            if (string.IsNullOrEmpty(m_file.OutputDirectory))
                GeneratePathsAsToolStripMenuItem_Click(sender, e);
            else
                GeneratePaths();
        }

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
                try
                {
                    m_file = JsonConvert.DeserializeObject<PathFile>(json);
                }
                catch(Newtonsoft.Json.JsonSerializationException ex)
                {
                    string msg = "Cannot load path file '" + dialog.FileName + "' - invalid file contents - " + ex.Message;
                    MessageBox.Show(msg, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                m_file.PathName = dialog.FileName;
                m_field.File = m_file;
                m_plot.Robot = m_file.Robot;
                m_detailed.Robot = m_file.Robot;
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
            m_pathfile_tree.SelectedNode = node;

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
            m_pathfile_tree.SelectedNode = nnode;
            m_file.AddPath(node.Text, newname);
            PathTreeSelectionChanged(m_pathfile_tree, new TreeViewEventArgs(nnode));
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
            if (m_field.DisplayedPath == path)
                m_field.DisplayedPath = null;

            if (m_detailed.DisplayedPath == path)
                m_detailed.DisplayedPath = null;

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

            if (m_field.DisplayedPath != null)
            {
                PathGroup gr = m_file.FindGroupByPath(m_field.DisplayedPath);
                if (gr != null)
                    st.AddPath(gr.Name, m_field.DisplayedPath.Name);
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
                m_detailed.Invalidate();
                m_detailed.Robot = m_file.Robot;
                m_plot.Robot = m_file.Robot;
                m_plot.Invalidate();
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
                                    m_pathfile_tree.SelectedNode = path;
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

        #region list view editing methods
        private bool FinishedEditingPathView()
        {
            double d;

            if (!double.TryParse(m_text_editor.Text, out d))
                return false;

            if (m_path_view_editing.Text == "Max Velocity")
            {
                PushUndoStack();
                m_selected_path.MaxVelocity = d;
            }
            else if (m_path_view_editing.Text == "Max Acceleration")
            {
                PushUndoStack();
                m_selected_path.MaxAcceleration = d;
            }
            else if (m_path_view_editing.Text == "Max Jerk")
            {
                PushUndoStack();
                m_selected_path.MaxJerk = d;
            }
            else if (m_path_view_editing.Text == "Start Angle")
            {
                if (d <= -180.0 && d > 180.0)
                    return false;

                PushUndoStack();
                m_selected_path.StartAngle = d;
            }
            else if (m_path_view_editing.Text == "End Angle")
            {
                if (d <= -180.0 && d > 180.0)
                    return false;

                PushUndoStack();
                m_selected_path.EndAngle = d;
            }

            UpdatePathWindow() ;
            m_field.Invalidate();
            m_detailed.Invalidate();
            GenerateAllSplines();

            return true;
        }

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
                    m_file.IsDirty = true;
                }
                else if (m_robot_param_editing.Text == "Length")
                {
                    m_file.Robot.Length = d;
                    m_file.IsDirty = true;
                }
                else if (m_robot_param_editing.Text == "Max Velocity")
                {
                    m_file.Robot.MaxVelocity = d;
                    m_file.IsDirty = true;
                }
                else if (m_robot_param_editing.Text == "Max Acceleration")
                {
                    m_file.Robot.MaxAcceleration = d;
                    m_file.IsDirty = true;
                }
                else if (m_robot_param_editing.Text == "Max Jerk")
                {
                    m_file.Robot.MaxJerk = d;
                    m_file.IsDirty = true;
                }
            }

            UpdateExistingPaths();
            UpdateRobotWindow();
            GenerateAllSplines();

            m_field.Invalidate();
            m_detailed.Invalidate();
            UpdatePathWindow();
            return true;
        }

        private void UpdateExistingPaths()
        {
            foreach(PathGroup gr in m_file.Groups)
            {
                foreach(RobotPath p in gr.Paths)
                {
                    if (p.MaxVelocity > m_file.Robot.MaxVelocity)
                    {
                        m_file.IsDirty = true;
                        p.MaxVelocity = m_file.Robot.MaxVelocity;
                    }

                    if (p.MaxAcceleration > m_file.Robot.MaxAcceleration)
                    {
                        m_file.IsDirty = true;
                        p.MaxAcceleration = m_file.Robot.MaxAcceleration;
                    }

                    if (p.MaxJerk > m_file.Robot.MaxJerk)
                    {
                        m_file.IsDirty = true;
                        p.MaxJerk = m_file.Robot.MaxJerk;
                    }
                }
            }
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
            UpdateWaypointPropertyWindow();

            GenerateSplines(path);
            GenerateSegments(path);

            return true;
        }
        #endregion

        private Nullable<PointF> FindPointAtTime(RobotPath p, double t)
        {
            double x, y;
            double heading;

            PathSegment[] segs = p.Segments;
            if (segs == null)
                return null;

            p.GetPositionForTime(segs, t, out x, out y, out heading);
            return new Nullable<PointF>(new PointF((float)x, (float)y));
        }

        private double FindTime(RobotPath path, WayPoint pt)
        {
            double lowdist = Double.MaxValue;
            double time = 0.0;

            for(int i = 0; i < path.Segments.Length; i++)
            {
                double x = path.Segments[i].GetValue("x");
                double y = path.Segments[i].GetValue("y");

                double dx = x - pt.X;
                double dy = y - pt.Y;

                double dist = dx * dx + dy * dy;
                if (dist < lowdist)
                {
                    time = path.Segments[i].GetValue("time");
                    lowdist = dist;
                }
            }

            return time;
        }



        private void HighlightTime(RobotPath path, WayPoint pt)
        {
            if (!path.HasSegments)
            {
                m_plot.HighlightTime = Double.MaxValue;
                path.SegmentsUpdated += Path_SegmentsUpdated;
                return;
            }

            double time = FindTime(path, pt);
            m_plot.HighlightTime = time;
        }

        private void Path_SegmentsUpdated(object sender, EventArgs e)
        {
            RobotPath path = sender as RobotPath;
            if (path != null)
            {
                path.SegmentsUpdated -= Path_SegmentsUpdated;
                if (InvokeRequired)
                    Invoke(new HighlightDelegate(HighlightTime), new object[] { path, m_field.SelectedWaypoint });
                else
                    HighlightTime(path, m_field.SelectedWaypoint);
            }
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
            m_detailed.FieldGame = g;
        }

        private void InitializeGenerator(PathGenerator g)
        {
            m_generator = g;
            GenerateAllSplines();
        }

        private void UpdateWaypointPropertyWindow()
        {
            ListViewItem item;

            m_waypoint_view.Items.Clear();

            if (m_field.SelectedWaypoint != null)
            {
                item = new ListViewItem("X");
                item.SubItems.Add(m_field.SelectedWaypoint.X.ToString("F1"));
                m_waypoint_view.Items.Add(item);

                item = new ListViewItem("Y");
                item.SubItems.Add(m_field.SelectedWaypoint.Y.ToString("F1"));
                m_waypoint_view.Items.Add(item);

                item = new ListViewItem("Heading");
                item.SubItems.Add(m_field.SelectedWaypoint.Heading.ToString("F1"));
                m_waypoint_view.Items.Add(item);

                item = new ListViewItem("Velocity");
                item.SubItems.Add(m_field.SelectedWaypoint.Velocity.ToString("F1"));
                m_waypoint_view.Items.Add(item);
            }
        }

        private void UpdatePathWindow()
        {
            ListViewItem item;

            m_path_view.Items.Clear();
            if (m_selected_path != null)
            {
                item = new ListViewItem("Max Velocity");
                item.SubItems.Add(m_selected_path.MaxVelocity.ToString());
                m_path_view.Items.Add(item);

                item = new ListViewItem("Max Acceleration");
                item.SubItems.Add(m_selected_path.MaxAcceleration.ToString());
                m_path_view.Items.Add(item);

                item = new ListViewItem("Max Jerk");
                item.SubItems.Add(m_selected_path.MaxJerk.ToString());
                m_path_view.Items.Add(item);

                if (m_file.Robot.DriveType == RobotParams.SwerveDriveType)
                {
                    item = new ListViewItem("Start Angle");
                    item.SubItems.Add(m_selected_path.StartAngle.ToString());
                    m_path_view.Items.Add(item);

                    item = new ListViewItem("End Angle");
                    item.SubItems.Add(m_selected_path.EndAngle.ToString());
                    m_path_view.Items.Add(item);
                }
            }
        }
        
        private void UpdateRobotWindow()
        {
            if (m_file!= null && m_file.Robot != null)
            {
                ListViewItem item;

                m_robot_view.Items.Clear();

                item = new ListViewItem("Type");
                item.SubItems.Add(m_file.Robot.DriveType);
                m_robot_view.Items.Add(item);

                item = new ListViewItem("Width");
                item.SubItems.Add(m_file.Robot.Width.ToString());
                m_robot_view.Items.Add(item);

                item = new ListViewItem("Length");
                item.SubItems.Add(m_file.Robot.Length.ToString());
                m_robot_view.Items.Add(item);

                item = new ListViewItem("Max Velocity");
                item.SubItems.Add(m_file.Robot.MaxVelocity.ToString());
                m_robot_view.Items.Add(item);

                item = new ListViewItem("Max Acceleration");
                item.SubItems.Add(m_file.Robot.MaxAcceleration.ToString());
                m_robot_view.Items.Add(item);

                item = new ListViewItem("Max Jerk");
                item.SubItems.Add(m_file.Robot.MaxJerk.ToString());
                m_robot_view.Items.Add(item);
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

        private void GenerateSegments(RobotPath p)
        {
            p.GenerateVelocityConstraints();
            p.SetSegmentsInvalid();

            if (m_generator != null)
            {
                try
                {
                    p.GenerateSegments(m_file.Robot, m_generator);
                }
                catch(Exception ex)
                {
                    string msg = "In path generator '";
                    msg += m_generator.Name;
                    msg += "' detailed path generation failed - " + ex.Message;
                    m_logger.LogMessage(Logger.MessageType.Warning, msg);
                }
            }
        }

        private void GenerateSplines(RobotPath p)
        {
            p.ClearSplines();

            if (m_generator != null)
            {
                try
                {

                    p.GenerateSplines(m_generator);
                }
                catch (Exception ex)
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
                    GenerateSegments(path);
                }
            }
        }

        static string[] fields = { "time", "x", "y", "position", "velocity", "acceleration", "jerk" };
        private void WritePath(string grname, string pathname, string suffix, PathSegment[] segs)
        {
            string filename;

            PathSegment seg = segs[0];
            bool first = true ;

            if (string.IsNullOrEmpty(suffix))
                filename = Path.Combine(m_file.OutputDirectory, grname + "_" + pathname + ".csv");
            else
                filename = Path.Combine(m_file.OutputDirectory, grname + "_" + pathname + "." + suffix + ".csv");

            using (StreamWriter writer = new StreamWriter(filename))
            {
                for (int i = 0; i < fields.Length; i++)
                {
                    if (seg.HasValue(fields[i]))
                    {
                        if (!first)
                            writer.Write(',');

                        writer.Write('"');
                        writer.Write(fields[i]);
                        writer.Write("'");

                        first = false;
                    }
                }
                writer.WriteLine();

                for (int j = 0; j < segs.Length; j++)
                {
                    first = true;

                    for (int i = 0; i < fields.Length; i++)
                    {
                        if (segs[j].HasValue(fields[i]))
                        {
                            if (!first)
                                writer.Write(',');

                            writer.Write(segs[j].GetValue(fields[i]));
                            first = false;
                        }
                    }
                    writer.WriteLine();
                }
            }
        }

        private void GeneratePaths()
        {
            foreach(PathGroup gr in m_file.Groups)
            {
                foreach(RobotPath path in gr.Paths)
                {
                    path.Segments = null;
                    m_generator.GenerateSegments(m_file.Robot, path);

                    while (!path.HasSegments)
                    {
                        //
                        // Wait for the background thread to generate the path
                        //
                        Thread.Sleep(1);
                    }

                    if (path.Segments != null && path.Segments.Length > 0)
                    {
                        WritePath(gr.Name, path.Name, string.Empty, path.Segments);
                        if (path.AdditionalSegments != null)
                        {
                            foreach (var pair in path.AdditionalSegments)
                                WritePath(gr.Name, path.Name, pair.Key, pair.Value);
                        }
                    }
                }
            }

            MessageBox.Show("Paths written to directory '" + m_file.OutputDirectory + "'", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private string[] MyCopyright =
        {
            "Copyright 2019 Jack W. (Butch) Griffin",
            "",
            "MIT License",
            "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation",
            "files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use,",
            "copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom",
            "the Software is furnished to do so, subject to the following conditions:",
            "",
            "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the",
            "Software.",
            "",
            "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO",
            "THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS",
            "OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR",
            "OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
        };

        private string[] JSonCopyright =
        {
            "",
            "",
            "------------------------------------------------------------------------------------------",
            "This program uses the JSON package from NewtonSoft (https://www.newtonsoft.com/json)",
            "",
            "MIT License",
            "Copyright (c) _____",
            "",
            "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation",
            "files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use,",
            "copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom",
            "the Software is furnished to do so, subject to the following conditions:",
            "",
            "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the",
            "Software.",
            "",
            "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO",
            "THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS",
            "OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR",
            "OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
        };

        private string[] CSVCopyRight =
        {
            "",
            "",
            "------------------------------------------------------------------------------------------",
            "This program uses the CSV package from Steven Hansen",
            "",
            "The MIT License (MIT)",
            "",
            "Copyright (c) 2015 Steve Hansen",
            "",
            "Permission is hereby granted, free of charge, to any person obtaining a copy",
            "of this software and associated documentation files (the \"Software\"), to deal",
            "in the Software without restriction, including without limitation the rights",
            "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell",
            "copies of the Software, and to permit persons to whom the Software is",
            "furnished to do so, subject to the following conditions:",
            "",
            "The above copyright notice and this permission notice shall be included in all",
            "copies or substantial portions of the Software.",
            "",
            "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR",
            "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,",
            "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE",
            "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER",
            "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,",
            "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE",
            "SOFTWARE."
        };

        private void OutputCopyright()
        {
            m_logger.LogMessage(Logger.MessageType.Info, "PathViewer Version " + Assembly.GetEntryAssembly().GetName().Version.ToString());
            m_logger.LogMessage(Logger.MessageType.Info, "");

            foreach (string str in MyCopyright)
                m_logger.LogMessage(Logger.MessageType.Info, str);

            foreach (string str in JSonCopyright)
                m_logger.LogMessage(Logger.MessageType.Info, str);

            foreach (string str in CSVCopyRight)
                m_logger.LogMessage(Logger.MessageType.Info, str);
        }
        #endregion

        private void ClearChecks(ToolStripMenuItem menu)
        {
            foreach(var item in menu.DropDownItems)
            {
                ToolStripMenuItem mitem = item as ToolStripMenuItem;
                if (mitem != null)
                    mitem.Checked = false;
            }
        }

        private void WheelsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem mitem = sender as ToolStripMenuItem;
            ClearChecks(m_detailed_path_view_menu);
            mitem.Checked = true;
            m_detailed.ViewType = DetailedFieldView.ViewTypeValue.WheelView;
        }

        private void RobotToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem mitem = sender as ToolStripMenuItem;
            ClearChecks(m_detailed_path_view_menu);
            mitem.Checked = true;
            m_detailed.ViewType = DetailedFieldView.ViewTypeValue.RobotView;
        }
    }
}
