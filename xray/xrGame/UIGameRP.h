#pragma once

#include "UIGameCustom.h"
#include "UIGameDM.h"

#include "ui/UIDialogWnd.h"
#include "ui/UISpawnWndRP.h"

// refs 

class CUITDMFragList;
class CUITDMPlayerList;
class CUISkinSelectorWndRP;
class game_cl_RolePlay;
class CUIStatic;
class CUITextWnd;
class CUISpawnWndRP;

class CUIGameRP: public CUIGameDM
{
private:
	game_cl_RolePlay * m_game;
	typedef CUIGameDM inherited;

public:
	CUISpawnWndRP*		m_pUITeamSelectWnd;

protected:
	CUIStatic* m_team1_icon;
	CUIStatic* m_team2_icon;
	CUIStatic* m_team3_icon;
	CUIStatic* m_team4_icon;
	CUIStatic* m_team5_icon;
	CUIStatic* m_team6_icon;
	CUIStatic* m_team7_icon;
	CUIStatic* m_team8_icon;
	CUIStatic* m_team9_icon;

	CUITextWnd* m_team1_score;
	CUITextWnd* m_team2_score;
	CUITextWnd* m_team3_score;
	CUITextWnd* m_team4_score;
	CUITextWnd* m_team5_score;
	CUITextWnd* m_team6_score;
	CUITextWnd* m_team7_score;
	CUITextWnd* m_team8_score;
	CUITextWnd* m_team9_score;

	CUITextWnd*			m_buy_msg_caption;
public:
	CUIGameRP();
	virtual 			~CUIGameRP();
	virtual void		SetClGame(game_cl_GameState* g);
	virtual	void		UnLoad();
	virtual	void		Init(int stage);
	//-------m4d_RP
	void				SetScoreCaption(int t1, int t2, int t3, int t4, int t5, int t6, int t7, int t8, int t9); 
	void		SetBuyMsgCaption(LPCSTR str);
	virtual void		SetFraglimit(int local_frags, int fraglimit);
	virtual void		Render();
	virtual void _BCL	OnFrame();

	virtual	bool		IR_UIOnKeyboardPress(int dik);
	virtual bool		IR_UIOnKeyboardRelease(int dik);
};
