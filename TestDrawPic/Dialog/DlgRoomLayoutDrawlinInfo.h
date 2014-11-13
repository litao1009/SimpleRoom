#pragma once
#include "afxwin.h"
#include "irrEngine/SRenderContextFwd.h"

// DlgRoomLayoutDrawlinInfo �Ի���

class DlgRoomLayoutDrawlinInfo : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRoomLayoutDrawlinInfo)

public:
	DlgRoomLayoutDrawlinInfo(const SRenderContextWPtr& rc, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgRoomLayoutDrawlinInfo();

// �Ի�������
	enum { IDD = IDD_DLG_ROOMLAYOUT_DRAWLINEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	HWND	GetTextHwnd() const { return EditNum_.GetSafeHwnd(); }

	bool	IsVisible() const { return Visible_; }

	void	SetVisible(bool val)
	{
		Visible_ = val;
		ShowWindow(val ? SW_SHOW : SW_HIDE);
	}

	void	SetNum(int num);

	void	Resize();

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();

private:

	CEdit				EditNum_;
	bool				Visible_;
	bool				OK_;
	bool				Inputting_;
	int					Result_;
	SRenderContextWPtr	RC_;
	CWnd*				Parent_;
public:
	afx_msg void OnEnChangeTxtNum();
};
