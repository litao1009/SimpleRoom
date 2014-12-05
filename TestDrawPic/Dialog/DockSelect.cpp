// DockSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "DockSelect.h"


// CDockSelect

IMPLEMENT_DYNAMIC(CDockSelect, CDockablePane)

CDockSelect::CDockSelect()
{
	m_bIsHTML=true;
}

CDockSelect::~CDockSelect()
{
}


BEGIN_MESSAGE_MAP(CDockSelect, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDockSelect 消息处理程序




int CDockSelect::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_dlgSelectHTML.Create(CDlgSelectHTML::IDD, this);
	m_bDisableAnimation=TRUE;
	return 0;
}


void CDockSelect::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CWnd* pWnd = nullptr;
	pWnd = &m_dlgSelectHTML;	
	if (!pWnd->GetSafeHwnd())
	{
		return;
	}
	CRect rectClient;
	rectClient.SetRectEmpty();
	GetWindowRect(rectClient);
	ScreenToClient(rectClient);
	
	// TODO: 在此处添加消息处理程序代码
	rectClient.top=0;
	rectClient.left=5;
	pWnd->MoveWindow(rectClient);
}
