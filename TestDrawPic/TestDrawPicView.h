// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TestDrawPicView.h : CTestDrawPicView ��Ľӿ�
//

#pragma once

#include "CtrlFuncView.h"

class	CTestDrawPicDoc;
class CMainFrame;

class CTestDrawPicView : public CCtrlFuncView
{
protected: // �������л�����
	CTestDrawPicView();
	DECLARE_DYNCREATE(CTestDrawPicView)

// ����
public:

	CTestDrawPicDoc* GetDocument() const;
	CMainFrame* GetMainFrame() const;

	SRenderContextSPtr GetRenderContext() const { return m_spRenderContext; }
	void SetRenderContext(SRenderContextSPtr val) { m_spRenderContext = val; }

// ����
public:

	COleDropTarget* m_pDrop;

private:

	Gdiplus::Color m_clGrout;
	SRenderContextSPtr	m_spRenderContext;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CTestDrawPicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSubMenuProperty();
	afx_msg void OnBtnRoomCreateDrawWall();
	afx_msg void OnBtnRoomCreateDrawRoom();
	afx_msg void OnBtnRoomPicturePicture();
};

#ifndef _DEBUG  // TestDrawPicView.cpp �еĵ��԰汾
inline CTestDrawPicDoc* CTestDrawPicView::GetDocument() const
   { return reinterpret_cast<CTestDrawPicDoc*>(m_pDocument); }
#endif

