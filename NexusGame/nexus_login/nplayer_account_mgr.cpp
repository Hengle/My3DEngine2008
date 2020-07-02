
#include "base_depend.h"
#include "nutil.h"
#include "nlogin_db.h"
#include "nlogin_player.h"
#include "nplayer_account_mgr.h"

namespace nexus{

//�����½������ȫ����ҹ������ӿ�
nplayer_account_mgr g_player_account_mgr;


nplayer_account_mgr::nplayer_account_mgr(void) : m_proof_policy(EPP_Nul)
{

}


nplayer_account_mgr::~nplayer_account_mgr(void)
{

}


bool nplayer_account_mgr::init(void)
{
	// to do(by leo):��֤������Ҫ�Ժ�ͨ�������ļ����ã���ʱ��Ĭ�ϲ���
	m_proof_policy	= EPP_Nul;
	m_cur_version	= 1;
	return true;
}


void nplayer_account_mgr::destroy(void)
{

}


// ���汾�Ƿ�һ��
bool nplayer_account_mgr::check_version(uint32 version)
{
	return true;

	return (version == m_cur_version);
}


// ������ƵĺϷ���
bool nplayer_account_mgr::check_name(const nchar* name)
{
	nstring str(name);

	//ȥ���س�
	size_t pos = 0;
	while( (pos = str.find(L'\r', pos)) != str.npos )
		str.replace(pos, 1, L"");

	//ȥ������
	pos = 0;
	while( (pos = str.find(L'\n', pos)) != str.npos )
		str.replace(pos, 1, L"");

	//ȥ��tab
	pos = 0;
	while( (pos = str.find('\t', pos)) != str.npos )
		str.replace(pos, 1, L"");

	//ȥ���ո�
	if((pos = str.find(L' ')) != str.npos)
	{
		str = str.substr(0 , pos);
	}

	//name�Ƿ�Ϊ��
	return !str.empty();
}


// ����˺��Ƿ����
bool nplayer_account_mgr::exist_name(const nchar* name)
{
	uint32 name_crc = s_util.crc32( (void*)name, (uint32)(sizeof(nchar)*_tcslen(name)) );

	uint32	account_id	= 0;
	uint32	last_ip		= 0;
	//if( !g_login_db.cache_account_data(name, account_id, last_ip) )
	//	return false;

	return true;
}


// ����˺��Ƿ����
bool nplayer_account_mgr::exist_account(uint32 account_id)
{
	return g_login_db.exist_account(account_id);
		}


} //namespace nexus