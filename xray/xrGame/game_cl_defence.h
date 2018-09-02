#pragma once

#include "game_cl_deathmatch.h"

class CUIGameDF;

class game_cl_Defence : public game_cl_Deathmatch
{
typedef game_cl_Deathmatch inherited;
	CUIGameDF*						m_game_ui;
public:
	game_cl_Defence();
	virtual		bool				IsEnemy(game_PlayerState* ps);
	virtual		bool				IsEnemy(CEntityAlive* ea1, CEntityAlive* ea2);

	virtual		char*				getTeamSection(int Team);
	virtual		void				UpdateMapLocations();
	virtual		void				GetMapEntities(xr_vector<SZoneMapEntityData>& dst);

	virtual		void				Set_ShowPlayerNames(bool Show) { m_bShowPlayersNames = Show; };
	virtual		bool				Get_ShowPlayerNames() { return m_bShowPlayersNames; };
	virtual		bool				Get_ShowPlayerNamesEnabled() { return m_bFriendlyNames; };

	virtual		CUIGameCustom*		createGameUI();
	virtual		void				SetGameUI(CUIGameCustom*);

	virtual		void				OnConnected();

protected:
	bool							m_bFriendlyIndicators;
	bool							m_bFriendlyNames;
	bool							m_bShowPlayersNames;
	virtual		void				OnRender();
};