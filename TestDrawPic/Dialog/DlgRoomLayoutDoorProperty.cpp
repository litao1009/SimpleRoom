// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutDoorProperty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutDoorProperty.h"
#include "afxdialogex.h"

#include "irrEngine/SRenderContext.h"
#include "RenderController/UserEvent.h"

class	DlgRoomLayoutDoorProperty::Imp
{
public:

	Imp()
	{
		pDoorInfo_ = nullptr;
		XValid_ = true;
		YValid_ = true;
		ZValid_ = true;
	}

public:

	SEventDoorInfo*		pDoorInfo_;
	SEventDoorInfo		TmpDoorInfo_;
	SRenderContextSPtr	RC_;
	POINT				InitPoint_;
	bool				XValid_,YValid_,ZValid_;
};

// DlgRoomLayoutDoorProperty �Ի���

IMPLEMENT_DYNAMIC(DlgRoomLayoutDoorProperty, CDialogEx)

DlgRoomLayoutDoorProperty::DlgRoomLayoutDoorProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutDoorProperty::IDD, pParent), ImpUPtr_(new Imp)
{
	auto info = static_cast<SEventDoorInfo*>(reinterpret_cast<void*>(data));
	ImpUPtr_->pDoorInfo_ = info;
	ImpUPtr_->TmpDoorInfo_ = *info;
	ImpUPtr_->RC_ = rc;
}

DlgRoomLayoutDoorProperty::~DlgRoomLayoutDoorProperty()
{
}

void DlgRoomLayoutDoorProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BTN_CANCEL, BtnCancel_);
	DDX_Control(pDX, ID_BTN_OK, BtnOK_);
	DDX_Control(pDX, IDC_BTN_DELETE, BtnDelete_);
	DDX_Control(pDX, IDC_BTN_MOVE, BtnMove_);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, EditHeight_);
	DDX_Control(pDX, IDC_EDIT_WIDTH, EditWidth_);
	DDX_Control(pDX, IDC_EDIT_DEPTH, EditDepth_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutDoorProperty, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &DlgRoomLayoutDoorProperty::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &DlgRoomLayoutDoorProperty::OnEnChangeEditHeight)
	ON_BN_CLICKED(IDC_BTN_MOVE, &DlgRoomLayoutDoorProperty::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_DELETE, &DlgRoomLayoutDoorProperty::OnBnClickedBtnDelete)
	ON_BN_CLICKED(ID_BTN_OK, &DlgRoomLayoutDoorProperty::OnBnClickedBtnOk)
	ON_BN_CLICKED(ID_BTN_CANCEL, &DlgRoomLayoutDoorProperty::OnBnClickedBtnCancel)
	ON_EN_CHANGE(IDC_EDIT_DEPTH, &DlgRoomLayoutDoorProperty::OnEnChangeEditDepth)
END_MESSAGE_MAP()


// DlgRoomLayoutDoorProperty ��Ϣ�������


void DlgRoomLayoutDoorProperty::OnEnChangeEditWidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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
		imp_.TmpDoorInfo_.XLength_ = static_cast<float>(std::stoi(str));

		imp_.XValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ );
}


void DlgRoomLayoutDoorProperty::OnEnChangeEditHeight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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
		imp_.TmpDoorInfo_.YLength_ = static_cast<float>(std::stoi(str));

		imp_.YValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ );
}


void DlgRoomLayoutDoorProperty::OnEnChangeEditDepth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		imp_.TmpDoorInfo_.ZLength_ = static_cast<float>(std::stoi(str));

		imp_.ZValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ );
}


void DlgRoomLayoutDoorProperty::OnBnClickedBtnMove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetCursorPos(ImpUPtr_->InitPoint_.x, ImpUPtr_->InitPoint_.y);

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_DOOR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_DOOR_MOVE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutDoorProperty::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_DOOR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_DOOR_DELETE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutDoorProperty::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*(ImpUPtr_->pDoorInfo_) = ImpUPtr_->TmpDoorInfo_;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_DOOR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_DOOR_UPDATE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutDoorProperty::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_DOOR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_DOOR_NONE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


BOOL DlgRoomLayoutDoorProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	EditWidth_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pDoorInfo_->XLength_)).c_str());
	EditHeight_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pDoorInfo_->YLength_)).c_str());
	EditDepth_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pDoorInfo_->ZLength_)).c_str());

	RECT r;
	GetClientRect(&r);

	auto width = r.right - r.left;
	auto height = r.bottom - r.top;

	GetCursorPos(&ImpUPtr_->InitPoint_);

	MoveWindow(ImpUPtr_->InitPoint_.x-width/2, ImpUPtr_->InitPoint_.y-height/2, width, height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL DlgRoomLayoutDoorProperty::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
