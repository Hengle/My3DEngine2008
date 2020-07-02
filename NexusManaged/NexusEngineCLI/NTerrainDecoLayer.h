#pragma once
#include "NativeObject.h"
#include "NResource.h"
#include "math/Vector3.h"
namespace NexusEngine
{
	ref class NTerrainDecoSetup;

	public ref class NTerrainDecoLayer : public NativeObjectOwnedBySharedPtr
	{
	public:
		[Category("����"), Description("װ�β�����")]
		property String^ Name
		{
			String^ get()
			{
				return gcnew String(NativePtr->get_name().c_str());
			}
			void set(String^ name);
		}

		[Category("����"), Description("װ�β����õ���Դ���Բ�Ҷװ�β���˵�������ǲ�����Դ���Ծ�̬ģ��װ�β���˵�������Ǿ�̬ģ����Դ")]
		property NResourceLoc ResourceLoc
		{
			NResourceLoc	get()	
			{
				return NResourceLoc(NativePtr->get_resource_loc());
			}
			void	set(NResourceLoc loc)
			{
				resource_location _loc;
				loc.ToNative(_loc);
				NativePtr->set_resource_loc(_loc);
			}
		}

		[Category("����"), Description("װ�β���ÿһ����Ʒ���������ƫ��")]
		property float RandomOffset
		{
			float	get()	
			{
				return NativePtr->get_random_offset();
			}
			void	set(float v)
			{
				NativePtr->set_random_offset(v);
			}
		}

		[Category("����"), Description("װ�β���ÿһ����Ʒ���ڸ߶��ϵ�ƫ�ƣ���ֵ��ʾ����")]
		property float HeightOffset
		{
			float	get()	
			{
				return NativePtr->get_height_offset();
			}
			void	set(float v)
			{
				NativePtr->set_height_offset(v);
			}
		}

		[Category("����"), Description("װ�β���ÿһ����Ʒ�������ת��ȡֵ��Χ0~180�ȣ���ʾ��-180��~180��֮��ȡ���ֵ")]
		property float RandomRotation
		{
			float	get()	
			{
				return NativePtr->get_random_rotation();
			}
			void	set(float v)
			{
				NativePtr->set_random_rotation(v);
			}
		}

		[Category("����"), Description("װ�β�ÿһ����Ʒ���������ֵ����Ʒʵ�ʵ�����ֵ�����������ֵ����С����ֵ�����ѡ��")]
		property float MaxRandomScale
		{
			float	get()	
			{
				return NativePtr->get_max_random_scale();
			}
			void	set(float v)
			{
				NativePtr->set_max_random_scale(v);
			}
		}

		[Category("����"), Description("װ�β�ÿһ����Ʒ���������ֵ����Ʒʵ�ʵ�����ֵ�����������ֵ����С����ֵ�����ѡ��")]
		property float MinRandomScale
		{
			float	get()	
			{
				return NativePtr->get_min_random_scale();
			}
			void	set(float v)
			{
				NativePtr->set_min_random_scale(v);
			}
		}

		[Category("����"), Description("װ�β���ÿһ�����������������Ʒ������")]
		property float Density
		{
			float	get()	
			{
				return NativePtr->get_density();
			}
			void	set(float v)
			{
				NativePtr->set_density(v);
			}
		}

		[Category("����"), Description("װ�β������������")]
		property int GridCount
		{
			int	get()	
			{
				return NativePtr->get_grid_size();
			}
		}


		[Category("����"), Description("���������")]
		property int RandomSeed
		{
			int	get()	
			{
				return NativePtr->get_random_seed();
			}
			void	set(int v)
			{
				NativePtr->set_random_seed(v);
			}
		}

	public:
		NTerrainDecoLayer(nexus::nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner);
	protected:
		NTerrainDecoSetup^	m_Owner;

		property nterrain_deco_layer* NativePtr
		{
			nterrain_deco_layer* get()
			{
				return dynamic_cast<nterrain_deco_layer*>(RawPtr);
			}
		}
	};

	public ref class NTerrainStaticMeshDecoLayer : public NTerrainDecoLayer
	{
	public:
		NTerrainStaticMeshDecoLayer(nexus::nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner);
	};

	public ref class NTerrainGrassDecoLayer : public NTerrainDecoLayer
	{
	public:
		NTerrainGrassDecoLayer(nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner);

		[Category("����"), Description("ÿһƬ��Ҷ�Ŀ��")]
		property float Width
		{
			float	get()	
			{
				return NativePtr->get_width();
			}
			void	set(float v)
			{
				NativePtr->set_width(v);
			}
		}

		[Category("����"), Description("ÿһƬ��Ҷ�ĸ߶�")]
		property float Height
		{
			float	get()	
			{
				return NativePtr->get_height();
			}
			void	set(float v)
			{
				NativePtr->set_height(v);
			}
		}

		[Category("����"), Description("�Ƿ�ʹ��ʮ���β�Ҷ��Ƭ")]
		property bool CrossShape
		{
			bool	get()	
			{
				return NativePtr->get_cross();
			}
			void	set(bool v)
			{
				NativePtr->set_cross(v);
			}
		}
	protected:
		property nterrain_grass_deco_layer* NativePtr
		{
			nterrain_grass_deco_layer* get()
			{
				return dynamic_cast<nterrain_grass_deco_layer*>(RawPtr);
			}
		}
	};
};