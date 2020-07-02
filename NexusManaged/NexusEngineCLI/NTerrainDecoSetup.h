#pragma once
#include "NativeObject.h"
#include "NActor.h"
#include "NMath.h"
#include "NResource.h"
#include "NTerrainDecoLayer.h"

namespace NexusEngine
{
	public enum class DecoLayerType
	{
		StaticMeshDecoLayer=nexus::deco_layer_static_mesh,
		GrassDecoLayer=nexus::deco_layer_grass,
	};

	public ref class NTerrainDecoSetup
	{
	public:
		//��ȡָ�����Ƶ�װ�β�
		NTerrainDecoLayer^	GetLayer(String^ name);

		//���һ���µ�װ�β�
		NTerrainDecoLayer^	AddLayer(DecoLayerType type,unsigned int size);

		//ɾ��ָ����װ�β�
		void	DeleteLayer(int index);

		property System::Collections::Generic::List<NTerrainDecoLayer^>^ DecoLayers
		{
			System::Collections::Generic::List<NTerrainDecoLayer^>^ get()	{return m_Layers;}
		}
	private:
		System::Collections::Generic::List<NTerrainDecoLayer^>^	m_Layers;
	public:
		NTerrainDecoSetup(nexus::nterrain_deco_setup* deco);
	protected:
		nterrain_deco_setup*	m_NativePtr;
		property nterrain_deco_setup* NativePtr
		{
			nterrain_deco_setup* get()
			{
				return m_NativePtr;
			}
		}
	};
}