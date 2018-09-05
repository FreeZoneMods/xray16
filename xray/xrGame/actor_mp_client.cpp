#include "stdafx.h"
#include "actor_mp_client.h"
#include "actorcondition.h"
#include "../xrEngine/CameraBase.h"
#include "../xrEngine/CameraManager.h"

#include "game_cl_base.h"
#include "ui/UIActorMenu.h"
#include "ui/UIDragDropReferenceList.h"
#include "uigamecustom.h"
#include "uigamedm.h"
#include "ui/UICharacterInfo.h"
#include "eatable_item.h"

//if we are not current control entity we use this value
const float	CActorMP::cam_inert_value = 0.7f;

CActorMP::CActorMP			()
{
	//m_i_am_dead				= false;
	CharacterName_ = "";
	CharacterRank_ = 0;
	CharacterCommunity_ = "";
	CharacterReputation_ = 0;
	CharacterIcon_ = "";
	IsGotCharacterInfo = 0;
}

void CActorMP::OnEvent		( NET_Packet &P, u16 type)
{
	if (type == GEG_PLAYER_USE_BOOSTER)
	{
		use_booster(P);
		return;
	}
	CUIGameDM* UIGameDM = smart_cast<CUIGameDM*>(CurrentGameUI());

	if (type == GE_DEADBODY_INFO)
	{
		P.r_stringZ(CharacterName_);
		P.r_stringZ(CharacterCommunity_);
		P.r_stringZ(CharacterIcon_);

		if (m_pPersonWeLookingAt) {
			CInventoryOwner* Actor = smart_cast<CInventoryOwner*>(this);
			CInventoryOwner* pEntityAliveWeLookingAt = smart_cast<CInventoryOwner*>(m_pPersonWeLookingAt);

			VERIFY(pEntityAliveWeLookingAt); //check if exist

			if (!m_pPersonWeLookingAt->deadbody_closed_status()) {
				Msg("Searching body, ID: %u", pEntityAliveWeLookingAt->object_id());
				UIGameDM->StartCarBodyOnClient(Actor, pEntityAliveWeLookingAt, CharacterName_, CharacterCommunity_, CharacterIcon_);
				return;
			}
		}
		return;
	}

	if (type == GE_DIALOG_INFO)
	{
		shared_str OurName;
		shared_str OurCommunity;
		shared_str OurIcon;		
		
		shared_str TheirName;
		shared_str TheirCommunity;
		shared_str TheirIcon;

		P.r_stringZ(OurName);
		P.r_stringZ(OurCommunity);
		P.r_stringZ(OurIcon);

		P.r_stringZ(TheirName);
		P.r_stringZ(TheirCommunity);
		P.r_stringZ(TheirIcon);

		if (m_pPersonWeLookingAt) {
			CInventoryOwner* pEntityAliveWeLookingAt = smart_cast<CInventoryOwner*>(m_pPersonWeLookingAt);

			VERIFY(pEntityAliveWeLookingAt); //check if exist

			if (m_pPersonWeLookingAt->is_alive()) {
				Msg("Talking, ID: %u", pEntityAliveWeLookingAt->object_id());
				UIGameDM->TalkMenu->InitTalkDialogOnClient(OurName, OurCommunity, OurIcon, TheirName, TheirCommunity, TheirIcon);
				return;
			}
		}
		return;
	}

	inherited::OnEvent(P,type);
}

void CActorMP::Die			(CObject *killer)
{
	//m_i_am_dead				= true;
	//conditions().health()	= 0.f;
	conditions().SetHealth( 0.f );
	inherited::Die			(killer);
}

void CActorMP::use_booster(NET_Packet &packet)
{
	if (OnServer())
		return;

	u16 tmp_booster_id;
	packet.r_u16			(tmp_booster_id);
	CObject* tmp_booster =	Level().Objects.net_Find(tmp_booster_id);
	VERIFY2(tmp_booster, "using unknown or deleted booster");
	if (!tmp_booster)
	{
		Msg("! ERROR: trying to use unkown booster object, ID = %d", tmp_booster_id);
		return;
	}

	CEatableItem* tmp_eatable = smart_cast<CEatableItem*>(tmp_booster);
	VERIFY2(tmp_eatable, "using not eatable object");
	if (!tmp_eatable)
	{
		Msg("! ERROR: trying to use not eatable object, ID = %d", tmp_booster_id);
		return;
	}
	tmp_eatable->UseBy(this);
}

void CActorMP::On_SetEntity()
{
	prev_cam_inert_value = psCamInert;
	if (this != Level().CurrentControlEntity())
	{
		psCamInert = cam_inert_value;
	}
	inherited::On_SetEntity();
}

void CActorMP::On_LostEntity()
{
	psCamInert = prev_cam_inert_value;
}