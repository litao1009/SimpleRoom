// DragDataListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DragDataListCtrl.h"


// CDragDataListCtrl

IMPLEMENT_DYNAMIC(CDragDataListCtrl, CListCtrl)

CDragDataListCtrl::CDragDataListCtrl()
{

}

CDragDataListCtrl::~CDragDataListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDragDataListCtrl, CListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CDragDataListCtrl 消息处理程序



void CDragDataListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//可以创建draganddrop对象
	::PostMessage(this->GetParent()->GetSafeHwnd(), WM_USER+1000, 10010, 0);

	CListCtrl::OnLButtonDown(nFlags, point);

}
