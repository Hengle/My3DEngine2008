/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	/**
	 * Native�����װ�ӿ�
	 * @remarks ����nobject������İ�װ��ʹ�û������nobjectָ�룬������ʹ��ʱ��castΪ������
	*/
	public ref class INativeObj abstract
	{
	public:
		nexus::nobject* GetRawPtr()
		{
			return RawPtr;
		}
	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() = 0;
		}

		static nexus::nobject* ConstructObject(System::String^ nativeClassName);
	};

	/**
	 *	�ṩһ����Native Object�ġ�Borrowned Reference������İ�װ
	 *	@remarks �������ڲ�����ָ��������ڣ���������ͨ����������Native Object����
	*/
	public ref class NativeObjectBorrowed : public INativeObj
	{
	public:
		explicit NativeObjectBorrowed(nexus::nobject* native_obj);

	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() override;
		}

		property nexus::nobject* NativePtr
		{
			nexus::nobject* get();
		}
	
	protected:
		nexus::nobject*	m_obj;
	};

	/**
	 *	�ṩһ����Native Object�ġ�Owner Reference������İ�װ
	 *	@remarsk ͨ��shared_ptr��Native����Э������������
	*/
	public ref class NativeObjectOwnedBySharedPtr : public INativeObj
	{
	public:
		explicit NativeObjectOwnedBySharedPtr(const boost::shared_ptr<nexus::nobject>& native_obj);		
		NativeObjectOwnedBySharedPtr(void):m_smart_ptr(NULL)
		{}
		~NativeObjectOwnedBySharedPtr();		

	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() override;
		}

		property nexus::nobject* NativePtr
		{
			nexus::nobject* get();
		}

	protected:
		boost::shared_ptr<nexus::nobject>*	m_smart_ptr;
	};

	/**
	 *	ͨ��ԭʼָ�뷽ʽ������ӵ��Native Object�Ķ����װ
	*/
	public ref class NativeObjectOwnedRaw : public INativeObj
	{
	public:
		explicit NativeObjectOwnedRaw(nexus::nobject* obj);
		~NativeObjectOwnedRaw();

	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() override;
		}
		property nexus::nobject* NativePtr
		{
			nexus::nobject* get();
		}

	protected:
		nexus::nobject*	m_obj;
	};
}//namespace NexusCore