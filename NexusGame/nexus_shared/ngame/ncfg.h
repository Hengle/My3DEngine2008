/**
 *	nexus ngame - ncfg
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NCFG_H_
#define _NGAME_NCFG_H_

#include "nconfig.h"
#include "nsingleton.h"

namespace nexus {

	enum EConfigIndex
	{
		ECI_None					= 0,
		ECI_WorldPort,							// world��Ϸ����˿�
		ECI_MaxLanSessionClient,				// lan client�����������
		ECI_MaxLanSessionServer,				// lan server�����������
		ECI_NLoginIp,							// login server��ip��ַ
		ECI_NLoginPort,							// login server�Ķ˿�
		ECI_NBgIp,								// bg server��ip��ַ
		ECI_NBgPort,							// bg server�Ķ˿�
		ECI_AeraId,								// �����������������id(<0xff)
		ECI_BattleGroundId,						// �������������ս����id(<0xff)
		ECI_WorldId,							// ս��������Ϸ����id(<0xff)
		ECI_WorldName,							// ��Ϸ�������ƣ�����������
		ECI_ServerId,							// ������id, �ɼ������ó�
		ECI_ResPath,							// ��������Դ·����Ŀ¼

		ECI_FloatTest,							// float test
		ECI_Max,
	};

	/**
	 *	ncfg
	 */
	class ncfg : public nconfig
	{
	public:
		ncfg();
		~ncfg();

		virtual bool init(const tstring& file_name, uint16 values_count);
		virtual void destroy();

	private:
		uint32 _calculate_server_id();
	};

	#define sncfg nsingleton<ncfg>::instance()

} // namespace nexus

#endif // _NGAME_NCFG_H_
