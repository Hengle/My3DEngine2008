/**
 *	nexus db - ncache_utility
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCACHE_UTILITY_H_
#define _NDB_NCACHE_UTILITY_H_

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>


#include "ncommon.h"
#include "nitem_define.h"
#include "nlogin_define.h"
#include "nobject_pool.h"
#include "timer/ntimestamp.h"

//�����Чָ��
#define UNVALID_POINT(ptr) (NULL==(ptr))

//�����Чָ��
#define VALID_POINT(ptr) (return NULL!=(ptr))

namespace nexus{
#pragma pack(push,1)

	struct data_base
	{
		bool	change;	// �����ýṹ�������Ƿ����ı�
	};

	/**
	 *	��¼��ҵ��˺ź���������Ϸ������Ϣ
	 *	@remark : �ýṹ�干 (40 + 16 + 20 + 16) �ֽ�
	 */
	struct ncharacter_cache : public data_base
	{
		//�˺š���ɫ���(40 byte)
		uint32	account_id;				// �˺�ID
		uint64	role_id;				// ��ɫID
		nchar	name[MAX_ACCOUNT_LEN];	// ��ɫ����

		//ְҵ����ɫ�������(16�ֽ�)
		uint8	sex;					// �Ա�
		uint8	race;					// ����
		uint8	class_type;				// ְҵ
		uint8	level;					// ��ǰ�ȼ�
		uint32	cur_exp;				// ��ǰӵ�еľ���ֵ
		uint32	hp;						// Ѫ��
		uint32	mp;						// ������

		//λ�����(20�ֽ�)
		uint16	map_id;					// ��ɫ���ڵ�ͼID
		float	x;						// X����
		float	y;						// y����
		float	z;						// z����
		float	orient;					// ����

		//ʱ�����(16�ֽ�)
		uint32	create_time; 			// ��ɫ����ʱ��
		uint32	login_time;				// ����ʱ��
		uint32	logout_time;			// ����ʱ��
		uint32	online_time;			// ������ʱ��(��λ:��)
		
		ncharacter_cache()
		{
			memset(this, 0, sizeof(ncharacter_cache));
			name[0] = '\0';
		}

		static ncharacter_cache* alloc_character_data(void);
		static void free_character_data(ncharacter_cache* character_cache_ptr);
	};

	struct nitem_cache : public data_base
	{
		nitem_data	item;
		static nitem_cache* alloc_item_data(void);
		static void free_item_data(nitem_cache* item_cache_ptr);
	};

	struct nequip_cache : public data_base
	{
		nequip_data	equip;

		nequip_cache()
		{
			memset(this, 0, sizeof(nequip_cache));
		}

		static nequip_cache* alloc_equip_data(void);
		static void free_equip_data(nequip_cache* equip_cache_ptr);
	};

	struct nskill_cache : public data_base
	{
		uint32	id;
		uint32	cd;

		nskill_cache()
		{
			id = 0;
			cd = 0;
		}

		static nskill_cache* alloc_skill_data(void);
		static void free_skill_data(nskill_cache* skill_cache_ptr);
	};

	struct nbuff_cache : public data_base
	{
	};

	typedef	boost::unordered_map<uint32, uint32>			Uint32Map;
	typedef	Uint32Map::iterator								Uint32MapIt;


	typedef	boost::unordered_set<uint64>					RoleSet;			//һ���˺Ŵ����Ĳ�ͬ��ɫ����
	typedef	RoleSet::iterator								RoleSetIt;


	typedef	boost::unordered_map<uint32, RoleSet>			AccountRoleMap;		//[key:account_id   value:role_ids]
	typedef	AccountRoleMap::iterator						AccountRoleMapIt;


	typedef	boost::unordered_map<uint64, ncharacter_cache*>	CharacterMap;		//[key:role_id		value:ncharacter_cache]
	typedef	CharacterMap::iterator							CharacterMapIt;


	typedef	boost::unordered_map<uint64, nitem_cache*>		ItemMap;			//[key:guid			value:nitem_cache*]
	typedef	ItemMap::iterator								ItemMapIt;

	typedef	boost::unordered_map<uint64, nequip_cache*>		EquipMap;			//[key:guid			value:nequip_cache*]
	typedef	EquipMap::iterator								EquipMapIt;

	typedef	boost::unordered_map<uint32, nskill_cache*>		SkillMap;			//[key:skill_id		value:nskill_cache*]
	typedef	SkillMap::iterator								SkillMapIt;


	typedef std::list<uint64>								Uint64List;
	typedef	Uint64List::iterator							Uint64ListIt;


	//���̰߳�ȫ���ڴ��
	extern nobject_pool<nnull_mutex>  g_pool;	//���ڷ����ڴ���ڴ��

	////�̰߳�ȫ���ڴ��
	//extern nobject_pool<nfast_mutex>  g_pool;	//���ڷ����ڴ���ڴ��


	//���µ����к궨�����ڲ���ʱ����ص�
	//#define	NSTORE_CENTER_CACHE_CHARACTER		//nstore_center::cache_character�������ִ��sql��ѯ�����ؽ������ʱ��
	//
	//#define	NSTORE_CENTER_CACHE_ITEM			//nstore_center::cache_item�������ִ��sql��ѯ�����ؽ������ʱ��
	//#define	NSTORE_CENTER_UPDATE_ITEM			//nstore_center::update_item�������ִ��sql��ʱ��
	//
	//#define	NSTORE_CENTER_CACHE_EQUIP			//nstore_center::cache_equip�������ִ��sql��ѯ�����ؽ������ʱ��
	//#define	NSTORE_CENTER_UPDATE_EQUIP_SPRINTF	//nstore_center::update_equip�������ƴдsql����ʱ��
	//#define	NSTORE_CENTER_UPDATE_EQUIP			//nstore_center::update_equip�������ִ��sql��ʱ��
	//
	//#define	NSTORE_CENTER_CACHE_SKILL			//nstore_center::cache_skill�������ִ��sql��ѯ�����ؽ������ʱ��
	//#define	NSTORE_CENTER_UPDATE_SKILL			//nstore_center::update_skill�������ִ��sql��ʱ��

	//#define	NITEM_UNLOAD_CACHE					//nitem::unload_cache��洢��ɫ������Ʒ��ʱ��
	//#define	NEQUIP_UNLOAD_CACHE					//nequip::unload_cache��洢��ɫ������Ʒ��ʱ��
	//#define	NSKILL_UNLOAD_CACHE					//nskill::unload_cache��洢��ɫ������Ʒ��ʱ��

#pragma pack(pop)

} // namespace nexus

#endif	_NDB_NCACHE_UTILITY_H_