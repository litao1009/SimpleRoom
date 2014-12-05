#pragma once

#include "resource.h"
// CBaseHTMLDialog 对话框
#include <afxdhtml.h>
class CBaseHTMLDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CBaseHTMLDialog)

public:
	CBaseHTMLDialog();
	CBaseHTMLDialog(UINT nIDTemplate, CWnd *pParentWnd = NULL);   // 标准构造函数
	virtual ~CBaseHTMLDialog();
// 重写
public:
	void SetIsDragWindow(bool bIsDrag)
	{
		m_bIsDrag = bIsDrag;
	};

	void SetIsParentNeedKey(bool bNeed)
	{
		m_bParentNeedKey = bNeed;
	};

	bool IsParentNeedKey(){return m_bParentNeedKey;};

	void SetReturnIsOK(BOOL bSetReturn =FALSE )
	{
		m_bReturnIsOK=bSetReturn;
	}

	virtual void OnFinalRelease();

	HRESULT Submit();
	void OKClose();

	void CancelClose();
	void ChangeSize(VARIANT& vtWidth, VARIANT& vtHeight);
	void MovingWindow();

	virtual BOOL IsExternalDispatchSafe(){return TRUE;};

	//STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved){return S_OK;};

private:
	BOOL m_bReturnIsOK;
	bool m_bParentNeedKey;		//上级窗口需要按键
	bool m_bIsDrag;		//窗口可以被拖动
	CString m_strBackground;
	POINT pt, pe;
	RECT rt, re;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
