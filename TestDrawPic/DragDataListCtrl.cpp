// DragDataListCtrl.cpp : ʵ���ļ�
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



// CDragDataListCtrl ��Ϣ�������



void CDragDataListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//���Դ���draganddrop����
	::PostMessage(this->GetParent()->GetSafeHwnd(), WM_USER+1000, 10010, 0);

	CListCtrl::OnLButtonDown(nFlags, point);

}
