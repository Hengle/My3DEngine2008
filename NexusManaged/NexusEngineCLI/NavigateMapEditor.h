#pragma once
#include "NTerrainActor.h"
#include "NRenderElement.h"
#include "NResource.h"
#include "NTerrainBrush.h"

namespace NexusEngine
{
	using namespace System::ComponentModel;
	
	/**
	* ����ͼ�༭��,��nav_map_editor�İ�װ�����Թ��͵��α༭������һ�𣬲����о��ܻ���
	* �͵��α༭���ֿ����в����ظ����룬�����߼��������ܶ�
	*/
	public ref class NavigateMapEditor
	{
	public:
		NavigateMapEditor(void);
		virtual ~NavigateMapEditor(void);

		// ��ʾ�������ص���ͼ
		property bool NavigationMapVisible
		{
			bool get() { return m_nav_map_editor->get_nav_map_visible(); }
			void set(bool value) 
			{ 
				m_nav_map_editor->set_nav_map_visible(value); 
			}
		}

		// ��ʾ�������ؿ���������λͼ
		property bool WalkableMapVisible
		{
			bool get() { return m_nav_map_editor->get_walkable_map_visibale();; }
			void set(bool value) 
			{ 
				m_nav_map_editor->set_walkable_map_visibale(value); 
			}
		}

		// �߶�ͼ��ɫ
		property Color4f NavigationHeightMapColor
		{
			Color4f get() { return Color4f::FromNative(m_nav_map_editor->get_height_nav_color()); }
			void set(Color4f value) 
			{ m_nav_map_editor->set_height_nav_color(value.ToNative()); }
		}

		// ������������ɫ
		property Color4f NavigationWalkableColor
		{
			Color4f get() { return Color4f::FromNative(m_nav_map_editor->get_walkable_nav_color()); }
			void set(Color4f value) 
			{ 
				m_nav_map_editor->set_walkable_nav_color(value.ToNative());
			}
		}

		property float CullDistance
		{
			float get() { return m_nav_map_editor->get_cull_dist(); }
			void set(float value) { m_nav_map_editor->set_cull_dist(value); }
		}

		// ��ʼ����ͼ�༭������nav_map_actor
		void BeginNavigateMapEditor(NLevel^ level);
		// ��ʼ����ͼ�༭������nav_map_actor
		void EndNavigateMapEditor();

		//!	�Ƿ����һ����Ч��nav_map_actor����
		bool Empty();

		//! �ر�, ִ��������
		void Close();

		//! �趨Brush����
		void SetBrush(NTerrainBrush brush);

		//! ���������ָ����ͼ���ļ���Ϊ�߶�ͼ
		void ImportHeightmap(System::String^ imgFileName);

		//-- ����ͼ�ӿ�
		void MoveNavigationMap(Vector3 loc, Vector3 scale);
		void ResizeNavigationMap(UInt32 width, UInt32 heigth);
		void ResetNavigationMap(UInt32 width, UInt32 heigth);
		// �������ɵ���ͼ
		void RebuildNavigationMap(float maxHeight, float stepHegiht, bool use_line_check);

		/** 
		*	����Windows���ڲ�����Ϣ
		*	@return �����Ƿ����������Ϣ
		*/
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam);

		//! ���Ʊ�ˢ���Ƚ���Ԫ��
		void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);

	private:
		// nativeʵ��
		nav_map_editor* m_nav_map_editor;
	};
}
