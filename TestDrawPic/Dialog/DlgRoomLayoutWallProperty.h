#pragma once
#include "afxwin.h"


// DlgRoomLayoutWallProperty �Ի���

class DlgRoomLayoutWallProperty : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRoomLayoutWallProperty)

public:
	DlgRoomLayoutWallProperty(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgRoomLayoutWallProperty();

// �Ի�������
	enum { IDD = IDD_DLG_ROOMLAYOUT_WALLPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEnChangeTxtNum();

private:

	CEdit	Num_;
	CButton	OKBtn_;
	int		Result_;
};
