// CtrlFuncView.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "CtrlFuncView.h"



IMPLEMENT_DYNCREATE(CCtrlFuncView, CView)

CCtrlFuncView::CCtrlFuncView()
{
}

CCtrlFuncView::~CCtrlFuncView()
{
}

BEGIN_MESSAGE_MAP(CCtrlFuncView, CView)
//	ON_WM_KEYUP()
//	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCtrlFuncView 绘图

void CCtrlFuncView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CCtrlFuncView 诊断

#ifdef _DEBUG
void CCtrlFuncView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCtrlFuncView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCtrlFuncView 消息处理程序

//void CCtrlFuncView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnKeyUp(nChar, nRepCnt, nFlags);
//}


//void CCtrlFuncView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnKeyDown(nChar, nRepCnt, nFlags);
//}


void CCtrlFuncView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	CView::OnLButtonDblClk(nFlags, point);
}
