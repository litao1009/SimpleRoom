// CtrlFuncView.cpp : ʵ���ļ�
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


// CCtrlFuncView ��ͼ

void CCtrlFuncView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CCtrlFuncView ���

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


// CCtrlFuncView ��Ϣ�������

//void CCtrlFuncView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CView::OnKeyUp(nChar, nRepCnt, nFlags);
//}


//void CCtrlFuncView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CView::OnKeyDown(nChar, nRepCnt, nFlags);
//}


void CCtrlFuncView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	CView::OnLButtonDblClk(nFlags, point);
}
