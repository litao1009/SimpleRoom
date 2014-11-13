#pragma once
#include "afxwin.h"


// DlgRoomLayoutWallProperty 对话框

class DlgRoomLayoutWallProperty : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRoomLayoutWallProperty)

public:
	DlgRoomLayoutWallProperty(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgRoomLayoutWallProperty();

// 对话框数据
	enum { IDD = IDD_DLG_ROOMLAYOUT_WALLPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEnChangeTxtNum();

private:

	CEdit	Num_;
	CButton	OKBtn_;
	int		Result_;
};
