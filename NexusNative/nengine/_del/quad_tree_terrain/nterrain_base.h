#ifndef _NEXUS_TERRAIN_BASE_H_
#define _NEXUS_TERRAIN_BASE_H_
#include "../../../ncore/math/rect.h"
#include "../actor/nprimitive_component.h"
#include "nheight_map.h"
#include "nterrain_mtl.h"

namespace nexus
{
	struct terrain_cfg
	{
		vector3		scale;
		bool		enable_lod;
	};

	/**
	 *	����һ����Σ��������ݺͲ���
	 *	@remarks ����߶�ͼ��render mesh, material���ݣ�
	 *		ʹ��quad tree����clip map�㷨ʵ��lod mesh�Լ���׶���ã�
	*/
	class nAPI nterrain_base
		: public nprimitive_component
	{
		enum EConst
		{
			MAX_ALPHA_LAYER = 4,
		};
	public:
		typedef shared_ptr<nterrain_base> ptr;

		nterrain_base(const nstring& name_str);
		virtual ~nterrain_base(void);

		virtual void create(size_t w, size_t h, short init_h, const terrain_cfg& cfg);
		virtual void destroy();

		/** �ڸ߶�ͼ��ָ�������ڲ���perlin noise�������
		*/
		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);

		//--nprimitive_component interface
		virtual nmaterial* get_material(int lod, int mtl_id);		

		virtual transparent_flag get_transparent_flag() const
		{
			return m_trans_flag;
		}

	protected:
		virtual void post_height_changed(const nrect& rc) = 0;
	protected:
		typedef short htype;
		typedef unsigned char alpha_type;

		nheight_map<htype>			m_height_map;
		nheight_map<alpha_type>		m_layer_map[MAX_ALPHA_LAYER];
		nresource_ptr<nterrain_mtl>	m_material;
		transparent_flag			m_trans_flag;
		terrain_cfg					m_cfg;

		nDECLARE_VIRTUAL_CLASS(nterrain_base)
	};
}//namespace nexus

#endif // _NEXUS_TERRAIN_BASE_H_