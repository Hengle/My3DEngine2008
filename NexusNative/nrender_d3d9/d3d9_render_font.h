#ifndef _NEXUS_D3D9_RENDER_FONT_H_
#define _NEXUS_D3D9_RENDER_FONT_H_
#include "d3d_ptr.h"
#include <vector>

namespace nexus
{

	using namespace std;

#define max_measure_ex_length 1024

	/**
	* ��ID3DXFont�İ�װ
	* ʵ����Ⱦ2D����
	*/
	class d3d9_render_font :
		public nrender_font
	{
	public:
		d3d9_render_font(void);
		virtual ~d3d9_render_font(void);

		// ��������
		virtual void create(const TCHAR* face_name, int width, int height, int weight = 0, bool italic = false);

		// ��������
		virtual int draw_text(const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format);
		// ��������,������϶���
		virtual int draw_text(const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color); 

		// ��������
		virtual int draw_text(const TCHAR* text, const frect& target_area, float z ,float scale ,unsigned int color, unsigned int shadow_color, unsigned int format);
		virtual int draw_text(const TCHAR* text, const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format);

		virtual void get_text_size( const TCHAR* text, nsize& out_size );
		virtual int measure_string( const TCHAR* text, unsigned int format, frect& rectangle );

		//�����ַ�����������Ҫ�������ַ���(���֧��max_measure_ex_length���ַ�)�����������ַ�֮��ľ��롢������ʾ����λ��
		virtual bool measure_string_ex( const nstring& text,vector<int>& dx,vector<int>& caret_pos , frect& rectangle);

		ID3DXFont* get_font() const
		{
			return m_font.get();
		}

	private:
		void create_ascii_size_table();

		int _draw_text(const TCHAR* text, 
			const frect& target_area,
			const D3DXMATRIXA16& mat,
			unsigned int color,
			unsigned int shadow_color,
			unsigned int format);

		d3d_font_ptr		m_font;
		HDC					m_hdc;
		nsize				m_ascii_size[127];
	};

} // namespace nexus

#endif // _NEXUS_D3D9_RENDER_FONT_H_