#pragma once

#include "nconfig.h"

enum EConfigIndex
{
	ECI_None					= 0,
	ECI_ServerIP,							// lan client�����������
	ECI_ServerPort,							// world��Ϸ����˿�
	ECI_ResPath,							// ��ԴĿ¼
	ECI_Max,
};

using namespace nexus;

class config : public nconfig
{
public:
	config();
	~config();

	static config* instance();

	virtual bool init(const tstring& file_name, uint16 values_count);
	virtual void destroy();
};
