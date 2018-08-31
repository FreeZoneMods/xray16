#pragma once

#include "game_cl_deathmatch.h"

class game_cl_Defence : public game_cl_Deathmatch
{
	typedef game_cl_Deathmatch inherited;

public:
	game_cl_Defence();
	virtual		bool				IsEnemy(game_PlayerState* ps);
	virtual		bool				IsEnemy(CEntityAlive* ea1, CEntityAlive* ea2);

	virtual		char*				getTeamSection(int Team);
	virtual		void				UpdateMapLocations();
	virtual		void				GetMapEntities(xr_vector<SZoneMapEntityData>& dst);
protected:
	bool							m_bFriendlyIndicators;
	bool							m_bFriendlyNames;
	bool							m_bShowPlayersNames;
	virtual		void				OnRender();
};