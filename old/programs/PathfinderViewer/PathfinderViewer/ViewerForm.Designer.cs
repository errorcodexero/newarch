namespace PathfinderViewer
{
    partial class ViewerForm
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
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createPositionalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createPositionVelocityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_slider = new PathfinderViewer.DoubleSlider();
            this.m_graphs = new PathfinderViewer.GraphPanel();
            this.m_menu.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_menu
            // 
            this.m_menu.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.m_menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.graphToolStripMenuItem});
            this.m_menu.Location = new System.Drawing.Point(0, 0);
            this.m_menu.Name = "m_menu";
            this.m_menu.Size = new System.Drawing.Size(1702, 33);
            this.m_menu.TabIndex = 2;
            this.m_menu.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(50, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(217, 30);
            this.loadToolStripMenuItem.Text = "Load Dataset ...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadFileCommand);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(217, 30);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.ExitCommand);
            // 
            // graphToolStripMenuItem
            // 
            this.graphToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createPositionalToolStripMenuItem,
            this.createPositionVelocityToolStripMenuItem});
            this.graphToolStripMenuItem.Name = "graphToolStripMenuItem";
            this.graphToolStripMenuItem.Size = new System.Drawing.Size(72, 29);
            this.graphToolStripMenuItem.Text = "Graph";
            // 
            // createPositionalToolStripMenuItem
            // 
            this.createPositionalToolStripMenuItem.Name = "createPositionalToolStripMenuItem";
            this.createPositionalToolStripMenuItem.Size = new System.Drawing.Size(280, 30);
            this.createPositionalToolStripMenuItem.Text = "Create XY";
            this.createPositionalToolStripMenuItem.Click += new System.EventHandler(this.CreateXYGraph);
            // 
            // createPositionVelocityToolStripMenuItem
            // 
            this.createPositionVelocityToolStripMenuItem.Name = "createPositionVelocityToolStripMenuItem";
            this.createPositionVelocityToolStripMenuItem.Size = new System.Drawing.Size(280, 30);
            this.createPositionVelocityToolStripMenuItem.Text = "Create Position Velocity";
            this.createPositionVelocityToolStripMenuItem.Click += new System.EventHandler(this.CreatePosVelGraph);
            // 
            // m_slider
            // 
            this.m_slider.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_slider.LeftValue = 25D;
            this.m_slider.Location = new System.Drawing.Point(12, 861);
            this.m_slider.Margin = new System.Windows.Forms.Padding(10, 3, 10, 3);
            this.m_slider.Maximum = 100D;
            this.m_slider.Minimum = 0D;
            this.m_slider.Name = "m_slider";
            this.m_slider.RightValue = 75D;
            this.m_slider.Size = new System.Drawing.Size(1678, 77);
            this.m_slider.TabIndex = 3;
            // 
            // m_graphs
            // 
            this.m_graphs.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_graphs.Location = new System.Drawing.Point(12, 36);
            this.m_graphs.Name = "m_graphs";
            this.m_graphs.Size = new System.Drawing.Size(1678, 819);
            this.m_graphs.TabIndex = 4;
            // 
            // ViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1702, 950);
            this.Controls.Add(this.m_graphs);
            this.Controls.Add(this.m_slider);
            this.Controls.Add(this.m_menu);
            this.MainMenuStrip = this.m_menu;
            this.Name = "ViewerForm";
            this.Text = "Pathfinder Viewer";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.m_menu.ResumeLayout(false);
            this.m_menu.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip m_menu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private DoubleSlider m_slider;
        private System.Windows.Forms.ToolStripMenuItem graphToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createPositionalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createPositionVelocityToolStripMenuItem;
        private GraphPanel m_graphs;
    }
}

