#pragma once


// CCtrlFuncView 视图
//////////////////////////////////////////////////////////////////////////
//这个类拿来保存操作函数
//创建墙、修改墙、移动鼠标、等等
//////////////////////////////////////////////////////////////////////////

#include "ODL/DesignODL.h"

class CCtrlFuncView : public CView
{
	DECLARE_DYNCREATE(CCtrlFuncView)

protected:

	CCtrlFuncView();           // 动态创建所使用的受保护的构造函数

	virtual ~CCtrlFuncView();

public:

	DesignODLSPtr		GetRootODL() const { return RootODL_; }

	void				SetRoot(DesignODLSPtr root) { RootODL_ = root; }

private:

	DesignODLSPtr	RootODL_;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


