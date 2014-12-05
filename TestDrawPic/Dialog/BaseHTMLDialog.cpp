// BaseHTMLDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BaseHTMLDialog.h"


// CBaseHTMLDialog �Ի���

IMPLEMENT_DYNCREATE(CBaseHTMLDialog, CDHtmlDialog)

CBaseHTMLDialog::CBaseHTMLDialog(UINT nIDTemplate, CWnd *pParentWnd)
	: CDHtmlDialog(nIDTemplate, 0, pParentWnd)
{
	EnableAutomation();
	m_bIsDrag = false;
	m_bReturnIsOK = FALSE;
	m_bParentNeedKey = false;
}

CBaseHTMLDialog::CBaseHTMLDialog():CDHtmlDialog()
{
	Initialize();
	m_bIsDrag = false;
	m_bReturnIsOK = FALSE;
	m_bParentNeedKey = false;
}

CBaseHTMLDialog::~CBaseHTMLDialog()
{
}

void CBaseHTMLDialog::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CDHtmlDialog::OnFinalRelease();
}

void CBaseHTMLDialog::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CBaseHTMLDialog::OnInitDialog()
{
	m_nHtmlResID = 0;
	CDHtmlDialog::OnInitDialog();
	m_pBrowserApp->put_Silent(VARIANT_TRUE);
	SetExternalDispatch(GetIDispatch(TRUE));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CBaseHTMLDialog, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CBaseHTMLDialog)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CBaseHTMLDialog, CDHtmlDialog)
	DISP_FUNCTION(CBaseHTMLDialog, "OKClose", OKClose, VT_EMPTY, VTS_NONE) 
	DISP_FUNCTION(CBaseHTMLDialog, "CancelClose", CancelClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CBaseHTMLDialog, "ChangeSize", ChangeSize, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CBaseHTMLDialog, "MovingWindow", MovingWindow, VT_EMPTY, VTS_NONE)
	//ÿ����������Ҫ���������ӳ��
END_DISPATCH_MAP()


// ע��: ������� IID_IBaseHTMLDialog ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {B12196B4-856C-4E75-93BA-06A3DD799D0B}

BEGIN_INTERFACE_MAP(CBaseHTMLDialog, CDHtmlDialog)
END_INTERFACE_MAP()


void CBaseHTMLDialog::OKClose()
{
	OnOK();
}

void CBaseHTMLDialog::CancelClose()
{
	OnCancel();
}

// CBaseHTMLDialog ��Ϣ�������


BOOL CBaseHTMLDialog::PreTranslateMessage(MSG* pMsg)
{
	if (!m_bReturnIsOK)
	{
		if (pMsg->message==WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:return TRUE;
			case VK_ESCAPE:return TRUE;
			}
		}
	}
	if (m_bIsDrag)
	{
		
	}
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

void CBaseHTMLDialog::ChangeSize( VARIANT& vtWidth, VARIANT& vtHeight )
{
	CComVariant varWidth(vtWidth);
	varWidth.ChangeType(VT_INT);
	CComVariant varHeight(vtHeight);
	varWidth.ChangeType(VT_INT);
	CRect rtMain;
	GetWindowRect(rtMain);
	rtMain.right = rtMain.left + vtWidth.intVal;
	rtMain.bottom = rtMain.top + vtHeight.intVal;
	CWnd* pWnd = GetParent();
	if (pWnd)
	{
		if (pWnd->GetSafeHwnd())
		{
			//this->ScreenToClient(rtMain);
		}
	}
	MoveWindow(rtMain);
}

void CBaseHTMLDialog::MovingWindow()
{
	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION);
	POINT pt; 
	GetCursorPos(&pt);
	POINTS pts = {(SHORT)pt.x, (SHORT)pt.y};

	::SendMessage(m_hWnd,WM_NCLBUTTONDOWN, HTCAPTION, (LPARAM)&pts);
	TRACE("xxx%d,%d\r\n", pts.x,pts.y);
}

HRESULT CBaseHTMLDialog::Submit()
{
	HRESULT hr;
	IHTMLDocument2* ppDoc;
	hr = this->GetDHtmlDocument(&ppDoc);
	if (S_OK!=hr)
	{
		return S_FALSE;
	}
	CComQIPtr<IHTMLElementCollection>spElementCollection; 
	hr = ppDoc->get_forms( &spElementCollection );
	if (S_OK!=hr)
	{
		return S_FALSE;
	}
	long nCount = 0;
	hr = spElementCollection->get_length(&nCount);
	if (S_OK!=hr)
	{
		return S_FALSE;
	}
	for (int i=0; i< nCount; i++)
	{
		IDispatch *pDisp = NULL;
		hr = spElementCollection->item( CComVariant( i ), CComVariant(), &pDisp );
		if ( FAILED( hr ) )        continue;

		CComQIPtr<IHTMLFormElement> pForm = pDisp;
		hr = pForm->submit();
		if (hr==S_OK)
		{
			return S_OK;
		}
	}
	return S_FALSE;
}


LRESULT CBaseHTMLDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_bParentNeedKey)
	{
		if (message==WM_KEYDOWN || message==WM_KEYUP)
		{
			CWnd* pParent = GetParent();
			if (pParent)
			{
				::SendMessage(pParent->GetSafeHwnd(), message, wParam, lParam);
			}
		}
	}
	return CDHtmlDialog::DefWindowProc(message, wParam, lParam);
}
