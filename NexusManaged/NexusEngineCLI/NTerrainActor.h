/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NActor.h"
#include "NMath.h"
#include "NTerrainMtlSetup.h"
#include "NTerrainDecoSetup.h"

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

		//!
		void GetWorldHeight(float worldX, float worldZ);
		void GetTileHeight(int tileX, int tileZ);

		//! �߶�ͼ��Ⱥ͸߶�
		property UInt32 HeightMapWidth
		{
			UInt32 get()
			{
				return NativePtr->get_map_width();
			}
		}

		property UInt32 HeightMapHeight
		{
			UInt32 get()
			{
				return NativePtr->get_map_height();
			}
		}

		NTerrainMtlSetup^ GetMaterial()	{	return m_mtlSetup; }

		NTerrainDecoSetup^ GetDeco()	{return m_DecoSetup;}

	private:
		NTerrainMtlSetup^	m_mtlSetup;
		NTerrainDecoSetup^	m_DecoSetup;
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