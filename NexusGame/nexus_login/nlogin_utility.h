/**
 *	nexus login - nlogin_utility
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLOGIN_UTILITY_H_
#define _NLOGIN_NLOGIN_UTILITY_H_

#include "nlogin_define.h"
#include "ncommon.h"
#include "nobject_pool.h"
#include "timer/ntimestamp.h"

namespace nexus{

	enum EProofPolicy
	{
		EPP_Nul,
		EPP_Tes,		// ����ר��
		EPP_Own,		// ��˾��֤����
		EPP_XunLei,		// xunlei

		EPP_End
	};


	// ������ҵ�½״̬
	enum EPlayerLoginStatus
	{
		EPLS_None,
		EPLS_Proofing,		//��֤�˺�
		EPLS_EndProof,		//�˺���֤����
		EPLS_MiBao,			//��֤�ܱ�
		EPLS_End_MiBao,		//�ܱ���֤����
		EPLS_Queuing,		//�����Ŷ�
		EPLS_EnterWorld,	//����world
	};


	// �˺���Ϣ�ṹ��
	struct naccount_info
	{
		uint32	id;						// �˺�ID(�˺����Ƶ�CRCֵ)
		char	name[MAX_ACCOUNT_LEN];	// �˺�����
		char	psd[MAX_MD5_SIZE];		// ����
		uint8	privilege;				// Ȩ��
		uint64	last_login;				// �ϴε�½ʱ��
		uint32	last_ip;				// �ϴε�½IP
		uint8	login_status;			// ��½״̬
		uint8	mac[8];					// mac��ַ

		naccount_info(void)
		{
			memset(this, 0, sizeof(naccount_info));
		}
	};


	class nlogin_player;
	typedef	std::list<nlogin_player*>			PlayerList;
	typedef	PlayerList::iterator				PlayerListIt;


#define NULL_MUTEX
#ifdef NULL_MUTEX
	//���̰߳�ȫ���ڴ��
	extern nobject_pool<nnull_mutex>  g_pool;	//���ڷ����ڴ���ڴ��
#else
	//�̰߳�ȫ���ڴ��
	extern nobject_pool<nfast_mutex>  g_pool;	//���ڷ����ڴ���ڴ��
#endif


	//�������к����ڲ���ʱ����ص�
	//#define NWORLD_UPDATE_QUEUE			1		//nworld::update_queue()�����ŶӶ��н�����Ϸ�����ʱ��
	//#define NHANDLE_LOGIN_PROOF			1		//nlan_session::handle_login_proof2()��֤�˺š������ʱ��
	//#define NLOGIN_DB_CACHE_ACCOUNT_INFO	1		//nlogin_db::cache_account_info()�����˺ŵ�ʱ��
	//#define NLOGIN_DB_EXIST_ACCOUNT		1		//nlogin_db::exist_account()����˺��Ƿ���ڵ�ʱ��
	//#define NLOGIN_DB_FIX_LOGIN_STATUS	1		//nlogin_db::fix_player_login_status()�޸��˺ŵ�½״̬��ʱ��
	#define NHANDLE_LOGIN_PROOF_NOLY		1		//nlan_session::handle_login_proof2()ֻ��֤�˺š����벻������
} // namespace nexus

#endif	_NLOGIN_NLOGIN_UTILITY_H_