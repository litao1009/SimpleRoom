// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgPicRefSize.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgPicRefSize.h"
#include "afxdialogex.h"

#include <string>

// DlgPicRefSize 对话框

IMPLEMENT_DYNAMIC(DlgPicRefSize, CDialogEx)

DlgPicRefSize::DlgPicRefSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgPicRefSize::IDD, pParent)
{
	Result_ = 0;
}

DlgPicRefSize::~DlgPicRefSize()
{
}

void DlgPicRefSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_PIC_REF_SIZE_NUM, NumText_);
	DDX_Control(pDX, IDOK, OKBtn_);
}

BOOL DlgPicRefSize::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message==WM_KEYDOWN || pMsg->message == WM_KEYUP )
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:return TRUE;
		case VK_ESCAPE:return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(DlgPicRefSize, CDialogEx)
	ON_EN_CHANGE(IDC_TXT_PIC_REF_SIZE_NUM, &DlgPicRefSize::OnEnChangeTxtPicRefSizeNum)
END_MESSAGE_MAP()

unsigned DlgPicRefSize::GetNum() const
{
	return Result_;
}

// DlgPicRefSize 消息处理程序


void DlgPicRefSize::OnEnChangeTxtPicRefSizeNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString num;
	NumText_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		OKBtn_.EnableWindow(FALSE);
	}
	else
	{
		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		Result_= std::stoi(str);

		OKBtn_.EnableWindow(TRUE);
	}
}


BOOL DlgPicRefSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OKBtn_.EnableWindow(FALSE);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
