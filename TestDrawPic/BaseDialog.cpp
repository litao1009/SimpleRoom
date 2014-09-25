// BaseDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "BaseDialog.h"
#include "afxdialogex.h"


// CBaseDialog 对话框

IMPLEMENT_DYNAMIC(CBaseDialog, CDialogEx)

CBaseDialog::CBaseDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialogEx(nIDTemplate, pParent)
{

}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDialog, CDialogEx)
END_MESSAGE_MAP()


// CBaseDialog 消息处理程序

INT CBaseDialog::CreateImage(const TCHAR* szPicPath, INT nImgWidth, INT nImgHeight, CImageList& imgList)
{
	if (_tcslen(szPicPath)<=0)
	{
		return -1;
	}
	HBITMAP m_hBitmap=NULL;
	
	Gdiplus::Bitmap bmp(szPicPath);
	Gdiplus::Bitmap* pThumbnail = (Gdiplus::Bitmap*)bmp.GetThumbnailImage(nImgWidth, nImgHeight, NULL, NULL);

	pThumbnail->GetHBITMAP(Gdiplus::Color(0,0,0), &m_hBitmap);
	CBitmap *pBitmap=new CBitmap();
	pBitmap->Attach(m_hBitmap);
	INT nImgIndex = imgList.Add(pBitmap,RGB(120,120,120));
	pBitmap->Detach();

	DeleteObject(m_hBitmap);
	delete pBitmap;
	pBitmap = nullptr;
	return nImgIndex;

}

void CBaseDialog::CreateImage(const TCHAR* szPicPath, INT nImgWidth, INT nImgHeight, CBitmap& bmpDefault)
{
	if (_tcslen(szPicPath)<=0)
	{
		return;
	}
	HBITMAP m_hBitmap=NULL;
	Gdiplus::Bitmap bmp(szPicPath);
	Gdiplus::Bitmap* pThumbnail = (Gdiplus::Bitmap*)bmp.GetThumbnailImage(nImgWidth, nImgHeight, NULL, NULL);

	pThumbnail->GetHBITMAP(Gdiplus::Color(0,0,0), &m_hBitmap);
	bmpDefault.Attach(m_hBitmap);
}

INT CBaseDialog::CreateImage( const std::string& strData, INT nImgWidth, INT nImgHeight, CImageList& imgList)
{
	size_t sz=strData.size();
	if (strData.empty())
	{
		return -1;
	}
	const BYTE* pData = reinterpret_cast<const BYTE*>(strData.data());
	HGLOBAL hGal=GlobalAlloc(GMEM_MOVEABLE, sz);
	memcpy((void*)GlobalLock(hGal), pData, sz);
	GlobalUnlock(hGal);

	IStream* pStream=NULL;
	CreateStreamOnHGlobal(hGal, TRUE, &pStream);
	Gdiplus::Bitmap bmp(pStream);
	Gdiplus::Bitmap* pThumbnail = (Gdiplus::Bitmap*)bmp.GetThumbnailImage(nImgWidth, nImgHeight, NULL, NULL);

	HBITMAP m_hBitmap=NULL;
	pThumbnail->GetHBITMAP(Gdiplus::Color(0,0,0), &m_hBitmap);
	CBitmap *pBitmap=new CBitmap();
	pBitmap->Attach(m_hBitmap);
	INT nImgIndex = imgList.Add(pBitmap,RGB(120,120,120));
	pBitmap->Detach();

	GlobalFree(hGal);
	hGal=NULL;
	DeleteObject(m_hBitmap);
	delete pBitmap;
	pBitmap = nullptr;
	return nImgIndex;
}

INT CBaseDialog::CreateImage( uint32_t uColor, INT nImgWidth, INT nImgHeight, CImageList& imgList)
{
	BYTE b= uColor & 0x000000FF;
	BYTE g= (uColor & 0x0000FF00) >> 8;
	BYTE r= (uColor & 0x00FF0000) >> 16;

	Gdiplus::Color cl(r,g,b);

	Gdiplus::Bitmap bmp(nImgWidth, nImgHeight);
	for (INT i=0; i<nImgWidth; i++)
	{
		for (int j = 0; j < nImgHeight; j++)
		{
			bmp.SetPixel(i, j, cl);
		}
	}

	Gdiplus::Bitmap* pThumbnail = (Gdiplus::Bitmap*)bmp.GetThumbnailImage(nImgWidth, nImgHeight, NULL, NULL);

	HBITMAP m_hBitmap=NULL;
	pThumbnail->GetHBITMAP(Gdiplus::Color(0,0,0), &m_hBitmap);
	CBitmap *pBitmap=new CBitmap();
	pBitmap->Attach(m_hBitmap);
	INT nImgIndex = imgList.Add(pBitmap,RGB(120,120,120));
	pBitmap->Detach();

	DeleteObject(m_hBitmap);
	delete pBitmap;
	pBitmap=nullptr;
	return nImgIndex;
}
