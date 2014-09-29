// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// TestDrawPicView.h : CTestDrawPicView 类的接口
//

#pragma once

#include "CtrlFuncView.h"

class	CTestDrawPicDoc;
class CMainFrame;

class CTestDrawPicView : public CCtrlFuncView
{
protected: // 仅从序列化创建
	CTestDrawPicView();
	DECLARE_DYNCREATE(CTestDrawPicView)

// 特性
public:

	CTestDrawPicDoc* GetDocument() const;
	CMainFrame* GetMainFrame() const;

// 操作
public:

	COleDropTarget* m_pDrop;

private:

	Gdiplus::Color m_clGrout;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTestDrawPicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	//清除所有未完成操作
	void ClearState();
// 生成的消息映射函数
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

#ifndef _DEBUG  // TestDrawPicView.cpp 中的调试版本
inline CTestDrawPicDoc* CTestDrawPicView::GetDocument() const
   { return reinterpret_cast<CTestDrawPicDoc*>(m_pDocument); }
#endif

