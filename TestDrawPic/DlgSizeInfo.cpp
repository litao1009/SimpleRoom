// DlgSizeInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgSizeInfo.h"
#include "afxdialogex.h"


// CDlgSizeInfo 对话框

IMPLEMENT_DYNAMIC(CDlgSizeInfo, CDialogEx)

CDlgSizeInfo::CDlgSizeInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSizeInfo::IDD, pParent)
{

}

CDlgSizeInfo::~CDlgSizeInfo()
{
}

void CDlgSizeInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_XLEN, m_txtXLen);
	DDX_Control(pDX, IDC_TXT_YLEN, m_txtYLen);
	DDX_Control(pDX, IDC_TXT_ZLEN, m_txtZLen);
	DDX_Control(pDX, IDC_TXT_YPOS, m_txtYPos);
}


BEGIN_MESSAGE_MAP(CDlgSizeInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSizeInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSizeInfo 消息处理程序


void CDlgSizeInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fXLen = m_txtXLen.GetNumber();
	m_fYLen = m_txtYLen.GetNumber();
	m_fZLen = m_txtZLen.GetNumber();
	m_fYPos = m_txtYPos.GetNumber();
	CDialogEx::OnOK();
}


BOOL CDlgSizeInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_txtXLen.InitSpinButton();
	m_txtYLen.InitSpinButton();
	m_txtZLen.InitSpinButton();
	m_txtYPos.InitSpinButton();

	m_txtXLen.SetNumber(m_fXLen);
	m_txtYLen.SetNumber(m_fYLen);
	m_txtZLen.SetNumber(m_fZLen);
	m_txtYPos.SetNumber(m_fYPos);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
