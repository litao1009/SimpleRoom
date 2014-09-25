// DockLayer.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DockLayer.h"


// CDockLayer

IMPLEMENT_DYNAMIC(CDockLayer, CDockablePane)

CDockLayer::CDockLayer()
{

}

CDockLayer::~CDockLayer()
{
}


BEGIN_MESSAGE_MAP(CDockLayer, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDockLayer 消息处理程序




int CDockLayer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_dlgLayer.Create(CDlgLayer::IDD, this);
	
	return 0;
}


void CDockLayer::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (!m_dlgLayer.GetSafeHwnd())
	{
		return;
	}
	CRect rtMain;
	GetClientRect(rtMain);
	CRect rtTree;
	rtTree.left = 0;
	rtTree.right = rtMain.Width();
	rtTree.top = 0;
	rtTree.bottom = rtMain.Height();
	m_dlgLayer.MoveWindow(rtTree);
}
