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

// ����
public:

	COleDropTarget* m_pDrop;

private:

	Gdiplus::Color m_clGrout;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CTestDrawPicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	//�������δ��ɲ���
	void ClearState();
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnBtnCreateWallLine();
	afx_msg void OnUpdateBtnCreateWallLine(CCmdUI *pCmdUI);

	afx_msg void OnBtnCreateWallRect();
	afx_msg void OnUpdateBtnCreateWallRect(CCmdUI *pCmdUI);

	afx_msg void OnBtnCreateBeamRect();
	afx_msg void OnUpdateBtnCreateBeamRect(CCmdUI *pCmdUI);

	afx_msg void OnBtnCreatePillarRect();
	afx_msg void OnUpdateBtnCreatePillarRect(CCmdUI *pCmdUI);

	afx_msg void OnBtnCreateRoomAuto();
	afx_msg void OnUpdateBtnCreateRoomAuto(CCmdUI *pCmdUI);

	afx_msg void OnBtnCreateRoomStyle();
	afx_msg void OnUpdateBtnCreateRoomStyle(CCmdUI *pCmdUI);

	afx_msg void OnBtnEditWallSkin();
	afx_msg void OnUpdateBtnEditWallSkin(CCmdUI *pCmdUI);

	afx_msg void OnBtnEditFloorSkin();
	afx_msg void OnUpdateBtnEditFloorSkin(CCmdUI *pCmdUI);

	afx_msg void OnBtnEditWallZone();
	afx_msg void OnUpdateBtnEditWallZone(CCmdUI *pCmdUI);

	afx_msg void OnBtnEditFloorZone();
	afx_msg void OnUpdateBtnEditFloorZone(CCmdUI *pCmdUI);

	afx_msg void OnBtnViewHide();
	afx_msg void OnUpdateBtnViewHide(CCmdUI *pCmdUI);

	afx_msg void OnBtnViewShow();
	afx_msg void OnUpdateBtnViewShow(CCmdUI *pCmdUI);

	afx_msg void OnBtnViewRender();
	afx_msg void OnUpdateBtnViewRender(CCmdUI *pCmdUI);

	afx_msg void OnBtnViewTop();
	afx_msg void OnUpdateBtnViewTop(CCmdUI *pCmdUI);

	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSubMenuProperty();
};

#ifndef _DEBUG  // TestDrawPicView.cpp �еĵ��԰汾
inline CTestDrawPicDoc* CTestDrawPicView::GetDocument() const
   { return reinterpret_cast<CTestDrawPicDoc*>(m_pDocument); }
#endif

