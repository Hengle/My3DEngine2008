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

		//-- Native Engine����
		System::String^	RenderClass;
		System::String^ FileSystemClass;
		System::String^ FileSystemRoot;
		System::String^ EngineDataPkg;

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

		//-- �ؿ�����ӿ� -------------------------------------------------------------
		//!	����һ���µĿհ׹ؿ�,�ؿ�������Ҫ����Ψһ��
		NLevel^ CreateLevel(System::String^ lvName, System::String^ nativeClassName);

		//! ����ĳ��Level, �൱��Dispose, ��ʹ�ⲿ�Գ���NLevel��������, ������Ҳ�Ѿ�������;
		void DestroyLevel(NLevel^ lv);

		//!	����ָ�����Ƶ�Level
		void DestroyLevel(System::String^ lvName);

		//!	��ָ�����ļ�ϵͳĿ¼����ؿ�
		NLevel^ LoadLevel(NResourceLoc^ loc);

		//!	��ָ�����Ƶ�Level�����ļ�ϵͳ��ĳ��Ŀ¼
		void SaveLevel(System::String^ lvName, NResourceLoc^ loc);

		property NFileSystem^ FileSystem
		{
			NFileSystem^ get()	{	return m_fileSys;}
		}
		
		property NEngineConfig^ Config
		{
			NEngineConfig^ get()	{	return m_config;}			
		}
	private:
		static NEngine^ s_instance;
		NFileSystem^	m_fileSys;
		NEngineConfig^	m_config;

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