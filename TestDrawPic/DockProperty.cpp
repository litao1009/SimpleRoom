// DockProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DockProperty.h"


// CDockProperty
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDockProperty, CDockablePane)

CDockProperty::CDockProperty()
{

}

CDockProperty::~CDockProperty()
{
	m_propObj->DestroyWindow();
}


BEGIN_MESSAGE_MAP(CDockProperty, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDockProperty 消息处理程序


int CDockProperty::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rt;
	GetWindowRect(rt);
	m_propObj = std::make_shared<CMFCPropertyGridCtrl>();
	if (!m_propObj->Create(WS_VISIBLE | WS_CHILD, rt, this, IDC_PROP_OBJ))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}
	::DeleteObject (m_fntPropList.Detach ());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont (&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics (info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect (&lf);

	m_propObj->SetFont(&m_fntPropList);
	m_propObj->EnableDescriptionArea();
	m_propObj->SetVSDotNetLook();
	m_propObj->MarkModifiedProperties();
	
	//m_dlgProperty.Create(CDlgProperty::IDD, this);
	CSize sSize;
	sSize.cx=270;
	sSize.cy=700;
	this->SetMinSize(sSize);

	m_propObj->AlwaysShowUserToolTip();

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("Appearance")));

	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t) false, _T("Specifies the dialog's font will be nonbold and controls will have a 3D border")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	apGroup1->AddSubItem(pProp);
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the dialog's title bar")));

	m_propObj->AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apSize(new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE));

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the dialog's height"));
	pProp->EnableSpinControl(TRUE, 0, 1000);
	apSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the dialog's width"));
	pProp->EnableSpinControl(TRUE, 1, 500);
	apSize->AddSubItem(pProp);

	m_propObj->AddProperty(apSize.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("Font")));

//	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial"));

	apGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the dialog")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the dialog uses MS Shell Dlg font")));

	m_propObj->AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("Misc")));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("IDD_ABOUT_BOX(dialog)"));
	pProp->Enable(FALSE);
	apGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), NULL, _T("Specifies the default dialog color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	apGroup3->AddSubItem(pColorProp);

	static TCHAR BASED_CODE szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	apGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the dialog icon")));

	apGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_propObj->AddProperty(apGroup3.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup4(new CMFCPropertyGridProperty(_T("Hierarchy")));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	apGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	apGroup4->Expand(FALSE);
	m_propObj->AddProperty(apGroup4.release());
	return 0;
}


void CDockProperty::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (!m_propObj->GetSafeHwnd())
	{
		return;
	}
	CRect rectClient;
	rectClient.SetRectEmpty();
	GetWindowRect(rectClient);
	ScreenToClient(rectClient);
	INT nInc=rectClient.right-rectClient.left;
	
	if (nInc<270)
	{
		nInc=270-nInc;
		rectClient.right+=nInc;
		MoveWindow(rectClient);
		return;
	}
	// TODO: 在此处添加消息处理程序代码
	rectClient.top=0;
	rectClient.left=0;
	m_propObj->MoveWindow(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), TRUE);
}


