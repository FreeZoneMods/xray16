#include "stdafx.h"
#include "game_sv_defence.h"
#include "string_table.h"

extern	BOOL	g_sv_tdm_bFriendlyIndicators;
extern	BOOL	g_sv_tdm_bFriendlyNames;
extern	float	g_sv_tdm_fFriendlyFireModifier;
extern	int		g_sv_tdm_iTeamKillLimit;
extern	int		g_sv_tdm_bTeamKillPunishment;

BOOL	game_sv_Defence::isFriendlyFireEnabled() { return (int(g_sv_tdm_fFriendlyFireModifier*100.0f) > 0); };
float	game_sv_Defence::GetFriendlyFire() { return (int(g_sv_tdm_fFriendlyFireModifier*100.0f) > 0) ? g_sv_tdm_fFriendlyFireModifier : 0.0f; };
BOOL	game_sv_Defence::Get_FriendlyIndicators() { return g_sv_tdm_bFriendlyIndicators; };
BOOL	game_sv_Defence::Get_FriendlyNames() { return g_sv_tdm_bFriendlyNames; };
int		game_sv_Defence::Get_TeamKillLimit() { return g_sv_tdm_iTeamKillLimit; };
BOOL	game_sv_Defence::Get_TeamKillPunishment() { return g_sv_tdm_bTeamKillPunishment; };

void game_sv_Defence::ReadOptions(shared_str &options)
{
	inherited::ReadOptions(options);

	g_sv_tdm_bFriendlyIndicators = get_option_i(*options, "fi", (g_sv_tdm_bFriendlyIndicators ? 1 : 0)) != 0;
	g_sv_tdm_bFriendlyNames = get_option_i(*options, "fn", (g_sv_tdm_bFriendlyNames ? 1 : 0)) != 0;

	float fFF = get_option_f(*options, "ffire", g_sv_tdm_fFriendlyFireModifier);
	g_sv_tdm_fFriendlyFireModifier = fFF;
}

KILL_RES game_sv_Defence::GetKillResult(game_PlayerState* pKiller, game_PlayerState* pVictim)
{
	KILL_RES Res = inherited::GetKillResult(pKiller, pVictim);
	switch (Res)
	{
		case KR_RIVAL:
		{
			if (pKiller->team == pVictim->team)
				Res = KR_TEAMMATE;
		}break;
		default:
		{
		}break;
	};
	return Res;
};

bool game_sv_Defence::OnKillResult(KILL_RES KillResult, game_PlayerState* pKiller, game_PlayerState* pVictim)
{
	bool res = true;
	TeamStruct* pTeam = GetTeamData(u8(pKiller->team));
	switch (KillResult)
	{
		case KR_TEAMMATE:
		{
			pKiller->m_iTeamKills++;
			if (pTeam)
				Player_AddMoney(pKiller, pTeam->m_iM_KillTeam);
			res = false;
		}break;
		default:
		{
			res = inherited::OnKillResult(KillResult, pKiller, pVictim);
		}break;
	}
	return res;
};

void game_sv_Defence::OnPlayerKillPlayer(game_PlayerState* ps_killer, game_PlayerState* ps_killed, KILL_TYPE KillType, SPECIAL_KILL_TYPE SpecialKillType, CSE_Abstract* pWeaponA)
{
	s16 OldKillsKiller = 0;
	s16 OldKillsVictim = 0;

	if (ps_killer)
	{
		OldKillsKiller = ps_killer->frags();
	}

	if (ps_killed)
	{
		OldKillsVictim = ps_killed->frags();
	}

	inherited::OnPlayerKillPlayer(ps_killer, ps_killed, KillType, SpecialKillType, pWeaponA);
	//-------------------------------------------------------------------	
	if (ps_killed && ps_killer)
	{
		if (ps_killed != ps_killer && ps_killer->team == ps_killed->team)
		{
			//Check for TeamKill
			if (Get_TeamKillPunishment())
			{
				if (ps_killer->m_iTeamKills >= Get_TeamKillLimit())
				{
					struct player_state_searcher
					{
						game_PlayerState* ps_killer;
						IClient* server_client;

						bool operator()(IClient* client)
						{
							xrClientData* pCL = (xrClientData*)client;
							if (!pCL || pCL == server_client) return false;
							if (!pCL->ps || pCL->ps != ps_killer) return false;
							return true;
						}
					};
					player_state_searcher tmp_predicate;
					tmp_predicate.ps_killer = ps_killer;
					tmp_predicate.server_client = m_server->GetServerClient();
					xrClientData* tmp_client = static_cast<xrClientData*>(
						m_server->FindClient(tmp_predicate)
						);
					if (tmp_client)
					{
#ifdef DEBUG
						Msg("--- Kicking player %s", tmp_client->ps->getName());
#endif
						LPSTR	reason;
						STRCONCAT(reason, CStringTable().translate("st_kicked_by_server").c_str());
						m_server->DisconnectClient(tmp_client, reason);
					}
				}
			}
		}
	}
}

//----хз пока
//void game_sv_Defence::OnPlayerHitPlayer_Case(game_PlayerState* ps_hitter, game_PlayerState* ps_hitted, SHit* pHitS)
//{
//	if (ps_hitter && ps_hitted)
//	{
//		if (ps_hitter->team == ps_hitted->team && ps_hitter != ps_hitted)
//		{
//			pHitS->power *= GetFriendlyFire();
//			pHitS->impulse *= (GetFriendlyFire()>1.0f) ? GetFriendlyFire() : 1.0f;
//		}
//	}
//
//	inherited::OnPlayerHitPlayer_Case(ps_hitter, ps_hitted, pHitS);
//};
//
//void game_sv_Defence::OnPlayerHitPlayer(u16 id_hitter, u16 id_hitted, NET_Packet& P)
//{
//	inherited::OnPlayerHitPlayer(id_hitter, id_hitted, P);
//};