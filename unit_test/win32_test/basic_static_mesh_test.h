/**
*	Nexus Engine - Win32 GUI���Գ���
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#pragma once
#include "ntest_case.h"

/**
 * ���Ի�����static mesh�ļ��ء���Ⱦ
*/
class basic_static_mesh_test :
	public nrender_test
{
public:
	basic_static_mesh_test(void);
	virtual ~basic_static_mesh_test(void);

	virtual void open(editor_camera_controller* cam);	

	nDECLARE_CLASS(basic_static_mesh_test)
};
