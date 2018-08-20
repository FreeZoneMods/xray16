#include "stdafx.h"
#include <dinput.h>
#include "UISpawnWndRP.h"
#include "UIXmlInit.h"
#include "../level.h"
#include "../game_cl_teamdeathmatch.h"
#include "UIStatix.h"
#include "UIScrollView.h"
#include "UI3tButton.h"
#include "../xr_level_controller.h"
#include "uicursor.h"
#include "uigamecustom.h"

CUISpawnWndRP::CUISpawnWndRP()
	: m_iCurTeam(0)
{
	m_pBackground = xr_new<CUIStatic>();	AttachChild(m_pBackground);
	m_pCaption = xr_new<CUIStatic>();	AttachChild(m_pCaption);

	m_pImage1 = xr_new<CUIStatix>();	AttachChild(m_pImage1);
	m_pImage2 = xr_new<CUIStatix>();	AttachChild(m_pImage2);
	m_pImage3 = xr_new<CUIStatix>();	AttachChild(m_pImage3);
	m_pImage4 = xr_new<CUIStatix>();	AttachChild(m_pImage4);
	m_pImage5 = xr_new<CUIStatix>();	AttachChild(m_pImage5);
	m_pImage6 = xr_new<CUIStatix>();	AttachChild(m_pImage6);
	m_pImage7 = xr_new<CUIStatix>();	AttachChild(m_pImage7);
	m_pImage8 = xr_new<CUIStatix>();	AttachChild(m_pImage8);
	m_pImage9 = xr_new<CUIStatix>();	AttachChild(m_pImage9);


	m_pFrames[0] = xr_new<CUIStatic>();	AttachChild(m_pFrames[0]);
	m_pFrames[1] = xr_new<CUIStatic>();	AttachChild(m_pFrames[1]);
	m_pFrames[2] = xr_new<CUIStatic>();	AttachChild(m_pFrames[2]);
	m_pFrames[3] = xr_new<CUIStatic>();	AttachChild(m_pFrames[3]);
	m_pFrames[4] = xr_new<CUIStatic>();	AttachChild(m_pFrames[4]);
	m_pFrames[5] = xr_new<CUIStatic>();	AttachChild(m_pFrames[5]);
	m_pFrames[6] = xr_new<CUIStatic>();	AttachChild(m_pFrames[6]);
	m_pFrames[7] = xr_new<CUIStatic>();	AttachChild(m_pFrames[7]);
	m_pFrames[8] = xr_new<CUIStatic>();	AttachChild(m_pFrames[8]);
	//	m_pFrames[2]	= xr_new<CUIStatic>();	AttachChild(m_pFrames[2]);

	m_pTextDesc = xr_new<CUIScrollView>();	AttachChild(m_pTextDesc);

	m_pBtnSpectator = xr_new<CUI3tButton>();	AttachChild(m_pBtnSpectator);
	m_pBtnBack = xr_new<CUI3tButton>();	AttachChild(m_pBtnBack);

	Init();
}

CUISpawnWndRP::~CUISpawnWndRP()
{
	xr_delete(m_pCaption);
	xr_delete(m_pBackground);

	xr_delete(m_pFrames[0]);
	xr_delete(m_pFrames[1]);
	xr_delete(m_pFrames[2]);
	xr_delete(m_pFrames[3]);
	xr_delete(m_pFrames[4]);
	xr_delete(m_pFrames[5]);
	xr_delete(m_pFrames[6]);
	xr_delete(m_pFrames[7]);
	xr_delete(m_pFrames[8]);

	xr_delete(m_pImage1);
	xr_delete(m_pImage2);
	xr_delete(m_pImage3);
	xr_delete(m_pImage4);
	xr_delete(m_pImage5);
	xr_delete(m_pImage6);
	xr_delete(m_pImage7);
	xr_delete(m_pImage8);
	xr_delete(m_pImage9);

	xr_delete(m_pTextDesc);
	xr_delete(m_pBtnSpectator);
	xr_delete(m_pBtnBack);
}

void CUISpawnWndRP::Init()
{
	CUIXml xml_doc;
	xml_doc.Load(CONFIG_PATH, UI_PATH, "spawn_rp.xml");

	CUIXmlInit::InitWindow(xml_doc, "team_selector", 0, this);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:caption", 0, m_pCaption);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:background", 0, m_pBackground);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_freedom", 0, m_pFrames[0]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_merc", 0, m_pFrames[1]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_stalker", 0, m_pFrames[2]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_bandit", 0, m_pFrames[3]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_clear_sky", 0, m_pFrames[4]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_dolg", 0, m_pFrames[5]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_monolith", 0, m_pFrames[6]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_ecolog", 0, m_pFrames[7]);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_frames_army", 0, m_pFrames[8]);

	CUIXmlInit::InitScrollView(xml_doc, "team_selector:text_desc", 0, m_pTextDesc);

	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_0", 0, m_pImage1);
	//m_pImage1->SetStretchTexture(true);//
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_1", 0, m_pImage2);
	// m_pImage2->SetStretchTexture(true);//
	//InitTeamLogo();//
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_2", 0, m_pImage3);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_3", 0, m_pImage4);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_4", 0, m_pImage5);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_5", 0, m_pImage6);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_6", 0, m_pImage7);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_7", 0, m_pImage8);
	CUIXmlInit::InitStatic(xml_doc, "team_selector:image_8", 0, m_pImage9);

	CUIXmlInit::Init3tButton(xml_doc, "team_selector:btn_spectator", 0, m_pBtnSpectator);
	//CUIXmlInit::Init3tButton(xml_doc, "team_selector:btn_autoselect", 0, m_pBtnAutoSelect);
	CUIXmlInit::Init3tButton(xml_doc, "team_selector:btn_back", 0, m_pBtnBack);
}

void CUISpawnWndRP::InitTeamLogo() 
{
	//---------m4d
	R_ASSERT(pSettings->section_exist("team_logo"));

	R_ASSERT(pSettings->line_exist("team_logo", "team1"));
	R_ASSERT(pSettings->line_exist("team_logo", "team2"));
	R_ASSERT(pSettings->line_exist("team_logo", "team3"));
	R_ASSERT(pSettings->line_exist("team_logo", "team4"));
	R_ASSERT(pSettings->line_exist("team_logo", "team5"));
	R_ASSERT(pSettings->line_exist("team_logo", "team6"));
	R_ASSERT(pSettings->line_exist("team_logo", "team7"));
	R_ASSERT(pSettings->line_exist("team_logo", "team8"));
	R_ASSERT(pSettings->line_exist("team_logo", "team9"));

	m_pImage1->InitTexture(pSettings->r_string("team_logo", "team1"));
	m_pImage2->InitTexture(pSettings->r_string("team_logo", "team2"));
	m_pImage3->InitTexture(pSettings->r_string("team_logo", "team3"));
	m_pImage4->InitTexture(pSettings->r_string("team_logo", "team4"));
	m_pImage5->InitTexture(pSettings->r_string("team_logo", "team5"));
	m_pImage6->InitTexture(pSettings->r_string("team_logo", "team6"));
	m_pImage7->InitTexture(pSettings->r_string("team_logo", "team7"));
	m_pImage8->InitTexture(pSettings->r_string("team_logo", "team8"));
	m_pImage9->InitTexture(pSettings->r_string("team_logo", "team9"));
}

void CUISpawnWndRP::SendMessage(CUIWindow *pWnd, s16 msg, void *pData)
{
	if (BUTTON_CLICKED == msg)
	{
		game_cl_mp * game = smart_cast<game_cl_mp*>(&Game());
		VERIFY(game);
		//game_cl_TeamDeathmatch * tdm = smart_cast<game_cl_TeamDeathmatch *>(&(Game()));
		//Под кейсы!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (pWnd == m_pImage1)
		{
			game->OnTeamSelect(0);
		}
		else if (pWnd == m_pImage2)
		{
			game->OnTeamSelect(1);
		}
		else if (pWnd == m_pImage3)
		{
			game->OnTeamSelect(2);
		}
		else if (pWnd == m_pImage4)
		{
			game->OnTeamSelect(3);
		}
		else if (pWnd == m_pImage5)
		{
			game->OnTeamSelect(4);
		}
		else if (pWnd == m_pImage6)
		{
			game->OnTeamSelect(5);
		}
		else if (pWnd == m_pImage7)
		{
			game->OnTeamSelect(6);
		}
		else if (pWnd == m_pImage8)
		{
			game->OnTeamSelect(7);
		}
		else if (pWnd == m_pImage9)
		{
			game->OnTeamSelect(8);
		}
		else if (pWnd == m_pBtnSpectator)
		{
			game->OnSpectatorSelect();
		}
		else if (pWnd == m_pBtnBack)
		{
			game->OnTeamMenuBack();
			/*MainMenu()->Activate(true);*/
		}

		HideDialog();
	}

	inherited::SendMessage(pWnd, msg, pData);
}

////////////////////////////////////////////////////////////////////////////////

bool CUISpawnWndRP::OnKeyboardAction(int dik, EUIMessages keyboard_action)
{
	if (WINDOW_KEY_PRESSED != keyboard_action)
	{
		if (dik == DIK_TAB)
		{
			ShowChildren(true);
			game_cl_mp* game = smart_cast<game_cl_mp*>(&Game());
			game->OnKeyboardRelease(kSCORES);
			UI().GetUICursor().Show();
		}
		return false;
	}

	if (dik == DIK_TAB)
	{
		ShowChildren(false);
		game_cl_mp* game = smart_cast<game_cl_mp*>(&Game());
		game->OnKeyboardPress(kSCORES);
		UI().GetUICursor().Hide();
		return false;
	}

	game_cl_mp* game = smart_cast<game_cl_mp*>(&Game());
	VERIFY(game);
	//game_cl_TeamDeathmatch * dm = smart_cast<game_cl_TeamDeathmatch *>(&(Game()));

	if (DIK_1 == dik || DIK_2 == dik || DIK_3 == dik ||
		DIK_4 == dik || DIK_5 == dik || DIK_6 == dik ||
		DIK_7 == dik || DIK_8 == dik || DIK_9 == dik)
	{
		switch (dik)
		{
			case DIK_1:
			{
				game->OnTeamSelect(0);
			}
			break;
			case DIK_2: 
			{
				game->OnTeamSelect(1);
			}
			break;
			case DIK_3:
			{
				game->OnTeamSelect(2);
			}
			break;
			case DIK_4: 
			{
				game->OnTeamSelect(3);
			}
			break;
			case DIK_5:
			{
				game->OnTeamSelect(4);
			}
			break;
			case DIK_6:
			{
				game->OnTeamSelect(5);
			}
			break;
			case DIK_7:
			{
				game->OnTeamSelect(6);
			}
			break;
			case DIK_8:
			{
				game->OnTeamSelect(7);
			}
			break;
			case DIK_9:
			{
				game->OnTeamSelect(8);
			}
			break;
		}
		HideDialog();
		return true;
	}
	switch (dik)
	{
		case DIK_ESCAPE:
		{
			HideDialog();
			game->OnTeamMenuBack();
			return true;
		}
		case DIK_SPACE:
		{
			HideDialog();
			game->OnTeamSelect(-1);
			return true;
		}
		case DIK_RETURN:
		{
			if (m_pImage1->GetSelectedState())
				game->OnTeamSelect(0);
			else if (m_pImage2->GetSelectedState())
				game->OnTeamSelect(1);
			else if (m_pImage3->GetSelectedState())
				game->OnTeamSelect(2);
			else if (m_pImage4->GetSelectedState())
				game->OnTeamSelect(3);
			else if (m_pImage5->GetSelectedState())
				game->OnTeamSelect(4);
			else if (m_pImage6->GetSelectedState())
				game->OnTeamSelect(5);
			else if (m_pImage7->GetSelectedState())
				game->OnTeamSelect(6);
			else if (m_pImage8->GetSelectedState())
				game->OnTeamSelect(7);
			else if (m_pImage9->GetSelectedState())
				game->OnTeamSelect(8);
			else
				game->OnTeamSelect(-1);

			HideDialog();
			return true;
		}
	}

	return inherited::OnKeyboardAction(dik, keyboard_action);
}

void CUISpawnWndRP::SetVisibleForBtn(ETEAMMENU_BTN_RP btn, bool state)
{
	switch (btn)
	{
		case 	TEAM_MENU_BACK_RP:			this->m_pBtnBack->SetVisible(state);		break;
		case	TEAM_MENU_SPECTATOR_RP:	this->m_pBtnSpectator->SetVisible(state);	break;
		default:
			R_ASSERT2(false, "invalid btn ID");
	}
}

void CUISpawnWndRP::SetCurTeam(int team)
{
	//R_ASSERT2(team >= -1 && team <= 1, "Invalid team number");

	//m_iCurTeam = team;
	//m_pImage1->SetSelectedState(0 == team ? true : false);
	//m_pImage2->SetSelectedState(1 == team ? true : false);
}

