#ifndef _NEXUS_D3D9_UI_CANVAS_H_
#define _NEXUS_D3D9_UI_CANVAS_H_
#include "d3d_ptr.h"

namespace nexus
{
	struct UI_SCREEN_VERTEX 
	{
		union
		{
			struct  
			{
				float		x,y,z,w;
				D3DCOLOR	color;
				float		tu, tv;
				float		tu2, tv2;
			};
			struct  
			{
				D3DXVECTOR4	pos;
				D3DCOLOR	color;
				D3DXVECTOR2	uv;
				D3DXVECTOR2	uv2;
			};
		};		

		static DWORD FVF;
	};

	struct UI_SCREEN_VERTEX_UNTEX 
	{
		union
		{
			struct  
			{
				float		x,y,z,w;
				D3DCOLOR	color;
			};
			struct  
			{
				D3DXVECTOR4	pos;
				D3DCOLOR	color;
			};
		};		

		static DWORD FVF;
	};

	/**
	* ʵ��2D��Ⱦ
	*/ 
	class d3d9_ui_canvas :
		public nui_canvas
	{
	public:
		d3d9_ui_canvas(void);
		virtual ~d3d9_ui_canvas(void);

		virtual void begin(bool depth_enable=false);

		virtual vector2 get_viewport_size();
		
		// ���òü�����
		virtual void push_scissor_rect(const frect* rect);
		// pop����from�ü�����
		virtual void pop_scissor_rect();
		// ��ȡ��ǰ�ü�����Ĵ�С
		virtual void get_scissor_rect(frect& rect);

		// ����ͼ�񣬲�ʹ��2D���飬�ĸ���������в�ͬ��ɫ��
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,const color_rect& color = color_rect(0xFFFFFFFF) );
		// ʹ��2D�������ͼ��
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,argb_t color = 0xFFFFFFFF );
		// ʹ��2D�������ͼ�񣬴���Ⱥ����ţ���Ŀ��λ������Ϊ��������
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float scale,argb_t color = 0xFFFFFFFF );
		// ʹ��2D�������ͼ�񣬴���Ⱥ����ţ���Ҫָ����������
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float scale,const vector2& scale_center,argb_t color = 0xFFFFFFFF );
		// ʹ��2D�������ͼ�񣬴���ת����Ҫָ����ת����
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float rotation,const vector2& rot_center,argb_t color = 0xFFFFFFFF );
		//ʹ��alphaͼ����ָ��ͼ��,�����ƻ��ƾ������ĵ���ת
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, nrender_texture2D* alpha_texture,const frect& alpha_source_area,const frect& target_area,float rotation,argb_t color = 0xFFFFFFFF );
		
		// �����ı�
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format);
		virtual void draw_string(nrender_font* font, const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color);
		//����Ⱥ����ŵĻ����ı�
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,unsigned int color, unsigned int shadow_color, unsigned int format);
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format);
		
		// ����һ�����ο�
		virtual void draw_rect(const frect& target_area,const color_rect& color);
		virtual void draw_rect(const frect& target_area,float z,float scale,const color_rect& color);
		virtual void draw_rect(const frect& target_area,float z,float scale,const vector2& scale_center,const color_rect& color);
		// ����
		virtual void draw_line( fpoint* points, unsigned int num_points, unsigned int color );
		virtual void draw_line( fpoint* points, unsigned int num_points, float z,float scale,const vector2& scale_center,unsigned int color );

		// ������κ���������cooldown����
		virtual void draw_polygon(fpoint* points,unsigned int num_points, unsigned int color = 0xFFFFFFFF );
		virtual void draw_polygon(fpoint* points,unsigned int num_points, float z,float scale,const vector2& scale_center,unsigned int color = 0xFFFFFFFF );

		virtual void flush();

		virtual void end();
		//-------------------------------------------------------------------------------

	private:
		void set_scissor_rect(const RECT* rc);

		typedef std::deque<RECT> scissor_rect_deque;
		scissor_rect_deque	m_scissor_rects;
	};

} // namespace nexus

#endif // _NEXUS_D3D9_UI_CANVAS_H_