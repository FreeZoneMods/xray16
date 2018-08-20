#include "stdafx.h"
#include "UIRankIndicator.h"
#include "UIXmlInit.h"
#include "UIStatic.h"

CUIRankIndicator::CUIRankIndicator()
{
	m_current		= u8(-1);
}

CUIRankIndicator::~CUIRankIndicator()
{
	for(u8 i=0; i<max_rank;++i)
		xr_delete	(m_ranks[i]);
}

void CUIRankIndicator::InitFromXml(CUIXml& xml_doc)
{
	CUIXmlInit::InitWindow(xml_doc, "rank_wnd", 0,	this);
	string256	str;
	for(u8 i=0; i<max_rank;++i){
		CUIStatic*&	s	= m_ranks[i];
		s				= xr_new<CUIStatic>();
		xr_sprintf(str, "rank_wnd:rank_%d", i);
		CUIXmlInit::InitStatic(xml_doc, str, 0,	s);
	}
	CUIStatic* back	= xr_new<CUIStatic>(); back->SetAutoDelete(true);
	CUIXmlInit::InitStatic(xml_doc, "rank_wnd:background", 0,	back);
	AttachChild	(back);
}

void CUIRankIndicator::SetRank(u8 team, u8 rank)
{
	// ���������� ������� max_rank - ���������� ������ * �������. 2 - ���-�� ������
	rank += 0;// team*(max_rank / 2); m4d_RP
	if(m_current==rank)		return;

	if(m_current!=u8(-1))
		DetachChild	(m_ranks[m_current]);

	m_current		= rank;
	AttachChild		(m_ranks[m_current]);
}

