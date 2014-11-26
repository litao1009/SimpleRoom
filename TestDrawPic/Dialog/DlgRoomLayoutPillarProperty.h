#pragma once
#include "afxwin.h"

#include "irrEngine/SRenderContextFwd.h"

#include <memory>

// DlgRoomLayoutPillarProperty �Ի���

class DlgRoomLayoutPillarProperty : public CDialogEx
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

	DECLARE_DYNAMIC(DlgRoomLayoutPillarProperty)

public:
	DlgRoomLayoutPillarProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgRoomLayoutPillarProperty();

// �Ի�������
	enum { IDD = IDD_DLG_ROOMLAYOUT_PILLAR_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CButton BtnCancel_;
	CButton BtnOK_;
	CButton BtnDelete_;
	CButton BtnMove_;
	CEdit EditDepth_;
	CEdit EditHeight_;
	CEdit EditOffsetHeight_;
	CEdit EditWidth_;
	
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditDepth();
	afx_msg void OnEnChangeEditOffsetheight();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
