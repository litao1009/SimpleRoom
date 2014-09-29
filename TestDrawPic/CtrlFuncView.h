#pragma once


// CCtrlFuncView ��ͼ
//////////////////////////////////////////////////////////////////////////
//��������������������
//����ǽ���޸�ǽ���ƶ���ꡢ�ȵ�
//////////////////////////////////////////////////////////////////////////

#include "SubViewer.h"
#include "ODL/DesignODL.h"

class CCtrlFuncView : public CView
{
	DECLARE_DYNCREATE(CCtrlFuncView)

protected:

	CCtrlFuncView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

	virtual ~CCtrlFuncView();

public:

	CDesignODLSPtr		GetRootODL() const { return RootODL_; }

	void				SetRoot(CDesignODLSPtr root) { RootODL_ = root; }

	CSubViewer&			GetSubView() { return SubViewPort_; }

	const CSubViewer&	GetSubView() const { return SubViewPort_; }

private:

	CDesignODLSPtr	RootODL_;

	//�Ӵ�����ʾ
	CSubViewer		SubViewPort_;

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ

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


