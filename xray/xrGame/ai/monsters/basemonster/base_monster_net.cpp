#include "stdafx.h"
#include "base_monster.h"

#include "../../../ai_object_location.h"
#include "../../../game_graph.h"
#include "../../../ai_space.h"
#include "../../../hit.h"
#include "../../../PHDestroyable.h"
#include "../../../CharacterPhysicsSupport.h"
#include "../control_direction_base.h"
#include "../control_animation_base.h"

#include "../../../../xrServerEntities/xrserver_objects_alife_monsters.h"
void CBaseMonster::net_Save			(NET_Packet& P)
{
	inherited::net_Save(P);
	m_pPhysics_support->in_NetSave(P);
}

BOOL CBaseMonster::net_SaveRelevant	()
{
	return (inherited::net_SaveRelevant() || BOOL(PPhysicsShell()!=NULL));
}

void CBaseMonster::net_Export(NET_Packet& P) 
{
	R_ASSERT				(Local());
	// export last known packet
	R_ASSERT(!NET.empty());
	net_update& N = NET.back();
	P.w_float(GetfHealth());
	P.w_u32(Level().timeServer());
	P.w_u8(0);
	P.w_vec3(Position());
	P.w_float /*w_angle8*/(N.o_model);
	P.w_float /*w_angle8*/(N.o_torso.yaw);
	P.w_float /*w_angle8*/(N.o_torso.pitch);
	P.w_float /*w_angle8*/(N.o_torso.roll);
	P.w_u8(u8(g_Team()));
	P.w_u8(u8(g_Squad()));
	P.w_u8(u8(g_Group()));

	GameGraph::_GRAPH_ID		l_game_vertex_id = ai_location().game_vertex_id();
	P.w(&l_game_vertex_id, sizeof(l_game_vertex_id));
	P.w(&l_game_vertex_id, sizeof(l_game_vertex_id));

	P.w_float(movement().m_body.current.yaw);
	P.w_float(movement().m_body.current.pitch);
}

void CBaseMonster::net_Import(NET_Packet& P)
{
	R_ASSERT				(Remote());
	net_update				N;
	u8 flags;
	float health;
	P.r_float(health);
	SetfHealth(health);

	P.r_u32(N.dwTimeStamp);
	P.r_u8(flags);
	P.r_vec3(N.p_pos);
	P.r_float /*r_angle8*/(N.o_model);
	P.r_float /*r_angle8*/(N.o_torso.yaw);
	P.r_float /*r_angle8*/(N.o_torso.pitch);
	P.r_float /*r_angle8*/(N.o_torso.roll);
	id_Team = P.r_u8();
	id_Squad = P.r_u8();
	id_Group = P.r_u8();

	GameGraph::_GRAPH_ID		l_game_vertex_id = ai_location().game_vertex_id();
	P.r(&l_game_vertex_id, sizeof(l_game_vertex_id));
	P.r(&l_game_vertex_id, sizeof(l_game_vertex_id));

	if (NET.empty() || (NET.back().dwTimeStamp<N.dwTimeStamp)) {
		NET.push_back(N);
		NET_WasInterpolating = TRUE;
	}

	P.r_float(movement().m_body.current.yaw);
	P.r_float(movement().m_body.current.pitch);

	//Collision sync
	SPHNetState state;
	state.position = N.p_pos;
	PHGetSyncItem(0)->set_State(state);
	make_Interpolation();

	setVisible				(TRUE);
	setEnabled				(TRUE);
}

BOOL CBaseMonster::net_Relevant()
{
	if (OnServer())
	{
		return getSVU() | getLocal();
	}
	else
	{
		return Local() & g_Alive();
	};
}

void CBaseMonster::Choose_Animation()
{
	if (Level().IsServer()) {
		NET_Packet MovePacket;
		EMotionAnim e_ma = anim().GetCurAnim();
		MotionID mid = anim().get_motion_id(e_ma);
		if (mid.valid()) {
			MovePacket.w_begin(M_MONSTER_ANM);
			MovePacket.w_u16(ID());
			MovePacket.w(&mid, sizeof(&mid));

			Level().Server->SendBroadcast(BroadcastCID, MovePacket);
		}
	}
}