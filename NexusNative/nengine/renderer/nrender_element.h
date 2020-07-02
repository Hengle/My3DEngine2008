#ifndef _NEXUS_RENDER_ELEMENT_H_
#define _NEXUS_RENDER_ELEMENT_H_
#include "ncore.h"
#include "nrender_proxy.h"

namespace nexus
{
	class nrender_simple_mesh;
	/**
	 *	�ṩ��render element��canvas�Ļ�ͼ�ӿ�
	 *	@remarks ����Ⱦ��ʵ��
	*/
	class nAPI nrender_primitive_draw_interface
		: public nobject
	{
	public:
		nrender_primitive_draw_interface(void)		{		}
		virtual ~nrender_primitive_draw_interface(void)	{	}

		//-- ����ʹ��begin����batch�����ֱ�ӵ���draw_line��ֱ�ӻ���
		virtual void begin_line_batch() = 0;
		virtual void draw_line(const vector3& start, const vector3& end, const color4f& color) = 0;
		virtual void draw_wired_box(const AABBox& box, const color4f& color) = 0;
		virtual void end_line_batch() = 0;

		virtual void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color) = 0;

		virtual void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color) = 0;
		virtual void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color) = 0;

		virtual void draw_primitive(const nrender_proxy* obj, bool hit_proxy, bool disable_ztest) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_primitive_draw_interface);
	};

	/**
	 *	����ֱ����back buff�ϻ��ƵĶ���
	 *	@remark ��ʹ��������������draw()�����е���PDI��ִ�л�ͼ
	*/
	class nAPI nrender_element
		: public nobject
	{
	public:
		typedef shared_ptr<nrender_element> ptr;

		nrender_element(void)		{		}
		virtual ~nrender_element(void)	{	}

		virtual void draw(nrender_primitive_draw_interface* PDI) = 0;
		virtual void update_hit_proxy(nrender_primitive_draw_interface* PDI)
		{
			(void)PDI;
		}

		nDECLARE_VIRTUAL_CLASS(nrender_element);
	};
}//namespace nexus

#endif //_NEXUS_RENDER_ELEMENT_H_