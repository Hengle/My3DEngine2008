
#include "nutil.h"
#include "nlog_mt.h"
#include "ndb_mysql/ndb_utility.h"

#include "nmsg_login.h"
#include "nlogin_db.h"
#include "nworld.h"
#include "nworld_mgr.h"
#include "nplayer_account_mgr.h"
#include "nlogin_player.h"


namespace nexus{

const uint32 DB_MIBAO_LEN = 240;


// ����һ�� nlogin_player ����
nlogin_player* nlogin_player::alloc_login_player(void)
{
	return g_pool.construct<nlogin_player>();
}


// ����һ�� nlogin_player ����
void nlogin_player::free_login_player(nlogin_player* player_ptr)
{
	if(player_ptr) g_pool.destroy<nlogin_player>(player_ptr);
}


nlogin_player::nlogin_player(void)
{
	m_need_mibao = false;
}

nlogin_player::~nlogin_player(void)
{
}

bool nlogin_player::init(uint64 client_id, uint32 account_id, uint32 ip)
{
	m_client_id	= client_id;
	m_info.login_status	= EPLS_None;
	m_info.id			= account_id;

	if( !g_login_db.cache_account_info(account_id, &m_info) )
	{//���ػ����ɫ�˺���Ϣ
		NLOG_ERROR(_T("%s : wrong to cache account"), __TFUNCTION__);
		return false;
	}
	m_info.last_ip		= ip;

	check_need_mibao();
	return true;
}


// ������֤
void nlogin_player::end_proof_account(void)
{
	set_status(EPLS_EndProof);
	nworld* world = g_world_mgr.get_world_by_name_crc(m_world_crc);
	if( !world ) return;

	if( m_need_mibao )
	{
		tagS2C_MiBao mibao;
		mibao.client_id = m_client_id;
		generate_mibao(mibao.MiBao);
		world->send_gateway_msg(&mibao, sizeof(mibao));

		set_status(EPLS_MiBao);
	}
	else
	{
		set_status(EPLS_End_MiBao);
		world->add_queue_player(this);
	}
}


void nlogin_player::end_proof_mibao(uint32 mibao_crc)
{
	
	tagS2C_LoginProofResult proof_result;

	proof_result.client_id	= m_client_id;
	proof_result.error		= ELoginProof_SUCCESS;

	if( !is_status(EPLS_MiBao) )
	{
		proof_result.error = ELoginError_UNKNOWN;
	}
	else if( m_mibao_crc != mibao_crc )
	{
		proof_result.error = ELoginProof_Mibao_Error;
	}

	nworld* world_ptr = g_world_mgr.get_world_by_name_crc(m_world_crc);
	if(!world_ptr)
		return;

	if( ELoginProof_Mibao_Error != proof_result.error )
	{
		world_ptr->send_gateway_msg(&proof_result, sizeof(proof_result));
	}

	world_ptr->remove_from_proof(this);

	set_status(EPLS_End_MiBao);
	world_ptr->add_queue_player(this);
}


void nlogin_player::set_status(EPlayerLoginStatus status)
{
	m_info.login_status = status;

	if(	EPLS_EnterWorld == status )
	g_login_db.fix_player_login_status(m_info.id, status);
}


bool nlogin_player::is_status(EPlayerLoginStatus status)
{
	return ( status == m_info.login_status );
}

// ��֤����
bool nlogin_player::proof(const char* psd)
{
	if( psd )
	{
		if( 0==strncmp(psd, m_info.psd, strlen(m_info.psd)) )
		{
			end_proof_account();
			return true;
		}
	}

	return false;
}

void nlogin_player::check_need_mibao(void)
{
	// to do(by leo):�ܱ���ʱΪ��
	m_need_mibao = false;
	return;

	char mibao[DB_MIBAO_LEN]={'\0'};

	m_need_mibao = g_login_db.get_mibao(m_info.id, mibao, DB_MIBAO_LEN);
	m_need_mibao = ('\0' != mibao[0]);
}


void nlogin_player::generate_mibao(nchar mibao[MAX_MIBAO_LEN])
{
	char mibao_db[DB_MIBAO_LEN] = {'\0'};
	
	if( !g_login_db.get_mibao(m_info.id, mibao_db, DB_MIBAO_LEN) )
	{
		return ;
	}

	/*	�ܱ��ĸ�ʽ���Զ��ŷָ����ַ����У�8*10�ľ���
		97,55,87,21,90,33,19,36,
		55,90,26,96,24,61,32,27,
		70,64,86,69,97,54,36,21,
		18,58,55,96,37,32,75,64,
		08,87,08,74,33,13,34,90,
		70,14,09,98,93,37,19,75,
		21,68,51,46,59,41,86,69,
		13,93,00,15,48,36,57,50,
		16,98,24,57,38,63,91,28,
		53,06,35,40,61,59,94,15	*/

	nchar mibao_value[MAX_MIBAO_LEN] = {'\0'};
	for(int32 n = 0; n < 3; ++n)
	{
		int32 nRand	= s_util.urand(1,80) % 80;	// ���ֵ
		int32 nX	= nRand / 10;				// x�ᣬ��'A'��'H'
		int32 nY	= nRand % 10;				// y�ᣬ��'0'��'9'

		// ���ܱ�������д�����к�
		mibao[n*2]	=	'A' + nX;
		mibao[n*2+1]=	'0' + nY;

		// ��ֵ��д���ܱ���ֵ
		mibao_value[n*2]	=	(nchar)(mibao_db[nRand*3]);
		mibao_value[n*2+1]	=	(nchar)(mibao_db[nRand*3+1]);
	}

	m_mibao_crc = s_util.crc32(mibao_value, MAX_MIBAO_LEN);
	return;
}


} //namespace nexus