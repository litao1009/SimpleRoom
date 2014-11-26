#pragma once
#include "afxwin.h"

#include "irrEngine/SRenderContextFwd.h"

// DlgRoomLayoutWallProperty 对话框

class DlgRoomLayoutWallProperty : public CDialogEx
{
	class Imp;
	std::unique_ptr<Imp> ImpUPtr_;

	DECLARE_DYNAMIC(DlgRoomLayoutWallProperty)

public:
	DlgRoomLayoutWallProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgRoomLayoutWallProperty();

// 对话框数据
	enum { IDD = IDD_DLG_ROOMLAYOUT_WALLPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

private:

	CButton	OKBtn_;
	int		HeightValue_;
	int		ThicknessValue_;
	bool	HeightValid_;
	bool	ThicknessValid_;
public:
	CEdit HeightNum_;
	CEdit ThicknessNum_;
	afx_msg void OnEnChangeTxtThicknessNum();
	afx_msg void OnEnChangeTxtHeightNum();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
