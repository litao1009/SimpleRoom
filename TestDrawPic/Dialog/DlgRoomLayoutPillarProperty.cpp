// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutPillarProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutPillarProperty.h"
#include "afxdialogex.h"

#include "irrEngine/SRenderContext.h"
#include "RenderController/UserEvent.h"

// DlgRoomLayoutPillarProperty 对话框

class	DlgRoomLayoutPillarProperty::Imp
{
public:

	Imp()
	{
		pPillarInfo_ = nullptr;
		XValid_ = true;
		YValid_ = true;
		ZValid_ = true;
		OffsetValid_ = true;
	}

public:

	SEventPillarInfo*	pPillarInfo_;
	SEventPillarInfo	TmpPillarInfo_;
	SRenderContextSPtr	RC_;
	POINT				InitPoint_;
	bool				XValid_,YValid_,ZValid_,OffsetValid_;
};

IMPLEMENT_DYNAMIC(DlgRoomLayoutPillarProperty, CDialogEx)

DlgRoomLayoutPillarProperty::DlgRoomLayoutPillarProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutPillarProperty::IDD, pParent), ImpUPtr_(new Imp)
{
	auto info = static_cast<SEventPillarInfo*>(reinterpret_cast<void*>(data));
	ImpUPtr_->pPillarInfo_ = info;
	ImpUPtr_->TmpPillarInfo_ = *info;
	ImpUPtr_->RC_ = rc;
}

DlgRoomLayoutPillarProperty::~DlgRoomLayoutPillarProperty()
{
}

void DlgRoomLayoutPillarProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BTN_CANCEL, BtnCancel_);
	DDX_Control(pDX, ID_BTN_OK, BtnOK_);
	DDX_Control(pDX, IDC_BTN_DELETE, BtnDelete_);
	DDX_Control(pDX, IDC_BTN_MOVE, BtnMove_);
	DDX_Control(pDX, IDC_EDIT_DEPTH, EditDepth_);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, EditHeight_);
	DDX_Control(pDX, IDC_EDIT_OFFSETHEIGHT, EditOffsetHeight_);
	DDX_Control(pDX, IDC_EDIT_WIDTH, EditWidth_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutPillarProperty, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &DlgRoomLayoutPillarProperty::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &DlgRoomLayoutPillarProperty::OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_DEPTH, &DlgRoomLayoutPillarProperty::OnEnChangeEditDepth)
	ON_EN_CHANGE(IDC_EDIT_OFFSETHEIGHT, &DlgRoomLayoutPillarProperty::OnEnChangeEditOffsetheight)
	ON_BN_CLICKED(IDC_BTN_MOVE, &DlgRoomLayoutPillarProperty::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_DELETE, &DlgRoomLayoutPillarProperty::OnBnClickedBtnDelete)
	ON_BN_CLICKED(ID_BTN_OK, &DlgRoomLayoutPillarProperty::OnBnClickedBtnOk)
	ON_BN_CLICKED(ID_BTN_CANCEL, &DlgRoomLayoutPillarProperty::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// DlgRoomLayoutPillarProperty 消息处理程序


BOOL DlgRoomLayoutPillarProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	EditWidth_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pPillarInfo_->XLength_)).c_str());
	EditHeight_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pPillarInfo_->YLength_)).c_str());
	EditDepth_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pPillarInfo_->ZLength_)).c_str());
	EditOffsetHeight_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pPillarInfo_->OffsetHeight_)).c_str());

	RECT r;
	GetClientRect(&r);

	auto width = r.right - r.left;
	auto height = r.bottom - r.top;

	GetCursorPos(&ImpUPtr_->InitPoint_);

	MoveWindow(ImpUPtr_->InitPoint_.x-width/2, ImpUPtr_->InitPoint_.y-height/2, width, height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	auto& imp_ = *ImpUPtr_;

	CString num;
	EditWidth_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		imp_.XValid_ = false;
	}
	else
	{
		if ( num.GetLength() > 4 )
		{
			num.Truncate(4);
			EditWidth_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		imp_.TmpPillarInfo_.XLength_ = static_cast<float>(std::stoi(str));

		imp_.XValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditHeight()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	auto& imp_ = *ImpUPtr_;

	CString num;
	EditHeight_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		imp_.YValid_ = false;
	}
	else
	{
		if ( num.GetLength() > 4 )
		{
			num.Truncate(4);
			EditHeight_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		imp_.TmpPillarInfo_.YLength_ = static_cast<float>(std::stoi(str));

		imp_.YValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditDepth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	auto& imp_ = *ImpUPtr_;

	CString num;
	EditDepth_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		imp_.ZValid_ = false;
	}
	else
	{
		if ( num.GetLength() > 4 )
		{
			num.Truncate(4);
			EditDepth_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		imp_.TmpPillarInfo_.ZLength_ = static_cast<float>(std::stoi(str));

		imp_.ZValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditOffsetheight()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	auto& imp_ = *ImpUPtr_;

	CString num;
	EditOffsetHeight_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		imp_.OffsetValid_ = false;
	}
	else
	{
		if ( num.GetLength() > 4 )
		{
			num.Truncate(4);
			EditOffsetHeight_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		imp_.TmpPillarInfo_.OffsetHeight_ = static_cast<float>(std::stoi(str));

		imp_.OffsetValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnBnClickedBtnMove()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCursorPos(ImpUPtr_->InitPoint_.x, ImpUPtr_->InitPoint_.y);

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PILLAR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_PILLAR_MOVE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutPillarProperty::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PILLAR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_PILLAR_DELETE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutPillarProperty::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	*(ImpUPtr_->pPillarInfo_) = ImpUPtr_->TmpPillarInfo_;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PILLAR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_PILLAR_UPDATE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutPillarProperty::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PILLAR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_PILLAR_NONE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


BOOL DlgRoomLayoutPillarProperty::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			OnBnClickedBtnOk();
			return TRUE;
		}

		if ( pMsg->wParam == VK_ESCAPE )
		{
			OnBnClickedBtnCancel();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
