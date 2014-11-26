// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutPillarProperty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutPillarProperty.h"
#include "afxdialogex.h"

#include "irrEngine/SRenderContext.h"
#include "RenderController/UserEvent.h"

// DlgRoomLayoutPillarProperty �Ի���

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


// DlgRoomLayoutPillarProperty ��Ϣ�������


BOOL DlgRoomLayoutPillarProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditWidth()
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
		imp_.TmpPillarInfo_.XLength_ = static_cast<float>(std::stoi(str));

		imp_.XValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditHeight()
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
		imp_.TmpPillarInfo_.YLength_ = static_cast<float>(std::stoi(str));

		imp_.YValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditDepth()
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
		imp_.TmpPillarInfo_.ZLength_ = static_cast<float>(std::stoi(str));

		imp_.ZValid_ = true;
	}

	BtnOK_.EnableWindow( imp_.XValid_ && imp_.YValid_ && imp_.ZValid_ && imp_.OffsetValid_);
}


void DlgRoomLayoutPillarProperty::OnEnChangeEditOffsetheight()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PILLAR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_PILLAR_DELETE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


void DlgRoomLayoutPillarProperty::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PILLAR_PROPERTY;
	evt.UserEvent.UserData2 = EUT_ROOMLAYOUT_PILLAR_NONE;

	ImpUPtr_->RC_->PostEvent(evt);

	CDialogEx::OnOK();
}


BOOL DlgRoomLayoutPillarProperty::PreTranslateMessage(MSG* pMsg)
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
