// DlgZoneInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgZoneInfo.h"
#include "afxdialogex.h"


// CDlgZoneInfo 对话框

IMPLEMENT_DYNAMIC(CDlgZoneInfo, CDialogEx)

CDlgZoneInfo::CDlgZoneInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgZoneInfo::IDD, pParent)
{
	m_nGroutX = 0;
	m_nGroutY = 0;
	m_fRotate = 0.0f;
	m_clColor = RGB(0,0,0);
}

CDlgZoneInfo::~CDlgZoneInfo()
{
}

void CDlgZoneInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_GROUT_X, m_txtGroutX);
	DDX_Control(pDX, IDC_TXT_GROUT_Y, m_txtGroutY);
	DDX_Control(pDX, IDC_TXT_ROTATE, m_txtRotate);
	DDX_Control(pDX, IDC_CBX_COLOR, m_cbxColor);
	DDX_Control(pDX, IDC_CBX_WRAP_MODE, m_cbxWrapMode);
}


BEGIN_MESSAGE_MAP(CDlgZoneInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgZoneInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgZoneInfo 消息处理程序


void CDlgZoneInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_txtGroutX.GetNumber(m_nGroutX);
	m_txtGroutY.GetNumber(m_nGroutY);
	m_fRotate = m_txtRotate.GetNumber();
	m_clColor = m_cbxColor.GetColor();
	int nCurSel = m_cbxWrapMode.GetCurSel();
	if (nCurSel>=0)
	{
		m_nWrapMode = nCurSel;
	}
	CDialogEx::OnOK();
}


BOOL CDlgZoneInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_txtGroutX.InitSpinButton();
	m_txtGroutY.InitSpinButton();
	m_txtRotate.InitSpinButton();

	m_txtGroutX.SetRange(0.0f, 500.0f);
	m_txtGroutY.SetRange(0.0f, 500.0f);
	m_txtRotate.SetRange(-360.0f, 360.0f);
	m_txtGroutX.SetNumber(m_nGroutX);
	m_txtGroutY.SetNumber(m_nGroutY);
	m_txtRotate.SetNumber(m_fRotate);
	m_cbxColor.SetColor(m_clColor);
	TCHAR arrTitle[5][10]={_T("平铺"),_T("水平镜像"),_T("垂直镜像"),_T("横纵镜像"), _T("单个"),};
	for (int i=0;i<5;i++)
	{
		m_cbxWrapMode.AddString(arrTitle[i]);
	}
	m_cbxWrapMode.SetCurSel(m_nWrapMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
