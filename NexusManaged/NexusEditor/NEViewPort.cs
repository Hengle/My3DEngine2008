﻿using System;
using System.Drawing;
using System.Windows.Forms;
using NexusEngine;
using NexusEngineExtension;

namespace NexusEditor
{
    public enum EditorCameraMode
    { 
        Perspective,
        Top,
        Front,
        Side
    }

	public delegate void CameraOperateFactorChange(int delta);
    /// <summary>
    /// 管理编辑器窗口的一个渲染窗口
    /// </summary>
    public class NEViewport : Panel
    {
        protected NViewport m_view;
        protected NEditorCameraController m_cameraCtrl;
        protected EditorCameraMode m_mode;
        protected ContextMenuStrip m_menu;
        protected NGameTimer m_mainTimer;
        protected string m_focusLevel;
		protected bool m_isRightMouseDown;
		protected float m_cameraSight;
		protected float m_minCameraSight;
		protected float m_maxCameraSight;
		protected float m_Sight;
        /// <summary>
        /// cache鼠标右键按下的位置，client坐标
        /// </summary>
        public Point RightMouseButtenDownPosition { get; set; }

		public CameraOperateFactorChange CameraOperateFactorChangeEvent;

        public NEViewport()
        {            
            m_view = new NViewport();
            int w = NEditorEngine.Instance().Config.ClientWidth;
            int h = NEditorEngine.Instance().Config.ClientHeight;
			m_minCameraSight = 500;
			m_maxCameraSight = 300000;
			m_Sight = 0.25f;
			m_cameraSight=m_minCameraSight*(1-m_Sight*m_Sight)+m_maxCameraSight*(m_Sight*m_Sight);
			m_view.Camera.SetPerspective(MathConst.PI / 4, w, h, 1.0f, m_cameraSight);
            m_cameraCtrl = new NPerspectiveCameraController();
            InitializeComponent();
            this.Dock = DockStyle.Fill;
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);

            m_view.HandleWnd = this.Handle;
            m_view.RenderMode = ERenderMode.Unlit;
            this.NEViewPort_SizeChanged(this, null); // set view pos/size

            NEWidgetsRender widgets = CreateWidgetsRender();
            m_view.WidgetsRender = widgets;
            widgets.AddViewport(this);

            //-- context menu
            m_menu = new ContextMenuStrip();            
            ToolStripMenuItem viewMenu = new ToolStripMenuItem("Render mode");            
            foreach (ERenderMode rm in Enum.GetValues(typeof(ERenderMode)))
            {
                ToolStripItem item = viewMenu.DropDownItems.Add(rm.ToString());
                item.Click += new EventHandler(this.OnMenuClickRenderMode);
            }
            m_menu.Items.Add(viewMenu);

            ToolStripItem cameraItem = m_menu.Items.Add("Reset camera");

            //-- 
            m_mainTimer = new NGameTimer();
            m_mainTimer.Reset();
        }

		public float Sight
		{
			get	{return m_Sight;}
			set
			{
				m_Sight=value;
				m_cameraSight = m_minCameraSight * (1 - m_Sight * m_Sight) + m_maxCameraSight * (m_Sight * m_Sight);
				int w = NEditorEngine.Instance().Config.ClientWidth;
				int h = NEditorEngine.Instance().Config.ClientHeight;
				if (m_cameraCtrl is NPerspectiveCameraController)
				{
					m_view.Camera.SetPerspective(MathConst.PI / 4, w, h, 20, m_cameraSight);
				}
				else
				{
					m_view.Camera.SetOrtho(w, h, 20, m_cameraSight);
				}
				
			}			
		}

        protected override void Dispose(bool disposing)
        {
            m_view.Destroy();
            base.Dispose(disposing);
        }

        protected virtual NEWidgetsRender CreateWidgetsRender()
        {
            return new NEWidgetsRender();
        }
        

        private void OnMenuClickRenderMode(Object sender, EventArgs e)
        {
            ToolStripItem item = (ToolStripItem)sender;

            ERenderMode rm = (ERenderMode)(Enum.Parse(typeof(ERenderMode), item.Text));
            m_view.RenderMode = rm;
        }

        public ERenderMode ViewRenderMode
        {
            get { return m_view.RenderMode; }
            set 
            {
                m_view.RenderMode = value;
                this.Refresh();
            }
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);

            this.Focus();
            if(e.Button == MouseButtons.Right)
            {
                System.Drawing.Rectangle rc = new Rectangle(0, 0, 18 * 6, 18);
                if (rc.Contains(e.Location))
                {
                    m_menu.Show(this, e.Location);
                }
            }
        }

        protected virtual void RenderLevel()
        {
            if (m_focusLevel.Length <= 0)
                return;

            m_mainTimer.Tick();
            m_cameraCtrl.UpdateCamera(m_view.Camera);

            NEditorEngine eng = NexusEditor.Program.engine;

            //-- draw level
            eng.UpdateLevel(m_focusLevel, m_mainTimer.DeltaTime, m_view);

            if (m_focusLevel == eng.MainLevelName)
                eng.RenderMainLevel(m_focusLevel, m_view);
            else
                eng.RenderLevel(m_focusLevel, m_view, true);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);

            RenderLevel();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // do nothing
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            // 设置鼠标Capture保证鼠标释放消息肯定能发生达到该窗口
            NativeMethods.CaptureMouse(this.Handle, true);
            NativeRectangle clipRect = (NativeRectangle)this.RectangleToScreen(this.ClientRectangle);
            NativeMethods.ClipCursor(ref clipRect);
            if(e.Button == MouseButtons.Right)
            {
				m_isRightMouseDown = true;
                while (NativeMethods.ShowCursor(false) >= 0) ;
                RightMouseButtenDownPosition = e.Location;
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            NativeMethods.ClipCursor(IntPtr.Zero);
            NativeMethods.CaptureMouse(this.Handle, false);
            while (NativeMethods.ShowCursor(true) < 0) ;
            if (e.Button == MouseButtons.Right)
            {
                Point screenPoint = this.PointToScreen(RightMouseButtenDownPosition);
                NativeMethods.SetCursorPos(screenPoint.X, screenPoint.Y);
				m_isRightMouseDown=false;
            }
        }

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if (m_isRightMouseDown)
			{
				//改变摄像机运行速度

				if (CameraOperateFactorChangeEvent!=null)
				{
					CameraOperateFactorChangeEvent(e.Delta);
				}
			}
			base.OnMouseWheel(e);
		}

        protected override void OnLostFocus(EventArgs e)
        {
            base.OnLostFocus(e);

            NativeMethods.CaptureMouse(this.Handle, false);
            while (NativeMethods.ShowCursor(true) < 0) ;
        }

        virtual public void DrawWidgets(NRenderPrimitiveDrawInterface PDI)
        {
            PDI.DrawDebugString(3, 3, m_mode.ToString(), new Color4ub(0, 5, 5, 255));
            PDI.DrawDebugString(2, 2, m_mode.ToString(), new Color4ub(255, 250, 250, 255));

            //-- draw axis            
            int p = 32;            
            Vector3 c = m_view.Camera.ScreenToWorld(p, (int)this.m_view.Height - p);
            Vector3 t = m_view.Camera.ScreenToWorld(p, (int)this.m_view.Height);
            float d = Vector3.Distance(c, t);

            c += m_view.Camera.Forward*d;
            Vector3 cx = c;
            cx.x += d;
            Vector3 cy = c;
            cy.y += d;
            Vector3 cz = c;
            cz.z += d;

            float g = 195.0f / 256.0f;
            PDI.BeginLineBatch();
            
            //-- draw axis
            PDI.DrawLine(c, cx, new Color4f(g, 0, 0));
            PDI.DrawLine(c, cy, new Color4f(0, g, 0));
            PDI.DrawLine(c, cz, new Color4f(0, 0, g));

            int offset = -8;            
            System.Drawing.Point vp = m_view.Camera.WorldToScreen(cx);            
            PDI.DrawDebugString(vp.X, vp.Y + offset, "X", new Color4ub(195, 0, 0, 255));

            vp = m_view.Camera.WorldToScreen(cy);            
            PDI.DrawDebugString(vp.X, vp.Y + offset, "Y", new Color4ub(0, 195, 0, 255));

            vp = m_view.Camera.WorldToScreen(cz);            
            PDI.DrawDebugString(vp.X, vp.Y + offset, "Z", new Color4ub(0, 0, 195, 255));

            //-- draw xz plane
            int num = 20;
            float grid = 100;
            Color4f gridColorLight = new Color4f(148 / 256.0f, 148 / 256.0f, 148 / 256.0f);
            Color4f gridColor = new Color4f(118 / 256.0f, 118 / 256.0f, 118 / 256.0f);
            Color4f blackColor = new Color4f();
            for (int z=1; z<num; z++)
            {
                float px = num * grid;
                float pz = z * grid;

                Color4f ct = gridColorLight;

                if (z % 10 == 0)
                    ct = gridColor;

                PDI.DrawLine(new Vector3(-px, 0, pz), new Vector3(px, 0, pz), ct);
                PDI.DrawLine(new Vector3(-px, 0, -pz), new Vector3(px, 0, -pz), ct);

                PDI.DrawLine(new Vector3(pz, 0, px), new Vector3(pz, 0, -px), ct);
                PDI.DrawLine(new Vector3(-pz, 0, px), new Vector3(-pz, 0, -px), ct);
            }

            float l = num * grid;
            PDI.DrawLine(new Vector3(-l, 0, 0), new Vector3(l, 0, 0), blackColor);
            PDI.DrawLine(new Vector3(0, 0, -l), new Vector3(0, 0, l), blackColor);
            
            PDI.EndLineBatch();            
        }

        public void SetCameraMode(EditorCameraMode md)
        { 
            m_mode = md;
            switch (md)
            {
            case EditorCameraMode.Perspective:
                m_cameraCtrl = new NPerspectiveCameraController();
            	break;
            case EditorCameraMode.Top:
                {
                    NOrthoCameraController cameraCtrl = new NOrthoCameraController();
                    cameraCtrl.SetOrientation('Y');
                    m_cameraCtrl = cameraCtrl;
                }
                break;
            case EditorCameraMode.Front:
                {
                    NOrthoCameraController cameraCtrl = new NOrthoCameraController();
                    cameraCtrl.SetOrientation('Z');
                    m_cameraCtrl = cameraCtrl;
                }
                break;
            case EditorCameraMode.Side:
                {
                    NOrthoCameraController cameraCtrl = new NOrthoCameraController();
                    cameraCtrl.SetOrientation('X');
                    m_cameraCtrl = cameraCtrl;
                }
                break;

            }
        }   

        public void SetViewLODBias(float val)
        {
            m_view.LODBias = val;
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // NEViewPort
            // 
            this.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Leave += new System.EventHandler(this.NEViewPort_Leave);
            this.Enter += new System.EventHandler(this.NEViewPort_Enter);
            this.SizeChanged += new System.EventHandler(this.NEViewPort_SizeChanged);
            this.ResumeLayout(false);

        }

        private void NEViewPort_SizeChanged(object sender, EventArgs e)
        {            
            m_view.Width = (uint)this.ClientSize.Width;
            m_view.Height = (uint)this.ClientSize.Height;

            NEngineConfig eng = NEditorEngine.Instance().Config;
            m_view.X = (uint)(eng.ClientWidth - m_view.Width) / 2;
            m_view.Y = (uint)(eng.ClientHeight - m_view.Height) / 2;
			m_cameraSight = m_minCameraSight * (1 - m_Sight * m_Sight) + m_maxCameraSight * (m_Sight * m_Sight);
            m_view.Camera.SetPerspective(MathConst.PI / 4, ClientSize.Width, ClientSize.Height, 20, m_cameraSight);
        }

        private void NEViewPort_Enter(object sender, EventArgs e)
        {
            this.Refresh();
        }

        private void NEViewPort_Leave(object sender, EventArgs e)
        {
            this.Refresh();
        }

        public NEditorCameraController CameraCtrl
        {
            get { return m_cameraCtrl; }
        }

		public NViewport Viewport
		{
			get { return m_view; }
		}

        protected override bool IsInputKey(Keys keyData)
        {
            switch (keyData)
            {
                case Keys.Up:
                case Keys.Down:
                case Keys.Right:
                case Keys.Left:
                case Keys.Up | Keys.Shift:
                case Keys.Down | Keys.Shift:
                case Keys.Right | Keys.Shift:
                case Keys.Left | Keys.Shift:
                case Keys.PrintScreen:
                case Keys.F9:
                    return true;
            }
            return base.IsInputKey(keyData);
        }
    }
}
