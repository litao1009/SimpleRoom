// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutRoomProperty.cpp : ʵ���ļ�
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
// DlgRoomLayoutRoomProperty �Ի���

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


// DlgRoomLayoutRoomProperty ��Ϣ�������


BOOL DlgRoomLayoutRoomProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	EditName_.SetWindowText(ImpUPtr_->pRoomInfo_->Name_.c_str());
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	RECT r;
	GetClientRect(&r);

	auto width = r.right - r.left;
	auto height = r.bottom - r.top;

	GetCursorPos(&ImpUPtr_->InitPoint_);

	MoveWindow(ImpUPtr_->InitPoint_.x-width/2, ImpUPtr_->InitPoint_.y-height/2, width, height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void DlgRoomLayoutRoomProperty::OnEnChangeEditName()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_ROOM_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_ROOM_NONE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnCancel();
}
