#include "ncommon.h"

namespace nexus {

	enum EMapType
	{
		EMT_Common						= 0,					// �������ͼ
		EMT_Instance					= 1,					// ������ͼ
		EMT_ForeverInstance				= 2,					// ���ִ��ͼ
		EMT_Battleground				= 3,					// ս��
		EMT_Arena						= 4                     // ������
	};

	struct nmap_desc
	{
		uint32		map_id;
		uint32		instance_num;

		nmap_desc() : map_id(0), instance_num(0) {}
	};

	struct narea_trigger_info
	{
		uint32					info_id;						// id
		uint32					type;							// ����
	};

	struct nmap_info
	{
		uint32					info_id;						// ��ͼ�߼�id
		uint32					type;							// ��ͼ����
		tstring					name;							// ��ͼ����
		float					visible_distance;				// ���, ��ͨ�ֿ��Ӿ���
		uint32					max_player;						// ��������Ŀ(������gm)
		uint32					born_pos_id[5];					// ������id
		uint32					relive_pos_id[5];				// �����id

		// ˢ�ֵ���Ϣ
		// ��������Ϣ
		// ������Ϣ
	};


} // namespace nexus
