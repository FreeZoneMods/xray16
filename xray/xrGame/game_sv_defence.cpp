#include "stdafx.h"
#include "game_sv_defence.h"
#include "string_table.h"
#include "clsid_game.h"
#include <functional>

#include "ui\UIBuyWndShared.h"

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

BOOL game_sv_Defence::OnTouchItem(CSE_ActorMP *actor, CSE_Abstract *item)
{
	VERIFY(actor);
	VERIFY(item);

	if ((item->m_tClassID == CLSID_OBJECT_PLAYERS_BAG) && (item->ID_Parent == 0xffff))
	{
		//-------------------------------
		//move all items from rukzak to player
		if (!item->children.empty())
		{
			NET_Packet	EventPack;
			NET_Packet	PacketReject;
			NET_Packet	PacketTake;

			EventPack.w_begin(M_EVENT_PACK);

			while (!item->children.empty())
			{
				CSE_Abstract *e_child_item = get_entity_from_eid(item->children.back());
				if (e_child_item)
				{
					if (!OnTouch(actor->ID, e_child_item->ID, FALSE))
					{
						NET_Packet P;
						u_EventGen(P, GE_OWNERSHIP_REJECT, item->ID);
						P.w_u16(e_child_item->ID);

						m_server->Process_event_reject(P, m_server->GetServerClient()->ID, 0, item->ID, e_child_item->ID);
						continue;
					}
				}

				m_server->Perform_transfer(PacketReject, PacketTake, e_child_item, item, actor);

				EventPack.w_u8(u8(PacketReject.B.count));
				EventPack.w(&PacketReject.B.data, PacketReject.B.count);
				EventPack.w_u8(u8(PacketTake.B.count));
				EventPack.w(&PacketTake.B.data, PacketTake.B.count);
			}
			if (EventPack.B.count > 2)
				u_EventSend(EventPack);
		}
		//-------------------------------
		//destroy the BAG
		DestroyGameItem(item);
		//-------------------------------
		return FALSE;

	};
	//---------------------------------------------------------------
	return TRUE;
}

void game_sv_Defence::OnDetachItem(CSE_ActorMP *actor, CSE_Abstract *item)
{
	R_ASSERT(actor);
	R_ASSERT(item);
	if (item->m_tClassID == CLSID_OBJECT_PLAYERS_BAG)
	{
		//move all items from player to rukzak
		xr_vector<u16>::const_iterator it_e = actor->children.end();

		xr_vector<CSE_Abstract*>			to_transfer;
		xr_vector<CSE_Abstract*>			to_destroy;
		xr_vector<CSE_Abstract*>			to_reject;
		// may be there is a sense to move next invokation into the ProcessDeath method...
		FillDeathActorRejectItems(actor, to_reject);

		for (xr_vector<u16>::const_iterator it = actor->children.begin();
			it != it_e; ++it)
		{
			u16 ItemID = *it;
			CSE_Abstract* e_item = get_entity_from_eid(ItemID);

			R_ASSERT(e_item->ID_Parent == actor->ID);

			if (std::find(to_reject.begin(), to_reject.end(), e_item) != to_reject.end())
				continue;

			if ((e_item->m_tClassID == CLSID_OBJECT_W_KNIFE) ||
				(e_item->m_tClassID == CLSID_DEVICE_TORCH))
			{
				to_destroy.push_back(e_item);
			}
			else if (m_strWeaponsData->GetItemIdx(e_item->s_name) != u32(-1))
			{
				if (!smart_cast<CSE_ALifeItemCustomOutfit*>(e_item))
				{
					to_transfer.push_back(e_item);
				}
			}
		}

		xr_vector<CSE_Abstract*>::const_iterator tr_it_e = to_transfer.end();

		NET_Packet							EventPack;
		NET_Packet							PacketReject;
		NET_Packet							PacketTake;
		EventPack.w_begin(M_EVENT_PACK);

		for (xr_vector<CSE_Abstract*>::const_iterator tr_it = to_transfer.begin();
			tr_it != tr_it_e; ++tr_it)
		{
			m_server->Perform_transfer(PacketReject, PacketTake, *tr_it, actor, item);
			EventPack.w_u8(u8(PacketReject.B.count));
			EventPack.w(&PacketReject.B.data, PacketReject.B.count);
			EventPack.w_u8(u8(PacketTake.B.count));
			EventPack.w(&PacketTake.B.data, PacketTake.B.count);
		}

		if (EventPack.B.count > 2)
			u_EventSend(EventPack);

		std::for_each(to_destroy.begin(), to_destroy.end(),
			std::bind1st(std::mem_fun<void, game_sv_mp, CSE_Abstract*>(
				&game_sv_mp::DestroyGameItem), this));

		std::for_each(to_reject.begin(), to_reject.end(),
			std::bind1st(std::mem_fun<void, game_sv_mp, CSE_Abstract*>(
				&game_sv_mp::RejectGameItem), this));

	};
}


BOOL game_sv_Defence::OnTouch(u16 eid_who, u16 eid_what, BOOL bForced)
{
	CSE_ActorMP *e_who = smart_cast<CSE_ActorMP*>(m_server->ID_to_entity(eid_who));
	if (!e_who)
		return FALSE;

	CSE_Abstract *e_entity = m_server->ID_to_entity(eid_what);
	if (!e_entity)
		return FALSE;

	return OnTouchItem(e_who, e_entity);
}

void game_sv_Defence::OnDetach(u16 eid_who, u16 eid_what)
{
	CSE_ActorMP *e_who = smart_cast<CSE_ActorMP*>(m_server->ID_to_entity(eid_who));
	if (!e_who)
		return;

	CSE_Abstract *e_entity = m_server->ID_to_entity(eid_what);
	if (!e_entity)
		return;

	OnDetachItem(e_who, e_entity);
}