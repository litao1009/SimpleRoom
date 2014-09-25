#pragma once


// CDlgRoomStyle 对话框
#include "BLL/RoomStyleBLL.h"
#include "ShowMgr/DrawImageEngine.h"
#include "afxcmn.h"

class CDlgRoomStyle : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRoomStyle)

public:
	CDlgRoomStyle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRoomStyle();
public:
	void GetPointList(std::vector<Gdiplus::PointF>& arrPoint);
private:
	INT m_nIndex;
	CRoomStyleBLLSPtr m_spRoomStyle;
	CImageList m_ImageList;
	CBitmap* m_pDefaultBitmap;
	INT m_nImgWidth;
	INT m_nImgHeight;
public:
// 对话框数据
	enum { IDD = IDD_DLG_ROOM_STYLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstRoomStyle;
	afx_msg void OnBnClickedOk();
};
