#pragma once


// CDragDataListCtrl

class CDragDataListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CDragDataListCtrl)

public:
	CDragDataListCtrl();
	virtual ~CDragDataListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


