#pragma once
#include "game_unit.h"

/**
 * ��Ϸ��ͼ�е����
*/
class game_player :	public game_unit
{
public:
	typedef boost::shared_ptr<game_player> ptr;

	game_player(void);
	virtual ~game_player(void);

private:

};
