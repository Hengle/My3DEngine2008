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

namespace NexusEngine
{
	public enum class ETrnLerpMode : System::Int32
	{
		None	= ELM_None,
		Linear	= ELM_Linear,
		Cosine	= ELM_Cosine,
		Quadratic = ELM_Quadratic,
	};

	public enum class ETrnBrushShape : System::Int32
	{
		Circle = EBS_Circle,
		Square = EBS_Square,
	};

	public enum class ETrnBurshTarget : System::Int32
	{
		Heightmap = EBT_Heightmap,
		Alphamap = EBT_Alphamap,
		Decomap = EBT_Decomap
	};

	public enum class ETrnBrushOP : System::Int32
	{
		Paint	= EOP_Paint,		
		Flatten	= EOP_Flatten,
		Smooth	= EOP_Smooth,
		Apply	= EOP_Apply,
		Noise	= EOP_Noise,
	};

	public value struct NTerrainBrush
	{
		ETrnBrushOP		op;
		ETrnBrushShape	shape;
		ETrnLerpMode	lerpMode;
		int				innerRadius;
		int				outerRadius;
		int				strength;
		int				noiseFreq;
		ETrnBurshTarget	target;
		int				targetIndex;

		void ToNative(nexus::nterrain_brush& brush);
	};

	//! ���α༭��, �ڲ�ʹ��Native����ʵ��
	public ref class NTerrainEditor
	{
	public:
		literal UInt32 MaxAphaLayers = 4;
		literal UInt32 MaxLayers = 5;

		System::EventHandler^ TerranActorBinded;

		NTerrainEditor(void);
		~NTerrainEditor(void);

		//!	ָ����ǰ������Terrain����
		void BindTerrain(NTerrainActor^ trn);

		//!	�Ƿ����һ����Ч��Terrain����
		bool Empty();

		//! �ر�, ִ��������
		void Close();

		//! �趨Brush����
		void SetBrush(NTerrainBrush brush);

		//!	�ڸ߶�ͼָ���������ڲ���������θ߶�
		void GenerateNoise(System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		//! ���������ָ����ͼ���ļ���Ϊ�߶�ͼ
		void ImportHeightmap(System::String^ imgFileName);

		//!	�����������͵Ĳ���
		void CreateMaterialBasic(NResourceLoc^ textureLoc);

		//!	������ͼ���ϵĲ���
		void CreateTextureSplatting(int alphaW, int alphaH);

		//!	�趨����ͼ��
		void SplatSetLayer(UInt32 layerIndex, NResourceLoc^ textureLoc, Vector2 uvScale, float uvRotate);

		//!	��ĳ��ͼ���ϲ��������Alpha
		void SplatLayerNoise(UInt32 layerIndex, System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		//!	ȡ��ĳ��ͼ�������
		void SplatGetLayer(UInt32 layerIndex, NResourceLoc% outTex, Vector2% outScale, float% outRotate);		

		/** 
		 *	����Windows���ڲ�����Ϣ
		 *	@return �����Ƿ����������Ϣ
		*/
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam);

		//! ���Ƶ��ι��Ƚ���Ԫ��
		void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);
	private:
		nterrain_editor*	m_nativeEditor;
	};
}//namespace NexusEngine