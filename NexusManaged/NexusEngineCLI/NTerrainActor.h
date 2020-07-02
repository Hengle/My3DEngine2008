/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NActor.h"
#include "NMath.h"

namespace NexusEngine
{
	/**
	 *	���ζ���, ��װnexus::nterrain_actor
	*/
	public ref class NTerrainActor : public NActor
	{
	public:
		//!	�����߶�ͼ, �Լ��ڲ���Ⱦ�����
		void Create(UInt32 w, UInt32 h, UInt16 initH, UInt32 chunkSize);

		//!	����λ�ú�Scale, ��֧����ת
		void Move(Vector3 pos, Vector3 scale);
		
		//-- Native Wratoer
	public:
		NTerrainActor(nexus::nterrain_actor::ptr nativeTrn);		
		nterrain_actor::ptr GetSmartPtr();		
		
	protected:
		property nterrain_actor* NativePtr
		{
			nterrain_actor* get();
		}		
	};
}//namespace NexusEngine