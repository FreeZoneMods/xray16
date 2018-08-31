#include "stdafx.h"
#include "game_cl_defence.h"
#include "level.h"
#include "map_manager.h"
#include "map_location.h"
#include "actor.h"

#define	TEAM0_MENU		"defence_team"

game_cl_Defence::game_cl_Defence()
{
	m_bShowPlayersNames = false;
	m_bFriendlyIndicators = false;
	m_bFriendlyNames = false;
}

bool game_cl_Defence::IsEnemy(game_PlayerState* ps)
{
	return false;
}

bool game_cl_Defence::IsEnemy(CEntityAlive* ea1, CEntityAlive* ea2)
{
	return false;
};

char* game_cl_Defence::getTeamSection(int Team)
{
	return "defence_team";
};

#define FRIEND_LOCATION	"mp_friend_location"

void game_cl_Defence::UpdateMapLocations()
{
	inherited::UpdateMapLocations();

	if (local_player && !Level().CurrentEntity())
	{
		PLAYERS_MAP_IT it = players.begin();
		for (; it != players.end(); ++it)
		{
			game_PlayerState* ps = it->second;
			u16 id = ps->GameID;
			if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
			{
				Level().MapManager().RemoveMapLocation(FRIEND_LOCATION, id);
				continue;
			};

			CObject* pObject = Level().Objects.net_Find(id);
			if (!pObject || !smart_cast<CActor*>(pObject)) continue;
			if (IsEnemy(ps))
			{
				if (Level().MapManager().HasMapLocation(FRIEND_LOCATION, id))
				{
					Level().MapManager().RemoveMapLocation(FRIEND_LOCATION, id);
				};
				continue;
			};
			if (!Level().MapManager().HasMapLocation(FRIEND_LOCATION, id))
			{
				(Level().MapManager().AddMapLocation(FRIEND_LOCATION, id))->EnablePointer();
			}
		}
	};
}

void game_cl_Defence::GetMapEntities(xr_vector<SZoneMapEntityData>& dst)
{
	
	SZoneMapEntityData D;
	u32 color_self_team	= 0xff00ff00;
	D.color	= color_self_team;

	PLAYERS_MAP_IT it = players.begin();
	for(;it!=players.end();++it)
	{
		game_PlayerState* ps = it->second;
		u16 id = ps->GameID;
		if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD)) continue;
		CObject* pObject = Level().Objects.net_Find(id);
		if (!pObject) continue;
		if (!pObject || !smart_cast<CActor*>(pObject)) continue;

		VERIFY(pObject);
		D.pos = pObject->Position();
		dst.push_back(D);
	}
}

#define PLAYER_NAME_COLOR 0xff40ff40

void game_cl_Defence::OnRender()
{
	game_PlayerState* lookat_player = Game().lookat_player();
	if (local_player && (local_player == lookat_player) &&
		(m_bShowPlayersNames || m_bFriendlyIndicators))
	{
		cl_TeamStruct *pTS = &TeamList[ModifyTeam(local_player->team)];
		PLAYERS_MAP_IT it = players.begin();
		for (; it != players.end(); ++it)
		{
			game_PlayerState* ps = it->second;
			u16 id = ps->GameID;
			if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD)) continue;
			CObject* pObject = Level().Objects.net_Find(id);
			if (!pObject) continue;
			if (!pObject || !smart_cast<CActor*>(pObject)) continue;
			if (IsEnemy(ps)) continue;


			if (ps == local_player)
				continue;


			float dup = 0.0f;
			if (/*m_bFriendlyNames &&*/ m_bShowPlayersNames)
			{
				VERIFY(pObject);
				CActor* pActor = smart_cast<CActor*>(pObject);
				VERIFY(pActor);
				Fvector IPos = pTS->IndicatorPos;
				IPos.y -= pTS->Indicator_r2;
				pActor->RenderText(ps->getName(), IPos, &dup, PLAYER_NAME_COLOR);
			}
			if (m_bFriendlyIndicators)
			{
				VERIFY(pObject);
				CActor* pActor = smart_cast<CActor*>(pObject);
				VERIFY(pActor);
				Fvector IPos = pTS->IndicatorPos;
				IPos.y += dup;
				pActor->RenderIndicator(IPos, pTS->Indicator_r1, pTS->Indicator_r2, pTS->IndicatorShader);
			};
		}
	};
	inherited::OnRender();
}