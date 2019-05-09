using System;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using Newtonsoft.Json;

namespace PathViewer
{
    public partial class PathViewerForm : Form
    {
        GameManager m_games;
        GeneratorManager m_generators;
        PathFile m_file;
        TextBox m_text_editor;
        TreeNode m_editing_pathtree;
        ListViewItem m_editing_property;

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

            m_props.DoubleClick += PropertyDoubleClick;

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
        }


        private void FieldKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                e.Handled = true;
                m_field.DeleteSelectedWaypoint();
            }
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


        private void PropertyDoubleClick(object sender, EventArgs e)
        {
            if (m_text_editor != null || m_props.SelectedItems.Count != 1 || m_field.SelectedWaypoint == null)
                return;

            ListViewItem item = m_props.SelectedItems[0];
            if (item.Text == "Group" || item.Text == "Path")
                return;

            m_editing_property = item;
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

        private void DoPathTreeDoubleClick(object sender, EventArgs e)
        {
            TreeNode tn = m_pathfile_tree.SelectedNode;
            if (tn != null)
            {
                EditGroupOrPathName(tn);
            }
        }

        private void FieldMouseMoved(object sender, FieldMouseMovedArgs e)
        {
            string pos = "Position: " + e.Point.X.ToString() + ", " + e.Point.Y.ToString();
            m_pos_status.Text = pos;
        }

        private void GameSelected(object sender, EventArgs args)
        {
            string game = sender.ToString();
            InitializeGame(game);
        }

        private void InitializeGame(string name)
        {
            Game g = m_games[name];
            m_field.FieldGame = g;
        }

        private void WayPointSelectedOrChanged(object sender, WaypointEventArgs e)
        {
            UpdateWaypointPropertyWindow(e.Group, e.Path, e.Point);
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
            m_editing_property = null;
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

        private void PreviewEditorKeyProperty(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyData == Keys.Cancel || e.KeyData == Keys.Escape)
            {
                StopEditing();
            }
            else if (e.KeyData == Keys.Return)
            {
                PathGroup group;
                RobotPath path;

                FinishedEditingProperty(sender, EventArgs.Empty);
                m_file.FindPathByWaypoint(m_field.SelectedWaypoint, out group, out path);
                UpdateWaypointPropertyWindow(group, path, m_field.SelectedWaypoint);
            }
        }

        private void FinishedEditingProperty(object sender, EventArgs e)
        {
            double d;

            if (!double.TryParse(m_text_editor.Text, out d))
                return;

            if (m_editing_property.Text == "X")
            {
                m_field.SelectedWaypoint.X = d;
                m_field.Invalidate();
            }
            else if (m_editing_property.Text == "Y")
            {
                m_field.SelectedWaypoint.Y = d;
                m_field.Invalidate();
            }
            else if (m_editing_property.Text == "Heading")
            {
                m_field.SelectedWaypoint.Heading = d;
                m_field.Invalidate();
            }
            else if (m_editing_property.Text == "Velocity")
            {
                m_field.SelectedWaypoint.Velocity = d;
                m_field.Invalidate();
            }

            StopEditing();
        }
    }
}
