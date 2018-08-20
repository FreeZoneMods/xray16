#pragma once

#include "UIWindow.h"
class CUIStatic;
class CUIXml;
class CUIStatic;
class CUIRankIndicator :public CUIWindow
{
	//---m4d_RP (old 10)
	enum{max_rank=5,};
	CUIStatic*		m_ranks[max_rank];
	u8				m_current;
public:

					CUIRankIndicator		();
	virtual			~CUIRankIndicator		();
	void			InitFromXml				(CUIXml& xml_doc);
	void			SetRank					(u8 team, u8 rank);
};
