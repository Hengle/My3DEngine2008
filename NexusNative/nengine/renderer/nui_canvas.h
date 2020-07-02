#ifndef _NEXUS_UI_CANVAS_H_
#define _NEXUS_UI_CANVAS_H_
#include "..\resource\nresource.h"
#include "..\resource\nresource_texture.h"
#include "nrender_resource.h"
#include "nrender_font.h"

namespace nexus
{
	/**
	*	����һ����ȾUI�õ�render target�����ṩ��ͼ�ӿ�
	*/
	class nAPI nui_canvas
		: public nrender_resource
	{
	public:
		nui_canvas(void)			{	}
		virtual ~nui_canvas(void)	{	}

		virtual void begin(bool depth_enable=false) = 0;

		// ��ȡ��ǰViewport��Ϣ
		virtual vector2 get_viewport_size() = 0;

		// push���򵽲ü�����
		virtual void push_scissor_rect(const frect* rect) = 0;
		// pop����from�ü�����
		virtual void pop_scissor_rect() = 0;
		// ��ȡ��ǰ�ü�����Ĵ�С
		virtual void get_scissor_rect(frect& rect) = 0;

		// ����ͼ��
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,const color_rect& color = color_rect(0xFFFFFFFF) ) = 0;
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,argb_t color = 0xFFFFFFFF ) = 0;
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float scale,argb_t color = 0xFFFFFFFF ) = 0;
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float scale,const vector2& scale_center,argb_t color = 0xFFFFFFFF ) = 0;
		// ʹ��2D�������ͼ�񣬴���ת����Ҫָ����ת����
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float rotation,const vector2& rot_center,argb_t color = 0xFFFFFFFF )=0;
		//ʹ��ָ����alpha��ͼ����ͼ��,�����ƻ��ƾ������ĵ���ת
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, nrender_texture2D* alpha_texture,const frect& alpha_source_area,const frect& target_area,float rotation,argb_t color = 0xFFFFFFFF) = 0;

		// �����ı�
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format) = 0;
		virtual void draw_string(nrender_font* font, const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color) = 0;
		//����Ⱥ����ŵĻ����ı�
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,unsigned int color, unsigned int shadow_color, unsigned int format) = 0;
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format) = 0;
		// ����һ�����ο�
		virtual void draw_rect(const frect& target_area,const color_rect& color) = 0;
		virtual void draw_rect(const frect& target_area,float z,float scale,const color_rect& color) = 0;
		virtual void draw_rect(const frect& target_area,float z,float scale,const vector2& scale_center,const color_rect& color) = 0;

		// ����
		virtual void draw_line( fpoint* points, unsigned int num_points, unsigned int color )=0;
		virtual void draw_line( fpoint* points, unsigned int num_points, float z,float scale,const vector2& scale_center,unsigned int color )=0;

		// ������κ���������cooldown����
		virtual void draw_polygon(fpoint* points,unsigned int num_points, unsigned int color = 0xFFFFFFFF )=0;
		virtual void draw_polygon(fpoint* points,unsigned int num_points, float z,float scale,const vector2& scale_center,unsigned int color = 0xFFFFFFFF )=0;

		virtual void flush() = 0;

		virtual void end() = 0;

		//-------------------------------------------------------------------------------

		nDECLARE_VIRTUAL_CLASS(nui_canvas)
	};
}//namespace nexus
#endif //_NEXUS_UI_CANVAS_H_