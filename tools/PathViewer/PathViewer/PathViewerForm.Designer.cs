namespace PathViewer
{
    partial class PathViewerForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.ToolStripStatusLabel m_misc_status;
            this.m_split = new System.Windows.Forms.SplitContainer();
            this.m_vertical = new System.Windows.Forms.SplitContainer();
            this.m_field_tab = new System.Windows.Forms.TabControl();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.m_field = new PathViewer.PathFieldView();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.m_detailed = new PathViewer.RobotFieldView();
            this.m_bottom_tab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.m_plot = new PathViewer.RobotPlotViewer();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.m_logger_window = new System.Windows.Forms.TextBox();
            this.m_flow = new System.Windows.Forms.FlowLayoutPanel();
            this.m_right_one = new System.Windows.Forms.GroupBox();
            this.m_pathfile_tree = new System.Windows.Forms.TreeView();
            this.m_right_two = new System.Windows.Forms.GroupBox();
            this.m_robot_view = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.m_right_three = new System.Windows.Forms.GroupBox();
            this.m_path_view = new System.Windows.Forms.ListView();
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.m_right_four = new System.Windows.Forms.GroupBox();
            this.m_waypoint_view = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.m_pos_status = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.m_running_status = new System.Windows.Forms.ToolStripStatusLabel();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.generatePathsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generatePathsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.pathsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newPathGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newPathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gamesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generatorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_menu = new System.Windows.Forms.MenuStrip();
            m_misc_status = new System.Windows.Forms.ToolStripStatusLabel();
            ((System.ComponentModel.ISupportInitialize)(this.m_split)).BeginInit();
            this.m_split.Panel1.SuspendLayout();
            this.m_split.Panel2.SuspendLayout();
            this.m_split.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_vertical)).BeginInit();
            this.m_vertical.Panel1.SuspendLayout();
            this.m_vertical.Panel2.SuspendLayout();
            this.m_vertical.SuspendLayout();
            this.m_field_tab.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.m_bottom_tab.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.m_flow.SuspendLayout();
            this.m_right_one.SuspendLayout();
            this.m_right_two.SuspendLayout();
            this.m_right_three.SuspendLayout();
            this.m_right_four.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.m_menu.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_split
            // 
            this.m_split.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_split.Location = new System.Drawing.Point(0, 33);
            this.m_split.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_split.Name = "m_split";
            // 
            // m_split.Panel1
            // 
            this.m_split.Panel1.Controls.Add(this.m_vertical);
            // 
            // m_split.Panel2
            // 
            this.m_split.Panel2.Controls.Add(this.m_flow);
            this.m_split.Size = new System.Drawing.Size(1764, 990);
            this.m_split.SplitterDistance = 1362;
            this.m_split.TabIndex = 3;
            // 
            // m_vertical
            // 
            this.m_vertical.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_vertical.Location = new System.Drawing.Point(0, 0);
            this.m_vertical.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_vertical.Name = "m_vertical";
            this.m_vertical.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // m_vertical.Panel1
            // 
            this.m_vertical.Panel1.Controls.Add(this.m_field_tab);
            // 
            // m_vertical.Panel2
            // 
            this.m_vertical.Panel2.AccessibleRole = System.Windows.Forms.AccessibleRole.MenuBar;
            this.m_vertical.Panel2.Controls.Add(this.m_bottom_tab);
            this.m_vertical.Size = new System.Drawing.Size(1362, 990);
            this.m_vertical.SplitterDistance = 583;
            this.m_vertical.SplitterWidth = 5;
            this.m_vertical.TabIndex = 0;
            // 
            // m_field_tab
            // 
            this.m_field_tab.Controls.Add(this.tabPage3);
            this.m_field_tab.Controls.Add(this.tabPage4);
            this.m_field_tab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_field_tab.Location = new System.Drawing.Point(0, 0);
            this.m_field_tab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_field_tab.Name = "m_field_tab";
            this.m_field_tab.SelectedIndex = 0;
            this.m_field_tab.Size = new System.Drawing.Size(1362, 583);
            this.m_field_tab.TabIndex = 3;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.m_field);
            this.tabPage3.Location = new System.Drawing.Point(4, 29);
            this.tabPage3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage3.Size = new System.Drawing.Size(1354, 550);
            this.tabPage3.TabIndex = 0;
            this.tabPage3.Text = "Editor";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // m_field
            // 
            this.m_field.DisplayedPath = null;
            this.m_field.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_field.FieldGame = null;
            this.m_field.File = null;
            this.m_field.HighlightPoint = null;
            this.m_field.Location = new System.Drawing.Point(3, 2);
            this.m_field.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_field.Name = "m_field";
            this.m_field.SelectedWaypoint = null;
            this.m_field.Size = new System.Drawing.Size(1348, 546);
            this.m_field.TabIndex = 2;
            this.m_field.Units = "inches";
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.m_detailed);
            this.tabPage4.Location = new System.Drawing.Point(4, 29);
            this.tabPage4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage4.Size = new System.Drawing.Size(1354, 550);
            this.tabPage4.TabIndex = 1;
            this.tabPage4.Text = "Robot View";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // m_detailed
            // 
            this.m_detailed.DisplayedPath = null;
            this.m_detailed.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_detailed.FieldGame = null;
            this.m_detailed.Location = new System.Drawing.Point(3, 2);
            this.m_detailed.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_detailed.Name = "m_detailed";
            this.m_detailed.Robot = null;
            this.m_detailed.Size = new System.Drawing.Size(1348, 546);
            this.m_detailed.TabIndex = 0;
            this.m_detailed.Time = 0D;
            // 
            // m_bottom_tab
            // 
            this.m_bottom_tab.Controls.Add(this.tabPage1);
            this.m_bottom_tab.Controls.Add(this.tabPage2);
            this.m_bottom_tab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_bottom_tab.Location = new System.Drawing.Point(0, 0);
            this.m_bottom_tab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_bottom_tab.Name = "m_bottom_tab";
            this.m_bottom_tab.SelectedIndex = 0;
            this.m_bottom_tab.Size = new System.Drawing.Size(1362, 402);
            this.m_bottom_tab.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.m_plot);
            this.tabPage1.Location = new System.Drawing.Point(4, 29);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage1.Size = new System.Drawing.Size(1354, 369);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Plots";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // m_plot
            // 
            this.m_plot.AutoScroll = true;
            this.m_plot.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_plot.Generator = null;
            this.m_plot.Location = new System.Drawing.Point(3, 2);
            this.m_plot.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_plot.Name = "m_plot";
            this.m_plot.Path = null;
            this.m_plot.Robot = null;
            this.m_plot.Size = new System.Drawing.Size(1348, 365);
            this.m_plot.TabIndex = 0;
            this.m_plot.Time = 0D;
            this.m_plot.Units = null;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.m_logger_window);
            this.tabPage2.Location = new System.Drawing.Point(4, 29);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage2.Size = new System.Drawing.Size(1354, 369);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Messages";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // m_logger_window
            // 
            this.m_logger_window.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_logger_window.Location = new System.Drawing.Point(3, 2);
            this.m_logger_window.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_logger_window.Multiline = true;
            this.m_logger_window.Name = "m_logger_window";
            this.m_logger_window.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.m_logger_window.Size = new System.Drawing.Size(1348, 365);
            this.m_logger_window.TabIndex = 0;
            // 
            // m_flow
            // 
            this.m_flow.Controls.Add(this.m_right_one);
            this.m_flow.Controls.Add(this.m_right_two);
            this.m_flow.Controls.Add(this.m_right_three);
            this.m_flow.Controls.Add(this.m_right_four);
            this.m_flow.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_flow.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.m_flow.Location = new System.Drawing.Point(0, 0);
            this.m_flow.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_flow.Name = "m_flow";
            this.m_flow.Size = new System.Drawing.Size(398, 990);
            this.m_flow.TabIndex = 3;
            // 
            // m_right_one
            // 
            this.m_right_one.Controls.Add(this.m_pathfile_tree);
            this.m_right_one.Location = new System.Drawing.Point(3, 2);
            this.m_right_one.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_one.Name = "m_right_one";
            this.m_right_one.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_one.Size = new System.Drawing.Size(395, 95);
            this.m_right_one.TabIndex = 0;
            this.m_right_one.TabStop = false;
            this.m_right_one.Text = "Paths";
            // 
            // m_pathfile_tree
            // 
            this.m_pathfile_tree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pathfile_tree.FullRowSelect = true;
            this.m_pathfile_tree.HideSelection = false;
            this.m_pathfile_tree.Location = new System.Drawing.Point(3, 21);
            this.m_pathfile_tree.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_pathfile_tree.Name = "m_pathfile_tree";
            this.m_pathfile_tree.Size = new System.Drawing.Size(389, 72);
            this.m_pathfile_tree.TabIndex = 0;
            // 
            // m_right_two
            // 
            this.m_right_two.Controls.Add(this.m_robot_view);
            this.m_right_two.Location = new System.Drawing.Point(3, 101);
            this.m_right_two.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_two.Name = "m_right_two";
            this.m_right_two.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_two.Size = new System.Drawing.Size(395, 140);
            this.m_right_two.TabIndex = 0;
            this.m_right_two.TabStop = false;
            this.m_right_two.Text = "Robot Parameters";
            // 
            // m_robot_view
            // 
            this.m_robot_view.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
            this.m_robot_view.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_robot_view.FullRowSelect = true;
            this.m_robot_view.Location = new System.Drawing.Point(3, 21);
            this.m_robot_view.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_robot_view.Name = "m_robot_view";
            this.m_robot_view.Size = new System.Drawing.Size(389, 117);
            this.m_robot_view.TabIndex = 2;
            this.m_robot_view.UseCompatibleStateImageBehavior = false;
            this.m_robot_view.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Name";
            this.columnHeader3.Width = 150;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Value";
            this.columnHeader4.Width = 150;
            // 
            // m_right_three
            // 
            this.m_right_three.Controls.Add(this.m_path_view);
            this.m_right_three.Location = new System.Drawing.Point(3, 245);
            this.m_right_three.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_three.Name = "m_right_three";
            this.m_right_three.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_three.Size = new System.Drawing.Size(395, 116);
            this.m_right_three.TabIndex = 1;
            this.m_right_three.TabStop = false;
            this.m_right_three.Text = "Selected Path";
            // 
            // m_path_view
            // 
            this.m_path_view.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader6});
            this.m_path_view.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_path_view.FullRowSelect = true;
            this.m_path_view.Location = new System.Drawing.Point(3, 21);
            this.m_path_view.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_path_view.Name = "m_path_view";
            this.m_path_view.Size = new System.Drawing.Size(389, 93);
            this.m_path_view.TabIndex = 0;
            this.m_path_view.UseCompatibleStateImageBehavior = false;
            this.m_path_view.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Name";
            this.columnHeader5.Width = 150;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Value";
            this.columnHeader6.Width = 150;
            // 
            // m_right_four
            // 
            this.m_right_four.Controls.Add(this.m_waypoint_view);
            this.m_right_four.Location = new System.Drawing.Point(3, 365);
            this.m_right_four.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_four.Name = "m_right_four";
            this.m_right_four.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_four.Size = new System.Drawing.Size(395, 248);
            this.m_right_four.TabIndex = 0;
            this.m_right_four.TabStop = false;
            this.m_right_four.Text = "Selected Waypoint";
            // 
            // m_waypoint_view
            // 
            this.m_waypoint_view.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.m_waypoint_view.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_waypoint_view.FullRowSelect = true;
            this.m_waypoint_view.Location = new System.Drawing.Point(3, 21);
            this.m_waypoint_view.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_waypoint_view.Name = "m_waypoint_view";
            this.m_waypoint_view.Size = new System.Drawing.Size(389, 225);
            this.m_waypoint_view.TabIndex = 0;
            this.m_waypoint_view.UseCompatibleStateImageBehavior = false;
            this.m_waypoint_view.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 150;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Value";
            this.columnHeader2.Width = 150;
            // 
            // m_pos_status
            // 
            this.m_pos_status.AutoSize = false;
            this.m_pos_status.Name = "m_pos_status";
            this.m_pos_status.Size = new System.Drawing.Size(200, 25);
            this.m_pos_status.Text = "Position: 0, 0";
            this.m_pos_status.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_pos_status,
            this.m_running_status,
            m_misc_status});
            this.statusStrip1.Location = new System.Drawing.Point(0, 1023);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 16, 0);
            this.statusStrip1.Size = new System.Drawing.Size(1764, 32);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // m_running_status
            // 
            this.m_running_status.AutoSize = false;
            this.m_running_status.Name = "m_running_status";
            this.m_running_status.Size = new System.Drawing.Size(250, 25);
            this.m_running_status.Text = "Generation: Idle";
            this.m_running_status.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem2,
            this.loadToolStripMenuItem,
            this.toolStripSeparator1,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator4,
            this.toolStripMenuItem1,
            this.toolStripSeparator3,
            this.generatePathsToolStripMenuItem,
            this.generatePathsToolStripMenuItem1});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(54, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(278, 34);
            this.toolStripMenuItem2.Text = "New";
            this.toolStripMenuItem2.Click += new System.EventHandler(this.NewMenuItemEventHandler);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(278, 34);
            this.loadToolStripMenuItem.Text = "Open ...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(275, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(278, 34);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.SaveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(278, 34);
            this.saveAsToolStripMenuItem.Text = "Save As ...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.SaveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(275, 6);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(278, 34);
            this.toolStripMenuItem1.Text = "Close";
            this.toolStripMenuItem1.Click += new System.EventHandler(this.CloseMenuItemEventHandler);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(275, 6);
            // 
            // generatePathsToolStripMenuItem
            // 
            this.generatePathsToolStripMenuItem.Name = "generatePathsToolStripMenuItem";
            this.generatePathsToolStripMenuItem.Size = new System.Drawing.Size(278, 34);
            this.generatePathsToolStripMenuItem.Text = "Generate Paths As  ...";
            this.generatePathsToolStripMenuItem.Click += new System.EventHandler(this.GeneratePathAsMenuItem);
            // 
            // generatePathsToolStripMenuItem1
            // 
            this.generatePathsToolStripMenuItem1.Name = "generatePathsToolStripMenuItem1";
            this.generatePathsToolStripMenuItem1.Size = new System.Drawing.Size(278, 34);
            this.generatePathsToolStripMenuItem1.Text = "Generate Paths";
            this.generatePathsToolStripMenuItem1.Click += new System.EventHandler(this.GeneratePathMenuItem);
            // 
            // pathsToolStripMenuItem
            // 
            this.pathsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newPathGroupToolStripMenuItem,
            this.newPathToolStripMenuItem,
            this.toolStripSeparator2,
            this.undoToolStripMenuItem});
            this.pathsToolStripMenuItem.Name = "pathsToolStripMenuItem";
            this.pathsToolStripMenuItem.Size = new System.Drawing.Size(58, 29);
            this.pathsToolStripMenuItem.Text = "Edit";
            // 
            // newPathGroupToolStripMenuItem
            // 
            this.newPathGroupToolStripMenuItem.Name = "newPathGroupToolStripMenuItem";
            this.newPathGroupToolStripMenuItem.Size = new System.Drawing.Size(242, 34);
            this.newPathGroupToolStripMenuItem.Text = "Add Path Group";
            this.newPathGroupToolStripMenuItem.Click += new System.EventHandler(this.NewPathGroupToolStripMenuItem_Click);
            // 
            // newPathToolStripMenuItem
            // 
            this.newPathToolStripMenuItem.Name = "newPathToolStripMenuItem";
            this.newPathToolStripMenuItem.Size = new System.Drawing.Size(242, 34);
            this.newPathToolStripMenuItem.Text = "Add Path";
            this.newPathToolStripMenuItem.Click += new System.EventHandler(this.NewPathToolStripMenuItemHandler);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(239, 6);
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(242, 34);
            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.UndoMenuItem);
            // 
            // gamesToolStripMenuItem
            // 
            this.gamesToolStripMenuItem.Name = "gamesToolStripMenuItem";
            this.gamesToolStripMenuItem.Size = new System.Drawing.Size(82, 29);
            this.gamesToolStripMenuItem.Text = "Games";
            // 
            // generatorsToolStripMenuItem
            // 
            this.generatorsToolStripMenuItem.Name = "generatorsToolStripMenuItem";
            this.generatorsToolStripMenuItem.Size = new System.Drawing.Size(114, 29);
            this.generatorsToolStripMenuItem.Text = "Generators";
            // 
            // m_menu
            // 
            this.m_menu.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.m_menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.pathsToolStripMenuItem,
            this.gamesToolStripMenuItem,
            this.generatorsToolStripMenuItem});
            this.m_menu.Location = new System.Drawing.Point(0, 0);
            this.m_menu.Name = "m_menu";
            this.m_menu.Padding = new System.Windows.Forms.Padding(7, 2, 0, 2);
            this.m_menu.Size = new System.Drawing.Size(1764, 33);
            this.m_menu.TabIndex = 1;
            this.m_menu.Text = "MainMenu";
            // 
            // m_misc_status
            // 
            m_misc_status.Name = "m_misc_status";
            m_misc_status.Size = new System.Drawing.Size(0, 25);
            // 
            // PathViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1764, 1055);
            this.Controls.Add(this.m_split);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.m_menu);
            this.MainMenuStrip = this.m_menu;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "PathViewerForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Path Editor";
            this.m_split.Panel1.ResumeLayout(false);
            this.m_split.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_split)).EndInit();
            this.m_split.ResumeLayout(false);
            this.m_vertical.Panel1.ResumeLayout(false);
            this.m_vertical.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_vertical)).EndInit();
            this.m_vertical.ResumeLayout(false);
            this.m_field_tab.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.m_bottom_tab.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.m_flow.ResumeLayout(false);
            this.m_right_one.ResumeLayout(false);
            this.m_right_two.ResumeLayout(false);
            this.m_right_three.ResumeLayout(false);
            this.m_right_four.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.m_menu.ResumeLayout(false);
            this.m_menu.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private PathFieldView m_field;
        private System.Windows.Forms.SplitContainer m_split;
        private System.Windows.Forms.SplitContainer m_vertical;
        private System.Windows.Forms.ListView m_waypoint_view;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ToolStripStatusLabel m_pos_status;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TreeView m_pathfile_tree;
        private System.Windows.Forms.ListView m_robot_view;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.GroupBox m_right_one;
        private System.Windows.Forms.GroupBox m_right_two;
        private System.Windows.Forms.GroupBox m_right_four;
        private RobotPlotViewer m_plot;
        private System.Windows.Forms.TabControl m_bottom_tab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox m_logger_window;
        private System.Windows.Forms.GroupBox m_right_three;
        private System.Windows.Forms.ListView m_path_view;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.FlowLayoutPanel m_flow;
        private System.Windows.Forms.TabControl m_field_tab;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private RobotFieldView m_detailed;
        private System.Windows.Forms.ToolStripStatusLabel m_running_status;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem generatePathsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generatePathsToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem pathsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newPathGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newPathToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem gamesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generatorsToolStripMenuItem;
        private System.Windows.Forms.MenuStrip m_menu;
    }
}

