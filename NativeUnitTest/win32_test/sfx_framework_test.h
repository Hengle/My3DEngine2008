#pragma once
#include "ntest_case.h"

/**
 *	������Ч��Դ-��ЧԪ��-��Чʵ���ȼ�������������
*/
class sfx_framework_test :
	public nrender_test
{
public:
	nactor::ptr m_actor;

	sfx_framework_test(void);
	virtual ~sfx_framework_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void tick(float run_time);

	nDECLARE_CLASS(sfx_framework_test)
};
