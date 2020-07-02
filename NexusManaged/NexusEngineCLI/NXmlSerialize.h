#pragma once
#include "NFileSystem.h"

namespace NexusEngine
{
	using namespace NexusEngineExtension;

	public ref class NXmlSerializeBase
	{
	public:
		explicit NXmlSerializeBase(narchive::ptr& nativeArchive);
		NXmlSerializeBase(void):m_smart_ptr(NULL)
		{}
		~NXmlSerializeBase();		

		void Close();

		virtual void ObjectBegin(String^ objectName);

		// �ṩ�����ı����ӿ�
		virtual void ArrayBegin(String^ arrayName, [Out] int% arraySize);
		virtual void ArrayNext(); 
		virtual void ArrayEnd();

		// ֻ�ṩ��managed code string�����л�����
		virtual void Serial(String^ key,[Out] String^% value);

		virtual void ObjectEnd();

	protected:
		property narchive* NativePtr
		{
			narchive* get();
		}

		property narchive::ptr* SharedPtr
		{
			narchive::ptr* get();
		}

	protected:
		narchive::ptr*	m_smart_ptr;
	};

	/**
	* ��Native narchive_rapidxml_reader��װ
	* �ṩ��Managed code���л��ӿ�
	*/
	public ref class NXmlSerializeReader 
		: public NXmlSerializeBase
	{
	public:
		NXmlSerializeReader(System::String^ pkgName, System::String^ fileName);

		virtual String^ ObjectDescription(String^ key);
	};

	/**
	* ��Native narchive_rapidxml_writer��װ
	* �ṩ��Managed code���л��ӿ�
	*/
	public ref class NXmlSerializeWriter 
		: public NXmlSerializeBase
	{
	public:
		NXmlSerializeWriter(System::String^ pkgName, System::String^ fileName);

		virtual void ObjectDescription(String^ key, String^ value);
	};
}
