// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutRoomProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutRoomProperty.h"
#include "afxdialogex.h"

#include "irrEngine/SRenderContext.h"
#include "RenderController/UserEvent.h"

class	DlgRoomLayoutRoomProperty::Imp
{
public:

	SEventRoomInfo*		pRoomInfo_;
	SRenderContextSPtr	RC_;
	POINT				InitPoint_;
};
// DlgRoomLayoutRoomProperty 对话框

IMPLEMENT_DYNAMIC(DlgRoomLayoutRoomProperty, CDialogEx)

DlgRoomLayoutRoomProperty::DlgRoomLayoutRoomProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutRoomProperty::IDD, pParent), ImpUPtr_(new Imp)
{
	auto info = static_cast<SEventRoomInfo*>(reinterpret_cast<void*>(data));
	ImpUPtr_->pRoomInfo_ = info;
	ImpUPtr_->RC_ = rc;
}

DlgRoomLayoutRoomProperty::~DlgRoomLayoutRoomProperty()
{
}

void DlgRoomLayoutRoomProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BTN_CANCEL, BtnCancel_);
	DDX_Control(pDX, ID_BTN_OK, BtnOK_);
	DDX_Control(pDX, IDC_EDIT_NAME, EditName_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutRoomProperty, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NAME, &DlgRoomLayoutRoomProperty::OnEnChangeEditName)
	ON_BN_CLICKED(ID_BTN_OK, &DlgRoomLayoutRoomProperty::OnBnClickedBtnOk)
	ON_BN_CLICKED(ID_BTN_CANCEL, &DlgRoomLayoutRoomProperty::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// DlgRoomLayoutRoomProperty 消息处理程序


BOOL DlgRoomLayoutRoomProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	EditName_.SetWindowText(ImpUPtr_->pRoomInfo_->Name_.c_str());
	// TODO:  在此添加额外的初始化

	RECT r;
	GetClientRect(&r);

	auto width = r.right - r.left;
	auto height = r.bottom - r.top;

	GetCursorPos(&ImpUPtr_->InitPoint_);

	MoveWindow(ImpUPtr_->InitPoint_.x-width/2, ImpUPtr_->InitPoint_.y-height/2, width, height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DlgRoomLayoutRoomProperty::OnEnChangeEditName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString name;
	EditName_.GetWindowText(name);

	if ( name.IsEmpty() )
	{
		BtnOK_.EnableWindow(FALSE);
	}
	else
	{
		BtnOK_.EnableWindow(TRUE);
	}
}


void DlgRoomLayoutRoomProperty::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCursorPos(ImpUPtr_->InitPoint_.x, ImpUPtr_->InitPoint_.y);

	CString name;
	EditName_.GetWindowText(name);

	ImpUPtr_->pRoomInfo_->Name_ = name.GetBuffer();
	name.ReleaseBuffer();

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_ROOM_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_ROOM_UPDATE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutRoomProperty::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_ROOM_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_ROOM_NONE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnCancel();
}
