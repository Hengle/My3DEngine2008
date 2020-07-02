/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NLevel.h"
#include "RenderDefine.h"
#include "NResource.h"

namespace NexusEngine
{
	ref class NFileSystem;
	ref class NRenderElement;	
	ref class NRenderResourceManager;

	//!	��������
	public ref struct NEngineConfig
	{
		//-- ��Ⱦ����
		System::IntPtr	RenderWndHandle;
		System::Int32	ClientWidth;
		System::Int32	ClientHeight;
		System::Byte	ColorBits;
		bool			FullScreen;
		bool			EnableHDR;
		bool			EnableSSAO;

		//-- Native Engine����
		System::String^	RenderClass;
		System::String^ FileSystemClass;
		System::String^ FileSystemRoot;
		System::String^ EngineDataPkg;
		System::String^ ResourceCacheClass;
		System::String^ ResourceIOClass;

		void ToNative(engine_config& cfg);
	};
	
	/**
	 *	�ṩ��Native Class ��nengine���İ�װ
	*/
	public ref class NEngine : public NativeObjectOwnedRaw
	{
	public:
		NEngine(void);		

		static NEngine^ Instance();

		//-- ��������--�رսӿ� -------------------------------------------------------
		//! ����Ҫ��ʼ������ģ��
		virtual void InitCore();

		//!	Ȼ�󴴽�����ģ��
		virtual void InitModules(NEngineConfig^ cfg);

		//! �ر�����,ִ����������
		virtual void Close();

		//! ��ʼ��Ϸ
		virtual void BeginPlay();

		//-- �ؿ�����ӿ� -------------------------------------------------------------
		//!	����һ���µĿհ׹ؿ�,�ؿ�������Ҫ����Ψһ��
		virtual NLevel^ CreateLevel(System::String^ lvName, System::String^ nativeClassName);

		//! ����ĳ��Level, �൱��Dispose, ��ʹ�ⲿ�Գ���NLevel��������, ������Ҳ�Ѿ�������;
		virtual void DestroyLevel(NLevel^ lv);

		//!	����ָ�����Ƶ�Level
		virtual void DestroyLevel(System::String^ lvName);

		//!	��ָ�����ļ�ϵͳĿ¼����ؿ�
		virtual NLevel^ LoadLevel(NResourceLoc^ loc);

		//!	��ָ�����Ƶ�Level�����ļ�ϵͳ��ĳ��Ŀ¼
		virtual void SaveLevel(System::String^ lvName, NResourceLoc^ loc);

		//!	��ָ�����Ƶ�Level������ĳ��Ŀ¼
		virtual void ExportLevel(System::String^ lvName, NResourceLoc^ loc);

		property NFileSystem^ FileSystem
		{
			NFileSystem^ get()	{	return m_fileSys;}
		}

		property NRenderResourceManager^ RenderResManager
		{
			NRenderResourceManager^ get() { return m_renderResourceManager; }
		}
		
		property NEngineConfig^ Config
		{
			NEngineConfig^ get()	{	return m_config;}			
		}
	private:
		static NEngine^ s_instance;
		NFileSystem^	m_fileSys;
		NEngineConfig^	m_config;
		NRenderResourceManager^ m_renderResourceManager;

	protected:
		System::Collections::Generic::Dictionary<System::String^, NLevel^>^	m_levelDict;

		//-- Native Wrapper
	protected:
		NEngine(System::String^ nativeClassName);
		property nengine* NativePtr
		{
			nengine* get();
		}
	};
}//namespace NexusEngine