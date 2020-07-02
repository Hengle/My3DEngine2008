/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NActor.h"
#include "NMath.h"

namespace NexusEngine
{
	//!	���߼��Ľ��
	public value struct NCheckResult
	{
		Vector3	location;
		Vector3	normal;
		float	dist;
	};

	//!	��װnexus::nlevel����
	public ref class NLevel : public NativeObjectOwnedBySharedPtr
	{
	public:				
		//!	��ʼ��, �����ڲ�����
		virtual void Init(System::String^ sceneClassName);

		//!	����һ��Actor, Actor NameҪ���ڹؿ�����Ψһ��
		virtual NActor^ CreateActor(System::String^ actorName, System::String^ nativeClassName);

		//!	����ָ�����Ƶ�Actor
		virtual void DestroyActor(System::String^ actorName);

		//!	�������Ʋ���Actor
		NActor^ FindActor(System::String^ actorName);

		//!	���߼��
		bool LineCheck([Out] NCheckResult% ret, Vector3 start, Vector3 end);

		//!	��չؿ��е�����Actor
		void DestroyAllActors();
		
		//!	ȡ�ùؿ�������
		property System::String^ Name
		{
			System::String^ get();
		}

		NActor^ FirstActor();
		NActor^ NextActor();

		void RenameActor(String^ oldName, String^ newName);

	public:
		//-- ��װ��ؽӿ� --
		NLevel(nexus::nlevel::ptr nativeLv);
		static NLevel^ FromNativePtr(nexus::nlevel::ptr nativeLv, System::String^ nativeClassName);
		nexus::nlevel::ptr GetNativeSmartPtr();

		void NativeLevelLoaded();
	
		property nexus::nlevel* NativePtr
		{
			nexus::nlevel* get();
		}

	private:
		typedef System::Collections::Generic::Dictionary<System::String^, NActor^> ActorDict;
		
		ActorDict::Enumerator^	m_actorEnum;
		ActorDict^				m_actorDict;
	};
}//namespace NexusEngine