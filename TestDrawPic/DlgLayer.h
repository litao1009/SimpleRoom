#pragma once
#include "afxcmn.h"


// CDlgLayer �Ի���
#include "ODL/BaseODL.h"

class CDlgLayer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLayer)
	
public:
	CDlgLayer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLayer();

// �Ի�������
	enum { IDD = IDD_DLG_LAYER };
public:
	void Refresh(CView* pActiveView, CBaseODLSPtr spObj);
private:
	void UpdateTree(HTREEITEM hItem, CBaseODLSPtr spSub);
	CBaseODLSPtr m_spRoot;
	
	CView* m_pActiveView;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeObj;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedTreeObj(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreeObj(NMHDR *pNMHDR, LRESULT *pResult);
};
