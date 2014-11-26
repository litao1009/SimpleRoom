#pragma once

#include "irrEngine/SRenderContextFwd.h"
#include <memory>
#include "afxwin.h"
// DlgRoomLayoutRoomProperty 对话框

class DlgRoomLayoutRoomProperty : public CDialogEx
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

	DECLARE_DYNAMIC(DlgRoomLayoutRoomProperty)

public:
	DlgRoomLayoutRoomProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgRoomLayoutRoomProperty();

// 对话框数据
	enum { IDD = IDD_DLG_ROOMLAYOUT_ROOMPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton BtnCancel_;
	CButton BtnOK_;
	CEdit EditName_;
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
