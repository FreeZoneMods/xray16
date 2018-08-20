#pragma once

#include "UIDialogWnd.h"

class CUIStatix;
class CUIStatic;
class CUI3tButton;
class CUIScrollView;

//typedef	void (*ButtonClickCallback) (int);
typedef enum 
{
	TEAM_MENU_BACK_RP = 0,
	TEAM_MENU_SPECTATOR_RP,
} ETEAMMENU_BTN_RP;

class CUISpawnWndRP: public CUIDialogWnd
{
private:
	typedef CUIDialogWnd inherited;
public:
	CUISpawnWndRP();
	virtual ~CUISpawnWndRP();

	virtual void Init();
	virtual void SendMessage(CUIWindow *pWnd, s16 msg, void *pData);
	virtual bool OnKeyboardAction(int dik, EUIMessages keyboard_action);
	void SetVisibleForBtn(ETEAMMENU_BTN_RP btn, bool state);
	void SetCurTeam(int team);

protected:
	void InitTeamLogo();

	// Установка нового режима отображения
	//	void SetDisplayMode(bool bDual = false);

	// -1 - еще не нажималась, 0 - primary (левая), 1 - secondary (правая)
	//	int GetPressingResult() { return 1; }

	//	void	SetCallbackFunc (ButtonClickCallback pFunc);

protected:
	CUIStatic*		m_pCaption;
	CUIStatic*		m_pBackground;
	CUIStatic*		m_pFrames[9];

	CUIScrollView*	m_pTextDesc;

	CUIStatix*		m_pImage1;
	CUIStatix*		m_pImage2;
	CUIStatix*		m_pImage3;
	CUIStatix*		m_pImage4;
	CUIStatix*		m_pImage5;
	CUIStatix*		m_pImage6;
	CUIStatix*		m_pImage7;
	CUIStatix*		m_pImage8;
	CUIStatix*		m_pImage9;

	CUI3tButton*	m_pBtnSpectator;
	CUI3tButton*	m_pBtnBack;

	int		m_iCurTeam;
};