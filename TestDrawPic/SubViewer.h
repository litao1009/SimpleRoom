#pragma once

#include "irrEngine/SRenderContextFwd.h"

// CSubViewer
//////////////////////////////////////////////////////////////////////////
//本类用于主、次窗口图像切换
//////////////////////////////////////////////////////////////////////////

class CSubViewer : public CStatic
{
	DECLARE_DYNAMIC(CSubViewer)

public:
	CSubViewer();
	virtual ~CSubViewer();

public:

	HDC m_hDC;
private:
	CDC* m_pDC;
	bool m_bLButtonDown;
	bool m_bMButtonDown;
	bool m_bRButtonDown;

	CRect m_rtMain;
	CPoint m_ptStart;
public:
	
	SRenderContextSPtr GetRenderContext() const { return m_spRenderContext; }
	void SetRenderContext(SRenderContextSPtr val) { m_spRenderContext = val; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:

	SRenderContextSPtr	m_spRenderContext;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL DestroyWindow();
};


