#pragma once
#include "game_unit.h"

/**
 *	��Ϸ��ͼ�е�NPC���������
*/
class game_npc : public game_unit
{
public:
	typedef boost::shared_ptr<game_npc> ptr;

	game_npc(void);
	virtual ~game_npc(void);
};
