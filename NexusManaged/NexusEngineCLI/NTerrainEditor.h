/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NTerrainActor.h"
#include "NRenderElement.h"
#include "NResource.h"
#include "NTerrainBrush.h"
#include "NTerrainMtlSetup.h"

namespace NexusEngine
{
	using namespace System::ComponentModel;

	//! ���α༭��, �ڲ�ʹ��Native����ʵ��
	public ref class NTerrainEditor
	{
	public:
		literal UInt32 MaxAphaLayers = 4;
		literal UInt32 MaxLayers = 5;

		System::EventHandler^ TerranActorBinded;

		NTerrainEditor(void);
		~NTerrainEditor(void);

		NTerrainMtlSetup^ GetMaterial();

		//!	ָ����ǰ������Terrain����
		void BindTerrain(NTerrainActor^ trn);

		//!	�Ƿ����һ����Ч��Terrain����
		bool Empty();

		void ShowChunkEdge(bool s);

		//! �ر�, ִ��������
		void Close();

		//! �趨Brush����
		void SetBrush(NTerrainBrush brush);

		//!	�ڸ߶�ͼָ���������ڲ���������θ߶�
		void GenerateNoise(System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		//! ���������ָ����ͼ���ļ���Ϊ�߶�ͼ
		void ImportHeightmap(System::String^ imgFileName);

		/** 
		 *	����Windows���ڲ�����Ϣ
		 *	@return �����Ƿ����������Ϣ
		*/
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam);

		//! ���Ƶ��ι��Ƚ���Ԫ��
		void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);

		NTerrainActor^ GetTerrainBinded()	{	return m_trn;}
	private:
		nterrain_editor*	m_nativeEditor;
		NTerrainActor^		m_trn;
	};
}//namespace NexusEngine