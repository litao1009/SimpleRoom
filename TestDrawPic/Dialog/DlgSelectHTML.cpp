// DlgSelect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSelectHTML.h"
#include "MainFrm.h"

#include <boost/filesystem.hpp>
// CDlgSelectHTML �Ի���

IMPLEMENT_DYNCREATE(CDlgSelectHTML, CBaseHTMLDialog)

CDlgSelectHTML::CDlgSelectHTML(CWnd* pParent /*=NULL*/)
	: CBaseHTMLDialog(CDlgSelectHTML::IDD, pParent)
{

}

CDlgSelectHTML::~CDlgSelectHTML()
{
}

void CDlgSelectHTML::DoDataExchange(CDataExchange* pDX)
{
	CBaseHTMLDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_SPLASH, m_lblSplash);
}

BOOL CDlgSelectHTML::OnInitDialog()
{
	auto curDir = boost::filesystem::current_path();
	
	curDir = curDir.parent_path() / "Data\\Resource\\HTML\\roomlayoutlist.html";
	m_strCurrentUrl = curDir.wstring().c_str();

	CBaseHTMLDialog::OnInitDialog();
	SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR);
	CRect rtMain;
	GetClientRect(rtMain);
	m_lblSplash.MoveWindow(rtMain);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CDlgSelectHTML, CBaseHTMLDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgSelectHTML)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CDlgSelectHTML, CBaseHTMLDialog)
	DISP_FUNCTION(CDlgSelectHTML, "SendObj", SendObj, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CDlgSelectHTML, "OnDrawWall", OnDrawWall, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDlgSelectHTML, "OnDropDoor", OnDropDoor, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDlgSelectHTML, "OnDropBeam", OnDropBeam, VT_EMPTY, VTS_NONE)
	//ÿ����������Ҫ���������ӳ��
END_DISPATCH_MAP()

// CDlgSelectHTML ��Ϣ�������

HRESULT CDlgSelectHTML::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDlgSelectHTML::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


void CDlgSelectHTML::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CBaseHTMLDialog::OnDocumentComplete(pDisp, szUrl);
	m_lblSplash.ShowWindow(SW_HIDE);
}

void CDlgSelectHTML::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CBaseHTMLDialog::OnNavigateComplete(pDisp, szUrl);
	CComPtr<IHTMLElement> spElement;
	IHTMLDocument2 * pDoc;
	GetDHtmlDocument(&pDoc);
	pDoc->get_body(&spElement);
	if (NULL == spElement)
		return;

	CComPtr<IHTMLBodyElement> spBodyElement;
	spElement->QueryInterface(__uuidof(IHTMLBodyElement), (void**)&spBodyElement);
	if (NULL == spBodyElement)
		return;

	spBodyElement->put_scroll(_bstr_t(_T("auto")));
	// TODO: �ڴ����ר�ô����/����û���
}

void CDlgSelectHTML::OnDrawWall()
{

}

void CDlgSelectHTML::OnDropDoor()
{

}

void CDlgSelectHTML::OnDropBeam()
{

}

void CDlgSelectHTML::SendObj( VARIANT& vtGTypeID, VARIANT& vtObjID )
{
	CComVariant cbGTypeID(vtGTypeID);
	CComVariant cbObjID(vtObjID);
	cbGTypeID.ChangeType(VT_INT);
	cbObjID.ChangeType(VT_INT);
	CString strFormat;
	strFormat.Format(_T("%d,%d"), cbObjID.intVal, cbGTypeID.intVal);
	CFrameWnd* pFrame = GetParentFrame();
	if ((pFrame) && (pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame))))
	{
		//��ȡ��ǰ���õ�View�����ö�������
	}
}

void CDlgSelectHTML::RefreshWindow()
{
	if (this->m_pBrowserApp)
	{
		this->m_pBrowserApp->Refresh();
	}
}
