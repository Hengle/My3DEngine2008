/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NFileQuery.h"

namespace NexusEngine
{
	/*
	 *	�ļ�ϵͳ���򿪵�һ���ļ�ʵ��, ��װnexus::nfile
	*/
	public ref class NFile : public NativeObjectOwnedBySharedPtr
	{
		//-- Native wrapper
	public:
		explicit NFile(nexus::nfile::ptr nativeObj);
		property nexus::nfile* NativePtr
		{	nexus::nfile* get(); }
	};

	/**
	 *	��װNative class ��nexus::nfile_system��
	*/
	public ref class NFileSystem
	{
	public:
		//!	�ļ�ϵͳ���ж��ٸ��ļ���
		int GetNumPackage();

		//!	��ѯĳ���ļ���������
		System::String^ GetPackageName(int i);

		//��ѯָ���ļ��Ƿ����
		bool IsFileExist(System::String^ pkgName, System::String^ path);
		//!	��ѯĳ���ļ����е�ĳ��·���µ������ļ����ļ���
		void QueryPackage(System::String^ pkgName, System::String^ path, NFileQuery^ fq);
		//��ָ�������洴��Ŀ¼
		void CreateDirectory(System::String^ pkgName, System::String^ path);
		//ɾ��ָ��Ŀ¼
		void DeleteDirectory(System::String^ pkgName, System::String^ path);
		//������ָ��Ŀ¼
		bool RenameDirectory(System::String^ pkgName, System::String^ oldName,System::String^ newName);

		System::UInt32 GetFileSize(System::String^ pkgName, System::String^ path);
		System::DateTime GetFileTime(System::String^ pkgName, System::String^ path);

		System::String^ GetFullPath(System::String^ pkgName, System::String^ fileName);

		static property System::String^ DefaultFileSystemRoot
		{
			System::String^ get()
			{
				return gcnew System::String(nfile_sys_default_root().c_str());
			}
		}

		//-- Native wrapper
	public:
		/** �ڲ�����һ��Native nfile_system�������������������*/
		explicit NFileSystem(System::String^ nativeClassName);
		/** ���ⲿ����Native nfile_system�����ⲿ���������ڹ���ʽ*/
		explicit NFileSystem(INativeObj^ nativeObj);

		property nfile_system* NativePtr
		{
			nfile_system* get();
		}
	private:
		INativeObj^	m_nativeObj;
	};
}//namespace NexusCore