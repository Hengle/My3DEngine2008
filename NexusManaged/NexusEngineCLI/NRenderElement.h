/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/

#pragma once
#include "math/Vector3.h"
#include "math/BoundingBox.h"
#include "Color.h"

namespace NexusEngine
{
	/**
	 *	����Ԫ�ػ�ͼ�ӿ�, ��װnexus::nrender_primitive_draw_interface
	*/
	public ref class NRenderPrimitiveDrawInterface sealed
	{
	public:
		//!	��ʼ��������
		void BeginLineBatch();

		//!	����һ��ָ����ɫ���߶�
		void DrawLine(Vector3 start, Vector3 end, Color4f color);
		void DrawWiredBox(BoundingBox box, Color4f color);

		//!	������������
		void EndLineBatch();

		//!	�򵥵����ֻ��ƽӿ�
		void DrawDebugString(int x, int y, System::String^ text, Color4ub color);

		//void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color) = 0;
		//void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color) = 0;

		//-- Native wrapper
	private:
		nexus::nrender_primitive_draw_interface* m_nativePDI;
	public:
		void SetNativePDI(nexus::nrender_primitive_draw_interface* nativePDI);
		property nexus::nrender_primitive_draw_interface* NativePtr
		{
			nexus::nrender_primitive_draw_interface* get();
		}
	};

	/**
	 *	����Ԫ�ػ�ͼ�ص��ӿ�
	*/
	public ref class NRenderElement abstract
	{
	public:
		NRenderElement(void);

		//!	ʹ��PDI���л�ͼ
		virtual void Draw(NRenderPrimitiveDrawInterface^ PDI) abstract;
		virtual void UpdateHitProxy(NRenderPrimitiveDrawInterface^ PDI)
		{
			(void)PDI;
		}
	};

	//!	��װ��, ʹ��native������Ե���NRenderElement���������
	class NativeRenderElementWrapper : public nexus::nrender_element
	{
		gcroot<NRenderElement^>			m_clrObj;
		gcroot<NRenderPrimitiveDrawInterface^>	m_clrPDI;
	public:
		typedef boost::shared_ptr<NativeRenderElementWrapper> Ptr;

		NativeRenderElementWrapper(NRenderElement^ clrObj);

		bool IsObject(NRenderElement^ clrObj)
		{
			return (NRenderElement^)m_clrObj == clrObj;
		}
		
		virtual void draw(nrender_primitive_draw_interface* PDI);
		virtual void update_hit_proxy(nrender_primitive_draw_interface* PDI);
	};
}//namespace NexusEngine