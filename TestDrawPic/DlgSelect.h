#pragma once
#include "afxwin.h"
#include "afxcmn.h"

//////////////////////////////////////////////////////////////////////////
// CDlgSelect 对话框
//这个类拿来保存可以拖的对象
//////////////////////////////////////////////////////////////////////////
#include <vector>
#include "PublicDefine.h"
#include "./DAL/DataManager.h"
#include "BaseDialog.h"
#include "DragDataListCtrl.h"

class CDlgSelect : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSelect)
	
public:
	CDlgSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelect();

// 对话框数据
	enum { IDD = IDD_DLG_SELECT };
private:
	void InitObjList(ENUM_Group_Type eGTypeID);
	std::vector<std::tuple<ENUM_Group_Type, CString>> m_arrGType;
	std::vector<StObjInfo> m_arrObjList;
	CImageList m_imgList;
	CBitmap m_bmpDefaultBitmap;
	INT m_nImgWidth;
	INT m_nImgHeight;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbxGType;
	CDragDataListCtrl m_lstObj;
	CButton m_rdoEditZone;
	CButton m_rdoEditSkin;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCbxGtype();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRdoEditZone();
	afx_msg void OnBnClickedRdoEditSkin();
	
};
