/**
 *	nexus network - nsafe_mem_pool �̰߳�ȫ�ڴ��
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: Y & D
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NSAFE_MEM_POOL_H_
#define _NNETWORK_NSAFE_MEM_POOL_H_

#include "ncommon.h"
#include "nmutex.h"
#include "nguard.h"

namespace nexus {

	/**
	 *	�ڴ��(ע�⣺�ڴ�صĴ�С��ʾ:�ڴ�ص�ǰ�����ڴ�Ĵ�С)
	 *	��ʼ��ʱ��
	 *	1. �ⲿ�趨�ڴ�ص���������С
	 *	2. �ڴ�ز����κ��ڴ�Ԥ����
	 *
	 *	�ⲿ�����ڴ�ʱ,�����жϣ�
	 *	1. ����ڴ���ж�Ӧ�ߴ�Ŀ��п�,ʹ�ÿ��п鷵��
	 *	2. û�п��п�ʱ,ֱ�������ڴ沢���ڴ�ǰ����tagMemItem,Ȼ�󷵻�
	 *
	 *	�ⲿ�ͷ��ڴ�ʱ,�����ж�:
	 *	1. ����ͷ��ڴ���С>�ڴ������С,ֱ���ͷŵ�ϵͳ�ڴ�
	 *	2. �����ǰ�ڴ�ش�С+�ͷ��ڴ���С<�ڴ������С,ֱ�ӷ����ڴ��
	 *	3. ���������ռ��������ռ������¼������ĵ�2���������ͨ��������е�4��
	 *	4. ����ֱ���ͷ���ϵͳ�ڴ�
	 */
	template<typename TMUTEX>
	class nmem_pool : private nnoncopyable
	{
	public:
		explicit nmem_pool(uint32 max_pool_size = 16 * 1024 * 1024);
		~nmem_pool();

		NINLINE void*	alloc(uint32 bytes);
		NINLINE void	free(void* const mem_ptr);
		NINLINE void*	try_alloc(uint32 bytes);					// ���Խ�����������
		NINLINE bool	try_free(void* const mem_ptr);				// ���Խ�����������
		NINLINE void	set_max_size(uint32 size) { m_max_size = size; }
		NINLINE uint32	get_size()		const { return m_current_size; }
		NINLINE uint32	get_malloc()	const { return m_malloc; }
		NINLINE uint32	get_gc()		const { return m_gc_times; }

	private:
		// �����ռ�
		NINLINE void _garbage_collect(uint32 expect_size, uint32 use_time);
		// ������ƥ��Ĵ�С
		NINLINE int32 _size2index(uint32 size, uint32& real_size);

	private:
		// �ڴ��ͷ����
		struct nnode
		{
			nnode*			next_ptr;
			nnode*			pre_ptr;
			int32			index;
			uint32			size;
			uint32			use_time;
			uint32			mem_ptr[1];							// ʵ���ڴ�ռ�
		};

		struct
		{
			nnode*			first_ptr;
			nnode*			last_ptr;
		}					m_pool[16];

		uint32				m_max_size;						// �ⲿ�趨�������������ڴ�
		uint32				m_malloc;						// ͳ���ã�ʵ��Malloc����
		uint32				m_gc_times;						// ͳ���ã�ʵ�������ռ�����

		uint32 volatile 	m_current_size;					// �ڴ���п����ڴ�����

		TMUTEX				m_mutex;
	};

	typedef nmem_pool<nfast_mutex> nsafe_mem_pool;

	//-----------------------------------------------------------------------------
	// construction
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	nmem_pool<TMUTEX>::nmem_pool(uint32 max_pool_size) : m_max_size(max_pool_size), m_malloc(0), m_gc_times(0), m_current_size(0)
	{
		ZeroMemory(m_pool, sizeof(m_pool));
	}

	//-----------------------------------------------------------------------------
	// destruction
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	nmem_pool<TMUTEX>::~nmem_pool()
	{
		for (int32 i = 0; i < 16; ++i)
		{
			while (m_pool[i].first_ptr)
			{
				nnode* node_ptr = m_pool[i].first_ptr;
				m_pool[i].first_ptr = m_pool[i].first_ptr->next_ptr;
				::free(node_ptr);
			}
		}
	}

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void* nmem_pool<TMUTEX>::alloc(uint32 bytes)
	{
		uint32 real_size = 0;
		int32 index = _size2index(bytes, real_size);

		if (-1 != index)
		{
			// ��ǰ����
			if (m_pool[index].first_ptr)	
			{
				nguard<TMUTEX> guard(m_mutex);

				// �����У��ʹӳ������
				if (m_pool[index].first_ptr)
				{
					nnode* node_ptr = m_pool[index].first_ptr;
					m_pool[index].first_ptr = m_pool[index].first_ptr->next_ptr;

					if (m_pool[index].first_ptr)
					{
						m_pool[index].first_ptr->pre_ptr = NULL;
					}
					else
					{
						m_pool[index].last_ptr = NULL;
					}

					m_current_size -= real_size;
					++node_ptr->use_time;
					return node_ptr->mem_ptr;	
				}
			}
		}

		++m_malloc;
		nnode* node_ptr = reinterpret_cast<nnode*>(::malloc(real_size + sizeof(nnode) - sizeof(uint32)));

		if (!node_ptr)
		{
			return NULL;
		}

		node_ptr->index		= index;
		node_ptr->size		= real_size;
		node_ptr->next_ptr	= NULL;
		node_ptr->pre_ptr	= NULL;
		node_ptr->use_time	= 0;
		return node_ptr->mem_ptr;			// ��ʵ���ڴ��з���
	}

	//-----------------------------------------------------------------------------
	// �ͷ�
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void nmem_pool<TMUTEX>::free(void* const mem_ptr)
	{
		nnode* node_ptr = reinterpret_cast<nnode*>(((uint8*)mem_ptr) - sizeof(nnode) + sizeof(uint32));

		if (-1 != node_ptr->index)
		{
			nguard<TMUTEX> guard(m_mutex);

			if (node_ptr->size + m_current_size > m_max_size && node_ptr->use_time > 0)
			{
				// �����ռ�
				_garbage_collect(node_ptr->size * 2, node_ptr->use_time);	
			}

			// �ڴ�ؿ�������
			if (node_ptr->size + m_current_size <= m_max_size) 
			{
				node_ptr->pre_ptr = NULL;
				node_ptr->next_ptr = m_pool[node_ptr->index].first_ptr;

				if (m_pool[node_ptr->index].first_ptr)
				{
					m_pool[node_ptr->index].first_ptr->pre_ptr = node_ptr;
				}
				else
				{
					m_pool[node_ptr->index].last_ptr = node_ptr;
				}

				m_pool[node_ptr->index].first_ptr = node_ptr;
				m_current_size += node_ptr->size;
				return;
			}
		}

		::free(node_ptr);
	}

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void* nmem_pool<TMUTEX>::try_alloc(uint32 bytes)
	{
#if _WIN32_WINNT < 0x0400
		return nmem_pool::alloc(bytes);
#else
		uint32 real_size = 0;
		int32 index = _size2index(bytes, real_size);
		
		if (-1 != index)
		{
			if (!m_mutex.try_lock())
			{
				return NULL;
			}

			// �����У��ʹӳ������
			if (m_pool[index].first_ptr)
			{
				nnode* node_ptr = m_pool[index].first_ptr;
				m_pool[index].first_ptr = m_pool[index].first_ptr->next_ptr;

				if (m_pool[index].first_ptr)
				{
					m_pool[index].first_ptr->pre_ptr = NULL;
				}
				else
				{
					m_pool[index].last_ptr = NULL;
				}

				m_current_size -= real_size;
				++node_ptr->use_time;
				m_mutex.unlock();
				return node_ptr->mem_ptr;	
			}

			m_mutex.unlock();
		}

		++m_malloc;
		nnode* node_ptr = reinterpret_cast<nnode*>(::malloc(real_size + sizeof(nnode) - sizeof(uint32)));

		if (!node_ptr)
		{
			return NULL;
		}

		node_ptr->index		= index;
		node_ptr->size		= real_size;
		node_ptr->next_ptr	= NULL;
		node_ptr->pre_ptr	= NULL;
		node_ptr->use_time	= 0;
		return node_ptr->mem_ptr;	// ��ʵ���ڴ��з���
#endif
	}

	//-----------------------------------------------------------------------------
	// �ͷ�
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	bool nmem_pool<TMUTEX>::try_free(void* const mem_ptr)
	{
#if _WIN32_WINNT < 0x0400
		nmem_pool::free(mem_ptr);
		return true;
#else
		nnode* node_ptr = reinterpret_cast<nnode*>(((uint8*)mem_ptr) - sizeof(nnode) + sizeof(uint32));
	
		if (-1 != node_ptr->index)
		{
			if (!m_mutex.try_lock())
			{
				return false;
			}

			if (node_ptr->size + m_current_size > m_max_size && node_ptr->use_time > 0)
			{
				// �����ռ�
				_garbage_collect(node_ptr->size * 2, node_ptr->use_time);
			}

			// �ڴ�ؿ�������
			if (node_ptr->size + m_current_size <= m_max_size)
			{
				node_ptr->pre_ptr = NULL;
				node_ptr->next_ptr = m_pool[node_ptr->index].first_ptr;

				if (m_pool[node_ptr->index].first_ptr)
				{
					m_pool[node_ptr->index].first_ptr->pre_ptr = node_ptr;
				}
				else
				{
					m_pool[node_ptr->index].last_ptr = node_ptr;
				}

				m_pool[node_ptr->index].first_ptr = node_ptr;
				m_current_size += node_ptr->size;
				m_mutex.unlock();
				return true;
			}

			m_mutex.unlock();
		}

		::free(node_ptr);
		return true;
#endif
	}

	//-----------------------------------------------------------------------------
	// �����ռ�
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void nmem_pool<TMUTEX>::_garbage_collect(uint32 expect_size, uint32 use_time)
	{
		++m_gc_times;
		uint32 free_size = 0;

		// �����Ŀ�ʼ����
		for (int32 i = 15; i >= 0; --i)
		{
			if (!m_pool[i].first_ptr)
			{
				continue;
			}

			// �����ʼ�ͷţ���Ϊ�����Nodeʹ�ô�����
			nnode* node_ptr = m_pool[i].last_ptr;

			while (node_ptr)
			{
				nnode* temp_node_ptr = node_ptr;
				node_ptr = node_ptr->pre_ptr;

				// �ͷŴ˽ڵ�
				if (temp_node_ptr->use_time < use_time)
				{
					if (node_ptr)
					{
						node_ptr->next_ptr = temp_node_ptr->next_ptr;
					}

					if (temp_node_ptr->next_ptr)
					{
						temp_node_ptr->next_ptr->pre_ptr = node_ptr;
					}

					if (m_pool[i].last_ptr == temp_node_ptr)
					{
						m_pool[i].last_ptr = node_ptr;
					}

					if (m_pool[i].first_ptr == temp_node_ptr)
					{
						m_pool[i].first_ptr = temp_node_ptr->next_ptr;
					}

					m_current_size -= temp_node_ptr->size;
					free_size += temp_node_ptr->size;
					::free(temp_node_ptr);
				}

				if (free_size >= expect_size)
				{
					return;
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	// ������ƥ��Ĵ�С
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	int32 nmem_pool<TMUTEX>::_size2index(uint32 size, uint32& real_size)
	{
		if (size <= 32)				{ real_size = 32;			return 0; }
		if (size <= 64)				{ real_size = 64;			return 1; }
		if (size <= 128)			{ real_size = 128;			return 2; }
		if (size <= 256)			{ real_size = 256;			return 3; }
		if (size <= 512)			{ real_size = 512;			return 4; }
		if (size <= 1024)			{ real_size = 1024;			return 5; }
		if (size <= 2 * 1024)		{ real_size = 2 * 1024;		return 6; }
		if (size <= 4 * 1024 )		{ real_size = 4 * 1024;		return 7; }
		if (size <= 8 * 1024 )		{ real_size = 8 * 1024;		return 8; }
		if (size <= 16 * 1024 )		{ real_size = 16 * 1024;	return 9; }
		if (size <= 32 * 1024 )		{ real_size = 32 * 1024;	return 10; }
		if (size <= 64 * 1024 )		{ real_size = 64 * 1024;	return 11; }
		if (size <= 128 * 1024 )	{ real_size = 128 * 1024;	return 12; }
		if (size <= 256 * 1024 )	{ real_size = 256 * 1024;	return 13; }
		if (size <= 512 * 1024 )	{ real_size = 512 * 1024;	return 14; }
		if (size <= 1024 * 1024 )	{ real_size = 1024 * 1024;	return 15; }
		real_size = size;
		return -1;
	}

} // namespace nexus

#endif _NNETWORK_NSAFE_MEM_POOL_H_
