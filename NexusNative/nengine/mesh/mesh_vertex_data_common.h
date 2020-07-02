/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MESH_VERTEX_DATA_COMMON_H_
#define _NEXUS_MESH_VERTEX_DATA_COMMON_H_
#include "mesh_vertex_data.h"

namespace nexus
{
	/**
	 *	��Ӧ���õ�static mesh�������ݸ�ʽ
	 *	@remark ��Ӧһ��factory shader�ļ���ͨ���궨����ƴ���������趨�м���normal������uv
	 *	.���ʹ����ģ����ơ�template<int num_normal, int num_uv>	 class mesh_vertex_data_common��������Ϊ����Ӧ���л����ĳ���nobject���	 
	*/	
	class nAPI nmesh_vertex_data_common :
		public nmesh_vertex_data
	{
	public:
		nmesh_vertex_data_common(void):m_num_normal(0),m_num_uv(0)
		{}
		virtual ~nmesh_vertex_data_common(void)	{}

		void init(size_t num_vert, int num_normal, int num_uv);	

		virtual nstring get_vertex_factory_name() const;
		virtual nstring get_vertex_type_name() const;		
		virtual size_t get_num_stream() const	{	return 2;}
		virtual vertex_stream* get_stream(size_t i)
		{
			nASSERT(i < 2);
			vertex_stream* ptr_array[3] = 
				{	&m_pos_stream,
					&m_second_stream,
					0,
			};

			return ptr_array[i];
		}

		virtual void serialize(narchive& ar);
	
	protected:
		int m_num_normal, 
			m_num_uv;
		vertex_stream	m_second_stream;	//	����normal, [tangent], uv[0~4], ������	

		nDECLARE_CLASS(nmesh_vertex_data_common)
	};

	class nAPI nmesh_vertex_data_common_morph :
		public nmesh_vertex_data_common
	{
	public:
		nmesh_vertex_data_common_morph(void)	{}
		virtual ~nmesh_vertex_data_common_morph(void)	{}

		virtual nstring get_vertex_factory_name() const;
		virtual nstring get_vertex_type_name() const;		

		nDECLARE_CLASS(nmesh_vertex_data_common_morph)
	};
}//namespace nexus

#endif //_NEXUS_MESH_VERTEX_DATA_COMMON_H_