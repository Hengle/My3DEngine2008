#include "nlogin.h"

using namespace nexus; 

int _tmain(int argc, char* argv[])
{
	// try - catch
	if (true == nlogin::instance().init())
	{
		nlogin::instance().run();
	}

	nlogin::instance().destroy();

	// to do: Ŀǰ��������ǿ�˳�����debug�汾�£������˳���ʱ������
	_exit(0);

	return 0;
}
