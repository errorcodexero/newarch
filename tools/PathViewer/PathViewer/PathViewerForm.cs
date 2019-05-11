using System;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using Newtonsoft.Json;

namespace PathViewer
{
    public partial class PathViewerForm : Form
    {
        #region private member variables
        /// <summary>
        /// The set of games that are available
        /// </summary>
        GameManager m_games;

        /// <summary>
        /// THe set of path generators that are available
        /// </summary>
        GeneratorManager m_generators;

        /// <summary>
        /// The current path file we are editing
        /// </summary>
        PathFile m_file;

        /// <summary>
        /// The control for editing path group names and path names in place
        /// </summary>
        TextBox m_text_editor;

        /// <summary>
        /// The node we are currently editing in the path group and path tree control
        /// </summary>
        TreeNode m_editing_pathtree;

        /// <summary>
        /// The list view item we are editing in the waypoint window or robot params window
        /// </summary>
        ListViewItem m_waypoint_editing;

        /// <summary>
        /// The robot view item we are editing
        /// </summary>
        ListViewItem m_robot_param_editing;

        #endregion

        #region public constructor
        public PathViewerForm()
        {
            m_file = new PathFile();
            m_games = new GameManager();
            m_generators = new GeneratorManager();
            m_editing_pathtree = null;
            m_text_editor = null;
            InitializeComponent();

            Text = "Path Editor - Unsaved";

            m_field.File = m_file;

            m_field.WaypointSelected += WayPointSelectedOrChanged;
            m_field.WaypointChanged += WayPointSelectedOrChanged;
            m_field.FieldMouseMoved += FieldMouseMoved;
            m_field.KeyDown += FieldKeyDown;

            m_pathfile_tree.DoubleClick += DoPathTreeDoubleClick;
            m_pathfile_tree.CheckBoxes = true;
            m_pathfile_tree.AfterCheck += PathTreeCheckBoxChanged;

            m_props.DoubleClick += WaypointDoubleClick;

            m_robot.DoubleClick += RobotParamDoubleClick;

            ContextMenu menu = new ContextMenu();
            MenuItem mi = new MenuItem("Add Path Group", NewPathGroupToolStripMenuItem_Click);
            menu.MenuItems.Add(mi);
            mi = new MenuItem("Add Path", NewPathToolStripMenuItem_Click);
            menu.MenuItems.Add(mi);
            m_pathfile_tree.ContextMenu = menu;

            var item = m_menu.Items[1] as ToolStripMenuItem;
            if (item != null)
            {
                foreach (string g in m_games.Games)
                {
                    item.DropDownItems.Add(g, null, GameSelected);
                }
            }

            UpdateRobotWindow();
        }
        #endregion

        #region event handlers for the field child control

        private void FieldKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                e.Handled = true;
                m_field.DeleteSelectedWaypoint();
            }
            else if (e.KeyCode == Keys.Insert)
            {
                e.Handled = true;
                m_field.InsertWaypoint();
            }
        }

        private void FieldMouseMoved(object sender, FieldMouseMovedArgs e)
        {
            string pos = "Position: " + e.Point.X.ToString() + ", " + e.Point.Y.ToString();
            m_pos_status.Text = pos;
        }
        private void WayPointSelectedOrChanged(object sender, WaypointEventArgs e)
        {
            UpdateWaypointPropertyWindow(e.Group, e.Path, e.Point);
        }
        #endregion

        #region event handlers for the path tree control

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
            if (m_text_editor != null || m_props.SelectedItems.Count != 1 || m_field.SelectedWaypoint == null)
                return;

            ListViewItem item = m_props.SelectedItems[0];
            if (item.Text == "Group" || item.Text == "Path")
                return;

            m_waypoint_editing = item;
            Rectangle b = new Rectangle(item.SubItems[1].Bounds.Left, item.SubItems[1].Bounds.Top, item.SubItems[1].Bounds.Width, item.SubItems[1].Bounds.Height);

            m_text_editor = new TextBox();
            m_text_editor.Text = item.SubItems[1].Text;
            m_text_editor.Bounds = b;
            m_text_editor.Parent = m_props;
            m_text_editor.Enabled = true;
            m_text_editor.Visible = true;
            m_text_editor.LostFocus += FinishedEditingProperty;
            m_text_editor.PreviewKeyDown += PreviewEditorKeyProperty;
            m_text_editor.Focus();
        }
        #endregion

        #region event handlers for the top level form

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
            if (m_robot_param_editing != null)
                FinishedEditingRobotParam();
            else if (m_waypoint_editing != null)
                FinishedEditingWaypoint();

            StopEditing();
        }

        private void FinishedEditingRobotParam()
        {
            double d;

            if (!double.TryParse(m_text_editor.Text, out d))
                return;

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
            UpdateRobotWindow();
        }

        private void FinishedEditingWaypoint()
        {
            double d;

            if (!double.TryParse(m_text_editor.Text, out d))
                return;

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
            UpdateWaypointPropertyWindow(group, path, m_field.SelectedWaypoint);
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
                if (m_file.RenameGroup(m_editing_pathtree.Text, m_text_editor.Text))
                {
                    m_editing_pathtree.Text = m_text_editor.Text;
                    StopEditing();
                }
            }
            else
            {
                if (m_file.RenamePath(m_editing_pathtree.Parent.Text, m_editing_pathtree.Text, m_text_editor.Text))
                {
                    m_editing_pathtree.Text = m_text_editor.Text;
                    StopEditing();
                }
            }
        }
        #endregion

        #region event handlers for the menus
        private void GameSelected(object sender, EventArgs args)
        {
            string game = sender.ToString();
            InitializeGame(game);
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
                UpdateSidePanel();
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
        #endregion

        #region private methods
        private void InitializeGame(string name)
        {
            Game g = m_games[name];
            m_field.FieldGame = g;
        }

        private void UpdateWaypointPropertyWindow(PathGroup group, RobotPath path, WayPoint pt)
        {
            ListViewItem item;

            m_props.Items.Clear();

            item = new ListViewItem("X");
            item.SubItems.Add(pt.X.ToString());
            m_props.Items.Add(item);

            item = new ListViewItem("Y");
            item.SubItems.Add(pt.Y.ToString());
            m_props.Items.Add(item);

            item = new ListViewItem("Heading");
            item.SubItems.Add(pt.Heading.ToString());
            m_props.Items.Add(item);

            item = new ListViewItem("Velocity");
            item.SubItems.Add(pt.Velocity.ToString());
            m_props.Items.Add(item);
        }
        
        private void UpdateRobotWindow()
        {
            if (m_file!= null && m_file.Robot != null)
            {
                ListViewItem item;

                m_robot.Items.Clear();

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

        private void UpdateSidePanel()
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
        #endregion

    }
}
