/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	2010-03-27
*/

#ifndef _NEXUS_ENGINE_NAV_MAP_VERTEX_DATA_H_
#define _NEXUS_ENGINE_NAV_MAP_VERTEX_DATA_H_
#include "../mesh/mesh_vertex_data.h"

namespace nexus
{

	/**
	* ��Ⱦ����ͼ�õĶ������ݸ�ʽ
	* @remark λ�ã�������ɫ
	*/
	class nav_map_vertex_data :
		public nmesh_vertex_data
	{
	public:
		nav_map_vertex_data(void);
		virtual ~nav_map_vertex_data(void);

		void init(size_t num_vert);

		//-- ���ػ����Ա
		virtual size_t get_num_stream() const { return 2; }
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		virtual vertex_stream* get_stream(size_t stream_index);

		//-- ����ͼ�Ķ��㲻��Ҫ���л�
		virtual void serialize(narchive& ar);

	private:
		// ������ɫ
		vertex_stream	m_diffuse_stream;
	};

} // end of namespace Nexus

#endif // end of _NEXUS_ENGINE_NAV_MAP_VERTEX_DATA_H_