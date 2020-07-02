/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NCamera.h"
#include "RenderDefine.h"
#include "NRenderElement.h"

namespace NexusEngine
{
	//! view port
	public ref class NViewport
	{
	public:
		NViewport(void);
		~NViewport(void);

		//! ���X����
		property UInt32 X
		{
			UInt32 get()	{	return m_obj->x;}
			void set(UInt32 val)	{	m_obj->x = val;}
		}

		//! ���Y����
		property UInt32 Y
		{
			UInt32 get()	{	return m_obj->y;}
			void set(UInt32 val)	{	m_obj->y = val;}
		}

		//!	�ӿڿ��
		property UInt32 Width
		{
			UInt32 get()	{	return m_obj->width;}
			void set(UInt32 val)	{	m_obj->width = val;}
		}

		//!	�ӿڸ߶�
		property UInt32 Height
		{
			UInt32 get()	{	return m_obj->height;}
			void set(UInt32 val)	{	m_obj->height = val;}
		}

		//!	z��Χ��Сֵ
		property float MinZ
		{
			float get()	{	return m_obj->min_z;}
			void set(float val)	{	m_obj->min_z = val;}
		}

		//!	z��Χ���ֵ
		property float MaxZ
		{
			float get()	{	return m_obj->max_z;}
			void set(float val)	{	m_obj->max_z = val;}
		}

		//!	���������
		property NCamera^ Camera
		{
			NCamera^ get()	{	return m_camera;}
		}

		//!	��Ⱦģʽ
		property ERenderMode RenderMode
		{
			ERenderMode get()	{	return static_cast<NexusEngine::ERenderMode>(m_obj->render_mode);}
			void set(ERenderMode val)	{	m_obj->render_mode = static_cast<nexus::ERenderMode>(val); }
		}

		//!	windows���ھ��
		property IntPtr HandleWnd
		{
			void set(IntPtr hwnd)	{	m_obj->handle_wnd = hwnd.ToPointer();}
		}

		//! PDI��ͼ����
		property NRenderElement^ WidgetsRender
		{
			NRenderElement^ get()	{	return m_widgetsRender;}
			void set(NRenderElement^ val)
			{
				m_widgetsRender = val;
				NativeRenderElementWrapper::ptr newWidgets( new NativeRenderElementWrapper(val) );
				m_obj->widgets_render = newWidgets;
			}
		}

		//!	LODֵ
		property float LODBias
		{
			float get()	{	return m_obj->lod_bias;}
			void set(float val)	{	m_obj->lod_bias=val;}
		}

		property bool EnableDynamicShadow
		{
			bool get() {	return m_obj->enable_dynamic_shadow; }
			void set(bool val)	{	m_obj->enable_dynamic_shadow=val;}
		}

		// todo : show_flags

		//!	������׶����ڲ�����״̬
		void Update();

		void CreateHitHash();

		void Destroy();
		

		nviewport* GetNaitvePtr()	{	return m_obj;}
	private:
		NCamera^	m_camera;
		nviewport*	m_obj;
		NRenderElement^	m_widgetsRender;
	};
}//namespace NexusEngine