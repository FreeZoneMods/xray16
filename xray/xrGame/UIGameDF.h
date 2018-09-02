#pragma once

#include "UIGameCustom.h"
#include "UIGameDM.h"

class game_cl_Defence;

class CUIGameDF: public CUIGameDM
{
private:
	game_cl_Defence * m_game;
	typedef CUIGameDM inherited;
public:
	virtual void		SetClGame				(game_cl_GameState* g);
	
	virtual	bool		IR_UIOnKeyboardPress	(int dik);
	virtual bool		IR_UIOnKeyboardRelease	(int dik);
};
