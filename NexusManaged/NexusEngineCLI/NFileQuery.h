/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	//!	�ļ����������
	public enum class EFileEntityType : System::Int32
	{
		Invalid		= EFile_Invalid,	// ��Чֵ
		Regular		= EFile_Regular,	// ��ͨ�ļ�
		Folder		= EFile_Folder		// �ļ���
	};

	//!	�ļ�������Ϣ
	public value struct NFileEntity
	{
		EFileEntityType type;		// ����
		System::String^	pkg;		// ���ڵ�package
		System::String^	path;		// ·��
		System::UInt32	fileSize;	// �ļ���С - ������ͨ�ļ�����

		property String^ PackageName
		{
			String^ get()	{	return pkg; }
		}

		property String^ FilePath
		{
			String^ get()	{	return path; }
		}

		property String^ FileName
		{
			String^ get();
		}

		property String^ FileNameNoExtension
		{
			String^ get();
		}

		property String^ FileExtension
		{
			String^ get();
		}

		static NFileEntity^ FromNative(const nfile_entity& file);
	};

	/**
	 *	�ļ���ѯ�ӿ�
	*/
	public ref class NFileQuery abstract
	{
	public:
		NFileQuery(void);

		//! �ļ�ϵͳ�ҵ�ĳ���ļ�����ʱ�Ļص�����
		virtual void OnFileEntity(NFileEntity% file) abstract;
	};

	//-- Native��װ,ʹ��Native������Ե���Managed����
	class NativeFileQueryWrapper : public nexus::nfile_query
	{
		gcroot<NFileQuery^>		m_objCLR;
	public:
		NativeFileQueryWrapper(NFileQuery^ clrObj);
		virtual void on_file_entity(const nfile_entity& file);
	};
}//namespace NexusEngine