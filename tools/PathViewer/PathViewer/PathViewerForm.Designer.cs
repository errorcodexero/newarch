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
            this.m_menu = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pathsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newPathGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newPathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gamesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generatorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_split = new System.Windows.Forms.SplitContainer();
            this.m_vertical = new System.Windows.Forms.SplitContainer();
            this.m_field = new PathViewer.FieldView();
            this.m_bottom_tab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.m_plot = new PathViewer.RobotPlotViewer();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.textBox1 = new System.Windows.Forms.TextBox();
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
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.generatePathsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generatePathsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.m_menu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_split)).BeginInit();
            this.m_split.Panel1.SuspendLayout();
            this.m_split.Panel2.SuspendLayout();
            this.m_split.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_vertical)).BeginInit();
            this.m_vertical.Panel1.SuspendLayout();
            this.m_vertical.Panel2.SuspendLayout();
            this.m_vertical.SuspendLayout();
            this.m_bottom_tab.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.m_flow.SuspendLayout();
            this.m_right_one.SuspendLayout();
            this.m_right_two.SuspendLayout();
            this.m_right_three.SuspendLayout();
            this.m_right_four.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
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
            this.m_menu.Size = new System.Drawing.Size(1568, 28);
            this.m_menu.TabIndex = 1;
            this.m_menu.Text = "MainMenu";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripSeparator1,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator3,
            this.generatePathsToolStripMenuItem,
            this.generatePathsToolStripMenuItem1});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(216, 26);
            this.loadToolStripMenuItem.Text = "Load ...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(213, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(216, 26);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.SaveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(216, 26);
            this.saveAsToolStripMenuItem.Text = "Save As ...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.SaveAsToolStripMenuItem_Click);
            // 
            // pathsToolStripMenuItem
            // 
            this.pathsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newPathGroupToolStripMenuItem,
            this.newPathToolStripMenuItem,
            this.toolStripSeparator2,
            this.undoToolStripMenuItem});
            this.pathsToolStripMenuItem.Name = "pathsToolStripMenuItem";
            this.pathsToolStripMenuItem.Size = new System.Drawing.Size(47, 24);
            this.pathsToolStripMenuItem.Text = "Edit";
            // 
            // newPathGroupToolStripMenuItem
            // 
            this.newPathGroupToolStripMenuItem.Name = "newPathGroupToolStripMenuItem";
            this.newPathGroupToolStripMenuItem.Size = new System.Drawing.Size(189, 26);
            this.newPathGroupToolStripMenuItem.Text = "Add Path Group";
            this.newPathGroupToolStripMenuItem.Click += new System.EventHandler(this.NewPathGroupToolStripMenuItem_Click);
            // 
            // newPathToolStripMenuItem
            // 
            this.newPathToolStripMenuItem.Name = "newPathToolStripMenuItem";
            this.newPathToolStripMenuItem.Size = new System.Drawing.Size(189, 26);
            this.newPathToolStripMenuItem.Text = "Add Path";
            this.newPathToolStripMenuItem.Click += new System.EventHandler(this.NewPathToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(186, 6);
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(189, 26);
            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.UndoToolStripMenuItem_Click);
            // 
            // gamesToolStripMenuItem
            // 
            this.gamesToolStripMenuItem.Name = "gamesToolStripMenuItem";
            this.gamesToolStripMenuItem.Size = new System.Drawing.Size(66, 24);
            this.gamesToolStripMenuItem.Text = "Games";
            // 
            // generatorsToolStripMenuItem
            // 
            this.generatorsToolStripMenuItem.Name = "generatorsToolStripMenuItem";
            this.generatorsToolStripMenuItem.Size = new System.Drawing.Size(93, 24);
            this.generatorsToolStripMenuItem.Text = "Generators";
            // 
            // m_split
            // 
            this.m_split.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_split.Location = new System.Drawing.Point(0, 28);
            this.m_split.Name = "m_split";
            // 
            // m_split.Panel1
            // 
            this.m_split.Panel1.Controls.Add(this.m_vertical);
            // 
            // m_split.Panel2
            // 
            this.m_split.Panel2.Controls.Add(this.m_flow);
            this.m_split.Size = new System.Drawing.Size(1568, 876);
            this.m_split.SplitterDistance = 1213;
            this.m_split.TabIndex = 3;
            // 
            // m_vertical
            // 
            this.m_vertical.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_vertical.Location = new System.Drawing.Point(0, 0);
            this.m_vertical.Name = "m_vertical";
            this.m_vertical.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // m_vertical.Panel1
            // 
            this.m_vertical.Panel1.Controls.Add(this.m_field);
            // 
            // m_vertical.Panel2
            // 
            this.m_vertical.Panel2.AccessibleRole = System.Windows.Forms.AccessibleRole.MenuBar;
            this.m_vertical.Panel2.Controls.Add(this.m_bottom_tab);
            this.m_vertical.Size = new System.Drawing.Size(1213, 876);
            this.m_vertical.SplitterDistance = 519;
            this.m_vertical.TabIndex = 0;
            // 
            // m_field
            // 
            this.m_field.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_field.FieldGame = null;
            this.m_field.File = null;
            this.m_field.Location = new System.Drawing.Point(0, 0);
            this.m_field.Name = "m_field";
            this.m_field.Path = null;
            this.m_field.SelectedWaypoint = null;
            this.m_field.Size = new System.Drawing.Size(1213, 519);
            this.m_field.TabIndex = 2;
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
            this.m_bottom_tab.Size = new System.Drawing.Size(1213, 353);
            this.m_bottom_tab.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.m_plot);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage1.Size = new System.Drawing.Size(1205, 324);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Plots";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // m_plot
            // 
            this.m_plot.AutoScroll = true;
            this.m_plot.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_plot.Generator = null;
            this.m_plot.HighlightTime = 0D;
            this.m_plot.Location = new System.Drawing.Point(3, 2);
            this.m_plot.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_plot.Name = "m_plot";
            this.m_plot.Path = null;
            this.m_plot.Robot = null;
            this.m_plot.Size = new System.Drawing.Size(1199, 320);
            this.m_plot.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.textBox1);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage2.Size = new System.Drawing.Size(1205, 324);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Messages";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(3, 2);
            this.textBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(1199, 320);
            this.textBox1.TabIndex = 0;
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
            this.m_flow.Name = "m_flow";
            this.m_flow.Size = new System.Drawing.Size(351, 876);
            this.m_flow.TabIndex = 3;
            // 
            // m_right_one
            // 
            this.m_right_one.Controls.Add(this.m_pathfile_tree);
            this.m_right_one.Location = new System.Drawing.Point(3, 2);
            this.m_right_one.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_one.Name = "m_right_one";
            this.m_right_one.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_one.Size = new System.Drawing.Size(351, 76);
            this.m_right_one.TabIndex = 0;
            this.m_right_one.TabStop = false;
            this.m_right_one.Text = "Paths";
            // 
            // m_pathfile_tree
            // 
            this.m_pathfile_tree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pathfile_tree.Location = new System.Drawing.Point(3, 17);
            this.m_pathfile_tree.Name = "m_pathfile_tree";
            this.m_pathfile_tree.Size = new System.Drawing.Size(345, 57);
            this.m_pathfile_tree.TabIndex = 0;
            // 
            // m_right_two
            // 
            this.m_right_two.Controls.Add(this.m_robot_view);
            this.m_right_two.Location = new System.Drawing.Point(3, 82);
            this.m_right_two.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_two.Name = "m_right_two";
            this.m_right_two.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_two.Size = new System.Drawing.Size(351, 112);
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
            this.m_robot_view.Location = new System.Drawing.Point(3, 17);
            this.m_robot_view.Name = "m_robot_view";
            this.m_robot_view.Size = new System.Drawing.Size(345, 93);
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
            this.m_right_three.Location = new System.Drawing.Point(3, 198);
            this.m_right_three.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_three.Name = "m_right_three";
            this.m_right_three.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_three.Size = new System.Drawing.Size(351, 93);
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
            this.m_path_view.Location = new System.Drawing.Point(3, 17);
            this.m_path_view.Name = "m_path_view";
            this.m_path_view.Size = new System.Drawing.Size(345, 74);
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
            this.m_right_four.Location = new System.Drawing.Point(3, 295);
            this.m_right_four.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_four.Name = "m_right_four";
            this.m_right_four.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.m_right_four.Size = new System.Drawing.Size(351, 198);
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
            this.m_waypoint_view.Location = new System.Drawing.Point(3, 17);
            this.m_waypoint_view.Name = "m_waypoint_view";
            this.m_waypoint_view.Size = new System.Drawing.Size(345, 179);
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
            this.m_pos_status.Name = "m_pos_status";
            this.m_pos_status.Size = new System.Drawing.Size(91, 20);
            this.m_pos_status.Text = "Position: 0, 0";
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_pos_status});
            this.statusStrip1.Location = new System.Drawing.Point(0, 904);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1568, 25);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(213, 6);
            // 
            // generatePathsToolStripMenuItem
            // 
            this.generatePathsToolStripMenuItem.Name = "generatePathsToolStripMenuItem";
            this.generatePathsToolStripMenuItem.Size = new System.Drawing.Size(219, 26);
            this.generatePathsToolStripMenuItem.Text = "Generate Paths As  ...";
            this.generatePathsToolStripMenuItem.Click += new System.EventHandler(this.GeneratePathsAsToolStripMenuItem_Click);
            // 
            // generatePathsToolStripMenuItem1
            // 
            this.generatePathsToolStripMenuItem1.Name = "generatePathsToolStripMenuItem1";
            this.generatePathsToolStripMenuItem1.Size = new System.Drawing.Size(219, 26);
            this.generatePathsToolStripMenuItem1.Text = "Generate Paths";
            this.generatePathsToolStripMenuItem1.Click += new System.EventHandler(this.GeneratePathsToolStripMenuItem_Click);
            // 
            // PathViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1568, 929);
            this.Controls.Add(this.m_split);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.m_menu);
            this.MainMenuStrip = this.m_menu;
            this.Name = "PathViewerForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Path Editor";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.m_menu.ResumeLayout(false);
            this.m_menu.PerformLayout();
            this.m_split.Panel1.ResumeLayout(false);
            this.m_split.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_split)).EndInit();
            this.m_split.ResumeLayout(false);
            this.m_vertical.Panel1.ResumeLayout(false);
            this.m_vertical.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_vertical)).EndInit();
            this.m_vertical.ResumeLayout(false);
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
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip m_menu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private FieldView m_field;
        private System.Windows.Forms.SplitContainer m_split;
        private System.Windows.Forms.SplitContainer m_vertical;
        private System.Windows.Forms.ListView m_waypoint_view;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ToolStripMenuItem gamesToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel m_pos_status;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TreeView m_pathfile_tree;
        private System.Windows.Forms.ToolStripMenuItem pathsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newPathGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newPathToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ListView m_robot_view;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.GroupBox m_right_one;
        private System.Windows.Forms.GroupBox m_right_two;
        private System.Windows.Forms.GroupBox m_right_four;
        private System.Windows.Forms.ToolStripMenuItem generatorsToolStripMenuItem;
        private RobotPlotViewer m_plot;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.TabControl m_bottom_tab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.GroupBox m_right_three;
        private System.Windows.Forms.ListView m_path_view;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.FlowLayoutPanel m_flow;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem generatePathsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generatePathsToolStripMenuItem1;
    }
}

