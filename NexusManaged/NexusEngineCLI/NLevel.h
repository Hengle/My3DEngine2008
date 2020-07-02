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
#include "NResource.h"

namespace NexusEngine
{
	//!	���߼��Ľ��
	public value struct NCheckResult
	{
		Vector3	location;
		Vector3	normal;
		float	dist;
	};

	[System::Flags]
	public enum class LineCheckType  : System::Int32
	{
		Static		= ELCT_Static,	
		Dynamic		= ELCT_Dynamic,

		Terrain		= ELCT_Terrain,
		Navigation	= ELCT_Navigation,

		World		= ELCT_World,

		All			= ELCT_All
	};

	public ref class RenderSetting
	{
	public:
		RenderSetting(render_setting* ns)
		{
			m_NativePtr=ns;
		}

		property Color4f SkyLightHightColor
		{
			Color4f get()
			{
				return Color4f::FromNative(m_NativePtr->skylight_high_color);
			}
			void	set(Color4f value)
			{
				m_NativePtr->skylight_high_color=value.ToNative();
			}
		}

		property Color4f SkyLightLowColor
		{
			Color4f get()
			{
				return Color4f::FromNative(m_NativePtr->skylight_low_color);
			}
			void	set(Color4f value)
			{
				m_NativePtr->skylight_low_color=value.ToNative();
			}
		}

		property float SSAOOcclustionRadius
		{
			float get()
			{
				return m_NativePtr->ssao_occlustion_radius;
			}
			void	set(float value)
			{
				m_NativePtr->ssao_occlustion_radius=value;
			}
		}

		property float SSAOOcclustionPower
		{
			float get()
			{
				return m_NativePtr->ssao_occlustion_power;
			}
			void	set(float value)
			{
				m_NativePtr->ssao_occlustion_power=value;
			}
		}

		property float SSAOBlockerPower
		{
			float get()
			{
				return m_NativePtr->ssao_blocker_power;
			}
			void	set(float value)
			{
				m_NativePtr->ssao_blocker_power=value;
			}
		}

	protected:
		render_setting*	m_NativePtr;		
	};

	//!	��װnexus::nlevel����
	public ref class NLevel : public NativeObjectOwnedBySharedPtr
	{
	public:				
		//!	��ʼ��, �����ڲ�����
		virtual void Init();

		//!	����һ��Actor, Actor NameҪ���ڹؿ�����Ψһ��
		virtual NActor^ CreateActor(System::String^ actorName, System::String^ nativeClassName);

		//!	����ָ�����Ƶ�Actor
		virtual void DestroyActor(System::String^ actorName);

		//!	�������Ʋ���Actor
		NActor^ FindActor(System::String^ actorName);

		//! ��¡ָ����Actor
		NActor^ CloneActor(NActor^ actor);

		//!	���߼��
		bool LineCheck([Out] NCheckResult% ret, Vector3 start, Vector3 end, LineCheckType checkType);
		
		//! ʹ�õ���ͼ�ļ��ref navigation_map::line_of_sight
		bool LineOfSight([Out] NCheckResult% ret, Vector3 start, Vector3 end, bool checkWalkableMap);

		//!	��չؿ��е�����Actor
		virtual void DestroyAllActors();
		
		//!	ȡ�ùؿ�������
		property System::String^ Name
		{
			System::String^ get();
		}

		NActor^ FirstActor();
		NActor^ NextActor();

		void RenameActor(String^ oldName, String^ newName);

		//! ��һ���Ѿ������õ�NActor��ӵ��ؿ���
		void AttachActor(NActor^ actor);
		//! ��һ���Ѿ������õ�NActor�ӹؿ����Ƴ������ǲ���ɾ����NActor
		bool DeattachActor(NActor^ actor);

		//! �ֲ�༭��ؽӿ�
		bool	IsLayerExist(String^ layerName);
		int		GetLayerCount();
		String^ GetLayerName(int index);
		void	CreateNewLayer(String^ layerName);
		void	RemoveLayer(String^ layerName);
		//��������
		void	RenameLayer(String^ oldlayername,String^ newLayerName);
		//���õ�ǰ������Ĳ�
		void	SetActiveLayer(String^ layerName);
		//��ȡ��ǰ������Ĳ������
		String^ GetActiveLayer();
		//�������߽���ָ�����ֵĲ�
		void	LockLayer(String^ layerName,bool lock);
		//ָ�����ֵĲ��Ƿ�����
		bool	IsLayerLocked(String^ layer_name);
		//��ʾ��������ָ���Ĳ�
		void	ShowLayer(String^ layerName,bool show);
		//�Ƿ���ʾָ����
		bool	IsLayerVisible(String^ layerName);
		//��ָ�����ֵĶ����ƶ�����ǰ����Ĳ�
		void	MoveActorToActiveLayer(String^ actorName);

		// ����ͼ������������ؽӿ�
		bool GetWorldWalkable(float wx, float wz);
		bool GetTileWalkable(int tx, int tz);
		// ����ͼ�߶�ͼ�ӿ�
		float GetWorldHeight(float wx, float wz);
		float GetTileHeight(int tx, int tz);

		//-- ����Level����Native function export_level��һ����װ
		virtual int ExportLevel(NResourceLoc^ location);

		//��ȡ��ǰLevel��RenderSetting����
		virtual RenderSetting^	GetRenderSetting()
		{
			return m_RenderSetting;
		}

		//�����ؽӿ�
		/*void	SetSky(NResourceLoc loc)
		{
			resource_location nloc;
			loc.ToNative(nloc);
			NativePtr->set_sky(nloc);
		}
		void	RemoveSky()
		{
			NativePtr->remove_sky();
		}
		NResourceLoc	GetSky()
		{
			return NResourceLoc(NativePtr->get_sky());
		}*/
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

	public:
		// ����ͼ��ؽӿ�
		//-- ����ͼ���
		property UInt32 NavigationMapWidth
		{
			UInt32 get()
			{
				return NativePtr->get_navigation_map()->get_raw_width();
			}
		}

		//-- ����ͼ�߶�
		property UInt32 NavigationMapHeight
		{
			UInt32 get()
			{
				return NativePtr->get_navigation_map()->get_raw_height();
			}
		}

		//-- ����ͼ��ʼλ��
		property Vector3 NavigationMapLocation
		{
			Vector3 get()
			{
				const nexus::vector3& nloc = NativePtr->get_navigation_map()->get_location();
				return Vector3(nloc.x, nloc.y, nloc.z);
			}
		}

		//-- ����ͼ����
		property Vector3 NavigationMapScale
		{
			Vector3 get()
			{
				const nexus::vector3& scale = NativePtr->get_navigation_map()->get_scale();
				return Vector3(scale.x, scale.y, scale.z);
			}
		}

	private:
		typedef System::Collections::Generic::Dictionary<System::String^, NActor^> ActorDict;
		
		ActorDict::Enumerator^	m_actorEnum;
		ActorDict^				m_actorDict;

		RenderSetting^			m_RenderSetting;
	};
}//namespace NexusEngine