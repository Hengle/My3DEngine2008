/**
*	nexus ndef_levelup
* ��ɫ��������ģ�嶨��
*/

#ifndef _NDEF_GAME_NDEF_LEVELUP_H_
#define _NDEF_GAME_NDEF_LEVELUP_H_

#include "ntype_def.h"
#include "ndef_base.h"

namespace nexus {

#pragma pack(push,1)

	// ��ɫ����������
	// idΪ��ɫ�ĵȼ�
	struct levelup_info : public att_info_base
	{
		uint32			exp;					//��һ����Ҫ�ľ���

		levelup_info(void)
			: exp(0xFFFFFFFF)
		{
		}

		explicit levelup_info(const nstring& id_str) 
			: att_info_base(id_str),exp(0xFFFFFFFF)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nSERIALIZE(ar, exp);
		}
	};

	//-- 
	SERIALIZE_ATT_INFO(levelup_info);


#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NDEF_LEVELUP_H_
