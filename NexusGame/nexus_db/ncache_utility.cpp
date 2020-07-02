
#include "nlog_mt.h"

#include "ncache_utility.h"

namespace nexus{


	//���̰߳�ȫ���ڴ��
	nobject_pool<nnull_mutex>  g_pool(500 * 1024 * 1024);	//�ص��������500M

	////�̰߳�ȫ���ڴ��
	//nobject_pool<nfast_mutex>  g_pool(500 * 1024 * 1024);	//�ص��������500M


	// ����һ�� ncharacter_cache ����
	ncharacter_cache* ncharacter_cache::alloc_character_data(void)
	{
		return g_pool.construct<ncharacter_cache>();
	}
	// ����һ�� ncharacter_cache ����
	void ncharacter_cache::free_character_data(ncharacter_cache* character_cache_ptr)
	{
		if(character_cache_ptr) g_pool.destroy<ncharacter_cache>(character_cache_ptr);
	}



	// ����һ�� nitem_cache ����
	nitem_cache* nitem_cache::alloc_item_data(void)
	{
		return g_pool.construct<nitem_cache>();
	}
	// ����һ�� nitem_cache ����
	void nitem_cache::free_item_data(nitem_cache* item_cache_ptr)
	{
		if(item_cache_ptr) g_pool.destroy<nitem_cache>(item_cache_ptr);
	}



	// ����һ�� nequip_cache ����
	nequip_cache* nequip_cache::alloc_equip_data(void)
	{
		return g_pool.construct<nequip_cache>();
	}
	// ����һ�� nequip_cache ����
	void nequip_cache::free_equip_data(nequip_cache* equip_cache_ptr)
	{
		if(equip_cache_ptr) g_pool.destroy<nequip_cache>(equip_cache_ptr);
	}



	// ����һ�� nskill_cache ����
	nskill_cache* nskill_cache::alloc_skill_data(void)
	{
		return g_pool.construct<nskill_cache>();
	}
	// ����һ�� nskill_cache ����
	void nskill_cache::free_skill_data(nskill_cache* skill_cache_ptr)
	{
		if(skill_cache_ptr) g_pool.destroy<nskill_cache>(skill_cache_ptr);
	}

}