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
            this.gamesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pathsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newPathGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newPathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generatorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.noneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_split = new System.Windows.Forms.SplitContainer();
            this.m_vertical = new System.Windows.Forms.SplitContainer();
            this.m_field = new PathViewer.FieldView();
            this.m_right_vertical = new System.Windows.Forms.SplitContainer();
            this.m_pathfile_tree = new System.Windows.Forms.TreeView();
            this.m_props = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.m_pos_status = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.m_menu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_split)).BeginInit();
            this.m_split.Panel1.SuspendLayout();
            this.m_split.Panel2.SuspendLayout();
            this.m_split.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_vertical)).BeginInit();
            this.m_vertical.Panel1.SuspendLayout();
            this.m_vertical.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_right_vertical)).BeginInit();
            this.m_right_vertical.Panel1.SuspendLayout();
            this.m_right_vertical.Panel2.SuspendLayout();
            this.m_right_vertical.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_menu
            // 
            this.m_menu.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.m_menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.gamesToolStripMenuItem,
            this.pathsToolStripMenuItem});
            this.m_menu.Location = new System.Drawing.Point(0, 0);
            this.m_menu.Name = "m_menu";
            this.m_menu.Padding = new System.Windows.Forms.Padding(7, 2, 0, 2);
            this.m_menu.Size = new System.Drawing.Size(1288, 33);
            this.m_menu.TabIndex = 1;
            this.m_menu.Text = "MainMenu";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripSeparator1,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(50, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(175, 30);
            this.loadToolStripMenuItem.Text = "Load ...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(172, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(175, 30);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.SaveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(175, 30);
            this.saveAsToolStripMenuItem.Text = "Save As ...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.SaveAsToolStripMenuItem_Click);
            // 
            // gamesToolStripMenuItem
            // 
            this.gamesToolStripMenuItem.Name = "gamesToolStripMenuItem";
            this.gamesToolStripMenuItem.Size = new System.Drawing.Size(78, 29);
            this.gamesToolStripMenuItem.Text = "Games";
            // 
            // pathsToolStripMenuItem
            // 
            this.pathsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newPathGroupToolStripMenuItem,
            this.newPathToolStripMenuItem,
            this.generatorToolStripMenuItem});
            this.pathsToolStripMenuItem.Name = "pathsToolStripMenuItem";
            this.pathsToolStripMenuItem.Size = new System.Drawing.Size(66, 29);
            this.pathsToolStripMenuItem.Text = "Paths";
            // 
            // newPathGroupToolStripMenuItem
            // 
            this.newPathGroupToolStripMenuItem.Name = "newPathGroupToolStripMenuItem";
            this.newPathGroupToolStripMenuItem.Size = new System.Drawing.Size(225, 30);
            this.newPathGroupToolStripMenuItem.Text = "New Path Group";
            this.newPathGroupToolStripMenuItem.Click += new System.EventHandler(this.NewPathGroupToolStripMenuItem_Click);
            // 
            // newPathToolStripMenuItem
            // 
            this.newPathToolStripMenuItem.Name = "newPathToolStripMenuItem";
            this.newPathToolStripMenuItem.Size = new System.Drawing.Size(225, 30);
            this.newPathToolStripMenuItem.Text = "New Path";
            this.newPathToolStripMenuItem.Click += new System.EventHandler(this.NewPathToolStripMenuItem_Click);
            // 
            // generatorToolStripMenuItem
            // 
            this.generatorToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.noneToolStripMenuItem});
            this.generatorToolStripMenuItem.Name = "generatorToolStripMenuItem";
            this.generatorToolStripMenuItem.Size = new System.Drawing.Size(225, 30);
            this.generatorToolStripMenuItem.Text = "Generator";
            // 
            // noneToolStripMenuItem
            // 
            this.noneToolStripMenuItem.Name = "noneToolStripMenuItem";
            this.noneToolStripMenuItem.Size = new System.Drawing.Size(139, 30);
            this.noneToolStripMenuItem.Text = "None";
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
            this.m_split.Panel2.Controls.Add(this.m_right_vertical);
            this.m_split.Size = new System.Drawing.Size(1288, 578);
            this.m_split.SplitterDistance = 997;
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
            this.m_vertical.Panel1.Controls.Add(this.m_field);
            // 
            // m_vertical.Panel2
            // 
            this.m_vertical.Panel2.AccessibleRole = System.Windows.Forms.AccessibleRole.MenuBar;
            this.m_vertical.Size = new System.Drawing.Size(997, 578);
            this.m_vertical.SplitterDistance = 343;
            this.m_vertical.SplitterWidth = 5;
            this.m_vertical.TabIndex = 0;
            // 
            // m_field
            // 
            this.m_field.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_field.FieldGame = null;
            this.m_field.File = null;
            this.m_field.Location = new System.Drawing.Point(0, 0);
            this.m_field.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_field.Name = "m_field";
            this.m_field.Size = new System.Drawing.Size(997, 343);
            this.m_field.TabIndex = 2;
            // 
            // m_right_vertical
            // 
            this.m_right_vertical.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_right_vertical.Location = new System.Drawing.Point(0, 0);
            this.m_right_vertical.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_right_vertical.Name = "m_right_vertical";
            this.m_right_vertical.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // m_right_vertical.Panel1
            // 
            this.m_right_vertical.Panel1.Controls.Add(this.m_pathfile_tree);
            // 
            // m_right_vertical.Panel2
            // 
            this.m_right_vertical.Panel2.Controls.Add(this.m_props);
            this.m_right_vertical.Size = new System.Drawing.Size(287, 578);
            this.m_right_vertical.SplitterDistance = 310;
            this.m_right_vertical.SplitterWidth = 5;
            this.m_right_vertical.TabIndex = 0;
            // 
            // m_pathfile_tree
            // 
            this.m_pathfile_tree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pathfile_tree.Location = new System.Drawing.Point(0, 0);
            this.m_pathfile_tree.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_pathfile_tree.Name = "m_pathfile_tree";
            this.m_pathfile_tree.Size = new System.Drawing.Size(287, 310);
            this.m_pathfile_tree.TabIndex = 0;
            // 
            // m_props
            // 
            this.m_props.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.m_props.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_props.FullRowSelect = true;
            this.m_props.Location = new System.Drawing.Point(0, 0);
            this.m_props.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.m_props.Name = "m_props";
            this.m_props.Size = new System.Drawing.Size(287, 263);
            this.m_props.TabIndex = 0;
            this.m_props.UseCompatibleStateImageBehavior = false;
            this.m_props.View = System.Windows.Forms.View.Details;
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
            this.m_pos_status.Size = new System.Drawing.Size(113, 25);
            this.m_pos_status.Text = "Position: 0, 0";
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_pos_status});
            this.statusStrip1.Location = new System.Drawing.Point(0, 611);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 16, 0);
            this.statusStrip1.Size = new System.Drawing.Size(1288, 30);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // PathViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1288, 641);
            this.Controls.Add(this.m_split);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.m_menu);
            this.MainMenuStrip = this.m_menu;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
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
            ((System.ComponentModel.ISupportInitialize)(this.m_vertical)).EndInit();
            this.m_vertical.ResumeLayout(false);
            this.m_right_vertical.Panel1.ResumeLayout(false);
            this.m_right_vertical.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_right_vertical)).EndInit();
            this.m_right_vertical.ResumeLayout(false);
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
        private System.Windows.Forms.ListView m_props;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ToolStripMenuItem gamesToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel m_pos_status;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TreeView m_pathfile_tree;
        private System.Windows.Forms.ToolStripMenuItem pathsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newPathGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newPathToolStripMenuItem;
        private System.Windows.Forms.SplitContainer m_right_vertical;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generatorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem noneToolStripMenuItem;
    }
}

