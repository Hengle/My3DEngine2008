#ifndef _NEXUS_CHUNKED_TERRAIN_H_
#define _NEXUS_CHUNKED_TERRAIN_H_
#include "../framework/nactor.h"
#include "../resource/nresource.h"

namespace nexus
{
	class nchunked_terrain_impl;
	/**
	 *	�����ηֳɺܶ�飬ÿ������Ϊһ��static mesh����Ⱦ
	*/
	class nAPI nchunked_terrain
		: public nactor
	{
	public:
		typedef shared_ptr<nchunked_terrain> ptr;

		nchunked_terrain(const nstring& name_str);
		virtual ~nchunked_terrain(void);

		void create(size_t w, size_t h, short init_h, size_t chunk_size=17);
		void destroy();

		void create_material_basic(const resource_location& texture_loc);

		void set_scale(const vector3& scale);
		void set_position(const vector3& pos);

		void import_heightmap(const nstring& img_file_name);
		/** �ڸ߶�ͼ��ָ�������ڲ���perlin noise�������	*/
		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);
	private:
		nchunked_terrain_impl*	m_impl;
		
		vector3	m_scale,
				m_pos;

		nDECLARE_NAMED_CLASS(nchunked_terrain)
	};
}//namespace nexus
#endif //_NEXUS_CHUNKED_TERRAIN_H_