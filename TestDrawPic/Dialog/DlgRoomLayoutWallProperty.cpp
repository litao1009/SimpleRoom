// DlgRoomLayoutWallProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutWallProperty.h"
#include "afxdialogex.h"

#include <string>

// DlgRoomLayoutWallProperty 对话框

IMPLEMENT_DYNAMIC(DlgRoomLayoutWallProperty, CDialogEx)

DlgRoomLayoutWallProperty::DlgRoomLayoutWallProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutWallProperty::IDD, pParent)
{
	Result_ = 0;
}

DlgRoomLayoutWallProperty::~DlgRoomLayoutWallProperty()
{
}

void DlgRoomLayoutWallProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_NUM, Num_);
	DDX_Control(pDX, IDOK, OKBtn_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutWallProperty, CDialogEx)
	ON_EN_CHANGE(IDC_TXT_NUM, &DlgRoomLayoutWallProperty::OnEnChangeTxtNum)
END_MESSAGE_MAP()


// DlgRoomLayoutWallProperty 消息处理程序


void DlgRoomLayoutWallProperty::OnEnChangeTxtNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString num;
	Num_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		OKBtn_.EnableWindow(FALSE);
	}
	else
	{
		if ( num.GetLength() > 3 )
		{
			num.Truncate(3);
			Num_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		Result_= std::stoi(str);

		OKBtn_.EnableWindow(TRUE);
	}
}
