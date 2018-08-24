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
	//	P.w						(&m_fGoingSpeed,			sizeof(m_fGoingSpeed));
	//	P.w						(&m_fGoingSpeed,			sizeof(m_fGoingSpeed));
	float					f1 = 0;
	if (ai().game_graph().valid_vertex_id(l_game_vertex_id)) {
		f1 = Position().distance_to(ai().game_graph().vertex(l_game_vertex_id)->level_point());
		P.w(&f1, sizeof(f1));
		f1 = Position().distance_to(ai().game_graph().vertex(l_game_vertex_id)->level_point());
		P.w(&f1, sizeof(f1));
	}
	else {
		P.w(&f1, sizeof(f1));
		P.w(&f1, sizeof(f1));
	};

	//Take current anm on server
	u32 anm_ = (u32)anim().GetCurAnim();
	P.w_u32					(anm_);
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

	//	P.r						(&m_fGoingSpeed,			sizeof(m_fGoingSpeed));
	//	P.r						(&m_fGoingSpeed,			sizeof(m_fGoingSpeed));
	float					f1 = 0;
		P.r(&f1, sizeof(f1));
		P.r(&f1, sizeof(f1));

	EMotionAnim anm;
	u32 anm_id;
	P.r_u32(anm_id);

	//Anm sync (temp)
	anm = Choose_Animation(anm_id);
	anim().SetCurAnim(anm);

	//Collision sync
	SPHNetState state;
	state.position = N.p_pos;
	PHGetSyncItem(0)->set_State(state);

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

EMotionAnim CBaseMonster::Choose_Animation(u32 id)
{
	switch (id)
	{									
	case 0:  return eAnimStandIdle;			 break;
	case 1:  return eAnimCapturePrepare;	 break;
	case 2:  return eAnimStandTurnLeft;		 break;
	case 3:  return eAnimStandTurnRight;	 break;
	case 4:  return eAnimSitIdle;			 break;
	case 5:  return eAnimLieIdle;			 break;
	case 6:  return eAnimSitToSleep;		 break;
	case 7:  return eAnimLieToSleep;		 break;
	case 8:  return eAnimStandSitDown;		 break;
	case 9:  return eAnimStandLieDown;		 break;
	case 10: return eAnimLieStandUp;		 break;
	case 11: return eAnimSitStandUp;		 break;
	case 12: return eAnimStandLieDownEat;	 break;
	case 13: return eAnimSitLieDown;		 break;
	case 14: return eAnimLieSitUp;			 break;
	case 15: return eAnimSleepStandUp;		 break;
	case 16: return	eAnimWalkFwd;			 break;
	case 17: return	eAnimWalkBkwd;			 break;
	case 18: return	eAnimWalkTurnLeft;		 break;
	case 19: return	eAnimWalkTurnRight;		 break;
	case 20: return	eAnimRun;				 break;
	case 21: return	eAnimRunTurnLeft;		 break;
	case 22: return	eAnimRunTurnRight;		 break;
	case 23: return	eAnimFastTurn;			 break;
	case 24: return	eAnimAttack;			 break;
	case 25: return	eAnimAttackFromBack;	 break;
	case 26: return	eAnimAttackRun;			 break;
	case 27: return	eAnimEat;				 break;
	case 28: return	eAnimSleep;				 break;
	case 29: return	eAnimSleepStanding;		 break;
	case 30: return	eAnimDie;				 break;
	case 31: return	eAnimDragCorpse;		 break;
	case 32: return	eAnimCheckCorpse;		 break;
	case 33: return	eAnimScared;			 break;
	case 34: return	eAnimAttackJump;		 break;
	case 35: return	eAnimLookAround;		 break;
	case 36: return	eAnimPrepareAttack;		 break;
	case 37: return	eAnimJump;				 break;
	case 38: return	eAnimSteal;				 break;
	case 39: return	eAnimJumpStart;			 break;
	case 40: return	eAnimJumpGlide;			 break;
	case 41: return	eAnimJumpFinish;		 break;
	case 42: return	eAnimJumpLeft;			 break;
	case 43: return	eAnimJumpRight;			 break;
	case 44: return	eAnimStandDamaged;		 break;
	case 45: return	eAnimWalkDamaged;		 break;
	case 46: return	eAnimRunDamaged;		 break;
	case 47: return	eAnimSniff;				 break;
	case 48: return	eAnimHowling;			 break;
	case 49: return	eAnimThreaten;			 break;
	case 50: return	eAnimMiscAction_00;		 break;
	case 51: return	eAnimMiscAction_01;		 break;
	case 52: return	eAnimUpperStandIdle;	 break;
	case 53: return	eAnimUpperStandTurnLeft; break;
	case 54: return	eAnimUpperStandTurnRight;break;
	case 55: return	eAnimStandToUpperStand;	 break;
	case 56: return	eAnimUppperStandToStand; break;
	case 57: return	eAnimUpperWalkFwd;		 break;
	case 58: return	eAnimUpperThreaten;		 break;
	case 59: return	eAnimUpperAttack;		 break;
	case 60: return	eAnimAttackPsi;			 break;
	case 61: return	eAnimTeleRaise;			 break;
	case 62: return	eAnimTeleFire;			 break;
	case 63: return	eAnimGraviPrepare;		 break;
	case 64: return	eAnimGraviFire;			 break;
	case 65: return	eAnimShieldStart;	     break;
	case 66: return	eAnimShieldContinue;	 break;
	case 67: return	eAnimTelekinesis;		 break;
	case 68: return	eAnimHomeWalkGrowl;		 break;
	case 69: return	eAnimHomeWalkSmelling;	 break;
	case 70: return	eAnimAttackOnRunLeft;	 break;
	case 71: return	eAnimAttackOnRunRight;	 break;
	case 72: return	eAnimAntiAimAbility;	 break;
	case 73: return	eAnimFastStandTurnLeft;  break;
	case 74: return	eAnimFastStandTurnRight; break;
	case 75: return	eAnimCount;				 break;
	case -1: return	eAnimUndefined;			 break;
	default: return	eAnimUndefined;
		break;

	}
}