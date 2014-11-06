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

	SRenderContextSPtr GetRenderContext() const { return m_spRenderContext; }
	void SetRenderContext(SRenderContextSPtr val) { m_spRenderContext = val; }

// 操作
public:

	COleDropTarget* m_pDrop;

private:

	Gdiplus::Color m_clGrout;
	SRenderContextSPtr	m_spRenderContext;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CTestDrawPicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
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

#ifndef _DEBUG  // TestDrawPicView.cpp 中的调试版本
inline CTestDrawPicDoc* CTestDrawPicView::GetDocument() const
   { return reinterpret_cast<CTestDrawPicDoc*>(m_pDocument); }
#endif

