#ifndef _NEXUS_DYNAMIC_VB_POOL_
#define _NEXUS_DYNAMIC_VB_POOL_
#include "d3d_ptr.h"

namespace nexus
{
	/**
	 *	��̬vertex buffer��
	 *	@remark ���������ʲô�����𣿼��볡������100��dynamic mesh������򵥵�Ϊÿ�������Լ������VB��
	 *		��ô������Դ���Ϊ100��dynamic mesh���ܺͣ����ÿ��dynamic mesh��ÿ֡�У�ȷ�Ͽɼ�ʱ�������
	 *		�����Դ棬����Ⱦ��ɺ��ͷ��Դ棬��ô������Դ���Ϊ�ɼ���dynamic mesh�ķ�ֵ�ĺͣ�����һ���ϴ�
	 *		�ĳ�����˵��ͨ����ԶԶС������mesh�ܺ͵ģ�
	 *		��������GPU morph����mesh��CPU������Чmesh�ȣ�
	*/
	class dynamic_vb_pool
	{
		dynamic_vb_pool(void);
		~dynamic_vb_pool(void);
	public:

		static dynamic_vb_pool* instance();

		void init(size_t max_item_size=512*1024)
		{
			m_max_item_size = max_item_size;
		}
		void destroy()
		{
			m_item_map.clear();
		}

		d3d_vb_ptr alloc_for_vertex_stream(const vertex_stream* stream, bool init_copy);
		void free(d3d_vb_ptr vb_ptr);

	private:
		d3d_vb_ptr pool_alloc(size_t s);

	private:
		size_t					m_max_item_size;		
		
		struct pool_item
		{
			d3d_vb_ptr	vb;
			size_t		size;
			bool		in_use;
			size_t		use_count;
		};
		typedef std::map<size_t, pool_item> item_map;
		item_map	m_item_map;	//key == pool_item.size
		size_t		m_pool_size;
	};
}//namespace nexus

#endif //_NEXUS_DYNAMIC_VB_POOL_