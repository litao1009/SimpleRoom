#pragma once


// CBaseDialog �Ի���
#include <string>
#include <gdiplus.h>

class CBaseDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDialog)

public:
	CBaseDialog(UINT nIDTemplate, CWnd *pParent = NULL);   // ��׼���캯��
	virtual ~CBaseDialog();


	INT CreateImage(const TCHAR* szPicPath, INT nImgWidth, INT nImgHeight,  CImageList& imgList);
	void CreateImage(const TCHAR* szPicPath, INT nImgWidth, INT nImgHeight, CBitmap& bmpDefault);

	INT CreateImage(const std::string& strData, INT nImgWidth, INT nImgHeight, CImageList& imgList);
	INT CreateImage(uint32_t uColor, INT nImgWidth, INT nImgHeight, CImageList& imgList);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
