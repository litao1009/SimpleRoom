#pragma once

// CDlgSelectHTML �Ի���
#include "BaseHTMLDialog.h"

class CDlgSelectHTML : public CBaseHTMLDialog
{
	DECLARE_DYNCREATE(CDlgSelectHTML)

public:
	CDlgSelectHTML(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSelectHTML();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	//1��GTypeID
	//2��ID
	void SendObj(VARIANT& vtGTypeID, VARIANT& vtObjID);
	void OnDrawWall();
	void OnDropDoor();
	void OnDropBeam();

public:
	void RefreshWindow();
	
// �Ի�������
	enum { IDD = IDD_DLG_HTML_PAGE_NC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	CStatic m_lblSplash;

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
public:
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
};
