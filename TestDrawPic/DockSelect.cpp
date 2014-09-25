// DockSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DockSelect.h"


// CDockSelect

IMPLEMENT_DYNAMIC(CDockSelect, CDockablePane)

CDockSelect::CDockSelect()
{

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
	m_dlgSelect.Create(CDlgSelect::IDD, this);
	CSize sSize;
	sSize.cx=270;
	sSize.cy=700;
	this->SetMinSize(sSize);
	return 0;
}


void CDockSelect::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (!m_dlgSelect.GetSafeHwnd())
	{
		return;
	}
	CRect rectClient;
	rectClient.SetRectEmpty();
	GetWindowRect(rectClient);
	ScreenToClient(rectClient);
	INT nInc=rectClient.right-rectClient.left;
	CDockablePane::OnSize(nType, cx, cy);
	if (nInc<270)
	{
		nInc=270-nInc;
		rectClient.right+=nInc;
		MoveWindow(rectClient);
		return;
	}
	// TODO: 在此处添加消息处理程序代码
	rectClient.top=0;
	rectClient.left=0;
	m_dlgSelect.MoveWindow(rectClient);
}
