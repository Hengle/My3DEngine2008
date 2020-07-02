
#include "ncache_center.h"



namespace nexus{

ncache_center g_cache_center;


ncache_center::ncache_center(void)
{
}


ncache_center::~ncache_center(void)
{
}


// ���������������ݿ�������
void ncache_center::start(void)
{
}


// ˢ��ʱ��
void ncache_center::update(uint32 elapse)
{
	::timeBeginPeriod(1);

	m_role_mgr.update(elapse);

	::timeEndPeriod(1);
}


// �ر�ʱ���ڴ��е����ݶ��洢�����ݿ�
void ncache_center::destroy(void)
{
	m_role_mgr.shut_dowm();
	m_role_mgr.unload_cache();
}


// ����character
void ncache_center::insert_character(ncharacter_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;

	m_role_mgr.create_role(data_ptr);
}

// ɾ��character
void ncache_center::destroy_character(uint64 role_id)
{
	m_role_mgr.delete_role(role_id);
}


} //namespace nexus