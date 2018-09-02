#include "stdafx.h"
#include "UIGameDF.h"

#include "game_cl_Defence.h"

#include <dinput.h>

//--------------------------------------------------------------------

void CUIGameDF::SetClGame (game_cl_GameState* g)
{
	inherited::SetClGame(g);
	m_game = smart_cast<game_cl_Defence*>(g);
	R_ASSERT(m_game);
}

bool CUIGameDF::IR_UIOnKeyboardPress(int dik)
{
	switch (dik) {
		case DIK_CAPSLOCK :
		{
			if (m_game)
			{
				if (m_game->Get_ShowPlayerNamesEnabled())
					m_game->Set_ShowPlayerNames( !m_game->Get_ShowPlayerNames() );
				else
					m_game->Set_ShowPlayerNames(true);
				return true;
			};
		}break;
	}
	return inherited::IR_UIOnKeyboardPress(dik);
}

bool CUIGameDF::IR_UIOnKeyboardRelease(int dik)
{
	switch (dik) {
		case DIK_CAPSLOCK :
			{
				if (m_game)
				{
					if (!m_game->Get_ShowPlayerNamesEnabled())						
						m_game->Set_ShowPlayerNames(false);
					return true;
				};
			}break;
	}

	return inherited::IR_UIOnKeyboardRelease(dik);
}