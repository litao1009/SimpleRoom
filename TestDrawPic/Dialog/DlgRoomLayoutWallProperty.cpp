// DlgRoomLayoutWallProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutWallProperty.h"
#include "afxdialogex.h"

#include "RenderController/UserEvent.h"

#include <string>

// DlgRoomLayoutWallProperty 对话框

class	DlgRoomLayoutWallProperty::Imp
{
public:

	Imp()
	{
		pWallInfo_ = nullptr;
	}

	SEventWallInfo*	pWallInfo_;
};

IMPLEMENT_DYNAMIC(DlgRoomLayoutWallProperty, CDialogEx)

DlgRoomLayoutWallProperty::DlgRoomLayoutWallProperty(int data, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutWallProperty::IDD, pParent), ImpUPtr_(new Imp)
{
	auto info = static_cast<SEventWallInfo*>(reinterpret_cast<void*>(data));
	ImpUPtr_->pWallInfo_ = info;

	HeightValid_ = true;
	ThicknessValid_ = true;
}

DlgRoomLayoutWallProperty::~DlgRoomLayoutWallProperty()
{
}

void DlgRoomLayoutWallProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, OKBtn_);
	DDX_Control(pDX, IDC_TXT_HEIGHT_NUM, HeightNum_);
	DDX_Control(pDX, IDC_TXT_THICKNESS_NUM, ThicknessNum_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutWallProperty, CDialogEx)
	ON_EN_CHANGE(IDC_TXT_THICKNESS_NUM, &DlgRoomLayoutWallProperty::OnEnChangeTxtThicknessNum)
	ON_EN_CHANGE(IDC_TXT_HEIGHT_NUM, &DlgRoomLayoutWallProperty::OnEnChangeTxtHeightNum)
	ON_BN_CLICKED(IDOK, &DlgRoomLayoutWallProperty::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgRoomLayoutWallProperty 消息处理程序

void DlgRoomLayoutWallProperty::OnEnChangeTxtThicknessNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString num;
	ThicknessNum_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		ThicknessValid_ = false;
	}
	else
	{
		if ( num.GetLength() > 3 )
		{
			num.Truncate(3);
			ThicknessNum_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		ThicknessValue_ = std::stoi(str);
		
		ThicknessValid_ = true;
	}

	OKBtn_.EnableWindow(HeightValid_ && ThicknessValid_);
}


void DlgRoomLayoutWallProperty::OnEnChangeTxtHeightNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString num;
	HeightNum_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		HeightValid_ = false;
	}
	else
	{
		if ( num.GetLength() > 4 )
		{
			num.Truncate(4);
			HeightNum_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		HeightValue_ = std::stoi(str);

		HeightValid_ = true;
	}

	OKBtn_.EnableWindow(HeightValid_ && ThicknessValid_);
}


void DlgRoomLayoutWallProperty::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	ImpUPtr_->pWallInfo_->Height_ = static_cast<float>(HeightValue_);
	ImpUPtr_->pWallInfo_->Thickness_ = static_cast<float>(ThicknessValue_);

	CDialogEx::OnOK();
}


BOOL DlgRoomLayoutWallProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HeightNum_.SetWindowText(std::to_wstring(ImpUPtr_->pWallInfo_->Height_).c_str());
	ThicknessNum_.SetWindowText(std::to_wstring(ImpUPtr_->pWallInfo_->Thickness_).c_str());
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
