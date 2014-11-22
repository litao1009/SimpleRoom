// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutWindowProperty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutWindowProperty.h"
#include "afxdialogex.h"

#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"
#include "RenderController/UserEvent.h"

class	DlgRoomLayoutWindowProperty::Imp
{
public:

	Imp()
	{
		pWindowInfo_ = nullptr;
		XValid_ = true;
		YValid_ = true;
		ZValid_ = true;
		OffsetHeightValid_ = true;
	}

public:

	SEventWindowInfo*	pWindowInfo_;
	SEventWindowInfo	TmpWindowInfo_;
	SRenderContextSPtr	RC_;
	POINT				InitPoint_;
	bool				XValid_,YValid_,ZValid_, OffsetHeightValid_;
};


// DlgRoomLayoutWindowProperty �Ի���

IMPLEMENT_DYNAMIC(DlgRoomLayoutWindowProperty, CDialogEx)

DlgRoomLayoutWindowProperty::DlgRoomLayoutWindowProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutWindowProperty::IDD, pParent), ImpUPtr_(new Imp)
{
	auto info = static_cast<SEventWindowInfo*>(reinterpret_cast<void*>(data));
	ImpUPtr_->pWindowInfo_ = info;
	ImpUPtr_->TmpWindowInfo_ = *info;
	ImpUPtr_->RC_ = rc;
}

DlgRoomLayoutWindowProperty::~DlgRoomLayoutWindowProperty()
{
}

void DlgRoomLayoutWindowProperty::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(DlgRoomLayoutWindowProperty, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &DlgRoomLayoutWindowProperty::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &DlgRoomLayoutWindowProperty::OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_DEPTH, &DlgRoomLayoutWindowProperty::OnEnChangeEditDepth)
	ON_EN_CHANGE(IDC_EDIT_OFFSETHEIGHT, &DlgRoomLayoutWindowProperty::OnEnChangeEditOffsetheight)
	ON_BN_CLICKED(IDC_BTN_MOVE, &DlgRoomLayoutWindowProperty::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_DELETE, &DlgRoomLayoutWindowProperty::OnBnClickedBtnDelete)
	ON_BN_CLICKED(ID_BTN_OK, &DlgRoomLayoutWindowProperty::OnBnClickedBtnOk)
	ON_BN_CLICKED(ID_BTN_CANCEL, &DlgRoomLayoutWindowProperty::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// DlgRoomLayoutWindowProperty ��Ϣ�������


BOOL DlgRoomLayoutWindowProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	EditWidth_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pWindowInfo_->XLength_)).c_str());
	EditHeight_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pWindowInfo_->YLength_)).c_str());
	EditDepth_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pWindowInfo_->ZLength_)).c_str());
	EditOffsetHeight_.SetWindowText(std::to_wstring(static_cast<int>(ImpUPtr_->pWindowInfo_->OffsetHeight_)).c_str());

	RECT r;
	GetClientRect(&r);

	auto width = r.right - r.left;
	auto height = r.bottom - r.top;

	GetCursorPos(&ImpUPtr_->InitPoint_);

	MoveWindow(ImpUPtr_->InitPoint_.x-width/2, ImpUPtr_->InitPoint_.y-height/2, width, height);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void DlgRoomLayoutWindowProperty::OnEnChangeEditWidth()
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
		imp_.TmpWindowInfo_.XLength_ = static_cast<float>(std::stoi(str));

		imp_.XValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetHeightValid_ );
}


void DlgRoomLayoutWindowProperty::OnEnChangeEditHeight()
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
		imp_.TmpWindowInfo_.YLength_ = static_cast<float>(std::stoi(str));

		imp_.YValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetHeightValid_ );
}


void DlgRoomLayoutWindowProperty::OnEnChangeEditDepth()
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
		imp_.TmpWindowInfo_.ZLength_ = static_cast<float>(std::stoi(str));

		imp_.ZValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetHeightValid_ );
}


void DlgRoomLayoutWindowProperty::OnEnChangeEditOffsetheight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	auto& imp_ = *ImpUPtr_;

	CString num;
	EditOffsetHeight_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		imp_.OffsetHeightValid_ = false;
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
		imp_.TmpWindowInfo_.OffsetHeight_ = static_cast<float>(std::stoi(str));

		imp_.OffsetHeightValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetHeightValid_ );
}


void DlgRoomLayoutWindowProperty::OnBnClickedBtnMove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetCursorPos(ImpUPtr_->InitPoint_.x, ImpUPtr_->InitPoint_.y);

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WINDOW_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WINDOW_MOVE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutWindowProperty::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WINDOW_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WINDOW_DELETE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutWindowProperty::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*(ImpUPtr_->pWindowInfo_) = ImpUPtr_->TmpWindowInfo_;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WINDOW_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WINDOW_UPDATE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutWindowProperty::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_WINDOW_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_WINDOW_NONE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}
