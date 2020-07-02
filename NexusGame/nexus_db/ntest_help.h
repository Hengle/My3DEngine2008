/**
 *	nexus db - ntest_help
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NTEST_HELP_H_
#define _NDB_NTEST_HELP_H_

#include "nstore_center.h"
#include "ncache_center.h"

namespace nexus{

	class ntest_help
	{
	public:
		//���ָ����ɫ��Ϣ
		static void out_nchar(nchar* info);

		//�����ɫ�˺Ż�����Ϣ
		static void out_ncharacter_cache(ncharacter_cache* data_ptr);

		//���ָ����Ʒ��Ϣ
		static void out_nitem_cache(nitem_cache* data_ptr);

		//���ָ��װ����Ϣ
		static void out_nequip_cache(nequip_cache* data_ptr);

		//���ָ��������Ϣ
		static void out_nskill_cache(nskill_cache* data_ptr);

		//�����ɫ��������Ʒ��Ϣ
		static void out_role_item(nrole_info* role_ptr);

		//�����ɫ������װ����Ϣ
		static void out_role_equip(nrole_info* role_ptr);

		//�����ɫ�����м�����Ϣ
		static void out_role_skill(nrole_info* role_ptr);

		/**
		 *	�޸Ľ�ɫ��ָ����Ʒ��Ϣ
		 *	@remark : ������Ʒ����(����2��)�������Ʒ���ݵı仯
		 */
		static void change_role_item(nrole_info* role_ptr);

		/**
		 *	�޸Ľ�ɫ��ָ��װ����Ϣ 
		 *	@remark : ����װ������(����2��)�����װ�����ݵı仯
		 */
		static void change_role_equip(nrole_info* role_ptr);

		/**
		 *	�޸Ľ�ɫ��ָ��������Ϣ
		 *	@remark : ���Ӽ���CD(����2����λʱ��)����⼼�����ݵı仯
		 */
		static void change_role_skill(nrole_info* role_ptr);
	};
}

#endif _NDB_NTEST_HELP_H_