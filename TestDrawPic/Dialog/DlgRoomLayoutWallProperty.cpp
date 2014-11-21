// DlgRoomLayoutWallProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutWallProperty.h"
#include "afxdialogex.h"

#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"

#include <string>

// DlgRoomLayoutWallProperty 对话框

class	DlgRoomLayoutWallProperty::Imp
{
public:

	Imp()
	{
		pWallInfo_ = nullptr;
	}

	SEventWallInfo*		pWallInfo_;
	SRenderContextSPtr	RC_;
};

IMPLEMENT_DYNAMIC(DlgRoomLayoutWallProperty, CDialogEx)

DlgRoomLayoutWallProperty::DlgRoomLayoutWallProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutWallProperty::IDD, pParent), ImpUPtr_(new Imp)
{
	auto info = static_cast<SEventWallInfo*>(reinterpret_cast<void*>(data));
	ImpUPtr_->pWallInfo_ = info;
	ImpUPtr_->RC_ = rc;

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
	ON_BN_CLICKED(IDC_BTN_MOVE, &DlgRoomLayoutWallProperty::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_DELETE, &DlgRoomLayoutWallProperty::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDCANCEL, &DlgRoomLayoutWallProperty::OnBnClickedCancel)
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

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WALL_PROPERTY;
	

	auto newHeight = static_cast<float>(HeightValue_);
	auto newThickness = static_cast<float>(ThicknessValue_);

	if ( std::abs(newHeight - ImpUPtr_->pWallInfo_->Height_) > 1 ||
		std::abs(newThickness - ImpUPtr_->pWallInfo_->Thickness_) > 1)
	{
		ImpUPtr_->pWallInfo_->Height_ = static_cast<float>(HeightValue_);
		ImpUPtr_->pWallInfo_->Thickness_ = static_cast<float>(ThicknessValue_);
		evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WALL_UPDATE;
	}
	else
	{
		evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WALL_NONE;
	}
	
	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


BOOL DlgRoomLayoutWallProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HeightNum_.SetWindowText(std::to_wstring(ImpUPtr_->pWallInfo_->Height_).c_str());
	ThicknessNum_.SetWindowText(std::to_wstring(ImpUPtr_->pWallInfo_->Thickness_).c_str());
	// TODO:  在此添加额外的初始化

	RECT r;
	GetClientRect(&r);

	auto width = r.right - r.left;
	auto height = r.bottom - r.top;
	POINT p;
	GetCursorPos(&p);
	this->ScreenToClient(&r);

	MoveWindow(p.x, p.y, width, height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DlgRoomLayoutWallProperty::OnBnClickedBtnMove()
{
	// TODO: 在此添加控件通知处理程序代码
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WALL_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WALL_MOVE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutWallProperty::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WALL_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WALL_DELETE;

	ImpUPtr_->RC_->PostEvent(evt);
	CDialogEx::OnOK();
}


void DlgRoomLayoutWallProperty::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WALL_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WALL_NONE;

	ImpUPtr_->RC_->PostEvent(evt);
	CDialogEx::OnCancel();
}
