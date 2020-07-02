
#include "ndb_mysql/ndb_utility.h"
#include "nlogin_utility.h"

namespace nexus{

#ifdef NULL_MUTEX
	//���̰߳�ȫ���ڴ��
	nobject_pool<nnull_mutex>  g_pool(500 * 1024 * 1024);	//�ص��������500M
#else
	//�̰߳�ȫ���ڴ��
	nobject_pool<nfast_mutex>  g_pool(500 * 1024 * 1024);	//�ص��������500M
#endif

}