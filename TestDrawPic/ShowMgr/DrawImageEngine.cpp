#include "stdafx.h"
#include "DrawImageEngine.h"


CDrawImageEngine::CDrawImageEngine(void)
{
	m_hDC = nullptr;
	m_clPenColor = Gdiplus::Color::Black;
	m_clDotColor = Gdiplus::Color::Black;
}


CDrawImageEngine::~CDrawImageEngine(void)
{
}

CDrawImageEngine& CDrawImageEngine::GetInstance()
{
	static	CDrawImageEngine sInstance;
	return sInstance;
}

void CDrawImageEngine::Create( HDC hdc, Gdiplus::REAL fPageWidth, Gdiplus::REAL fPageHeight)
{
	if ( hdc )
	{
		m_hDC = hdc;
	}
	if (fPageWidth>100)
	{
		m_fWidth = fPageWidth;
	}
	if (fPageHeight>100)
	{
		m_fHeight = fPageHeight;
	}
}

INT CDrawImageEngine::CreateImage(CArray<Gdiplus::Point>& ptList, INT nImgWidth, INT nImgHeight, CImageList& imgList)
{
	Gdiplus::Bitmap bmp(nImgWidth, nImgHeight);
	//�����������Сλ��
	float x0=0;
	float y0=0;
	float x1=0;
	float y1=0;
	for (INT i=0; i<ptList.GetCount(); i++)
	{
		auto& curPnt = ptList[i];

		if (curPnt.X<x0)
		{
			x0=static_cast<float>(curPnt.X);
		}
		if (curPnt.X>x1)
		{
			x1=static_cast<float>(curPnt.X);
		}
		if (curPnt.Y<y0)
		{
			y0=static_cast<float>(curPnt.Y);
		}
		if (curPnt.Y>y1)
		{
			y1=static_cast<float>(curPnt.Y);
		}
	}
	float fXOffset = abs(x1-x0);
	if (fXOffset<=0.00001)
	{
		fXOffset = static_cast<float>(nImgWidth);
	}
	float fYOffset = abs(y1-y0);
	if (fYOffset<=0.00001)
	{
		fYOffset = static_cast<float>(nImgHeight);
	}
	fXOffset+=nImgWidth;
	fYOffset+=nImgHeight;
	Gdiplus::Graphics buffer(&bmp);//Gaphics���������ڴ滭��   
	buffer.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//SmoothingModeAntiAlias);
	buffer.ResetTransform();
	float m_ScaleX = nImgWidth / fXOffset;
	float m_ScaleY = nImgHeight / fYOffset;
	if (m_ScaleX<0.001)
	{
		m_ScaleX=0.9f;
	}
	if (m_ScaleY<0.001)
	{
		m_ScaleY =0.9f;
	}

	buffer.TranslateTransform((nImgWidth - fXOffset * m_ScaleX) /2, (nImgHeight - fYOffset * m_ScaleY)/2);
	buffer.ScaleTransform(m_ScaleX, m_ScaleY);
	buffer.Clear(Gdiplus::Color::AliceBlue);//����ڴ滭�����԰�ɫ��䣬��ȻҲ������������ɫ   
	{
		//OnDrawTop
		Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0));
		pen.SetDashStyle(Gdiplus::DashStyleSolid);
		buffer.DrawLines(&pen, ptList.GetData(), ptList.GetSize());
	}
	
	HBITMAP hBmp;
	bmp.GetHBITMAP(Gdiplus::Color::Black, &hBmp);
	CBitmap pmp;
	pmp.Attach(hBmp);
	INT nImgIndex = imgList.Add(&pmp,RGB(120,120,120));
	pmp.Detach();
	return nImgIndex;
}

Gdiplus::Bitmap* CDrawImageEngine::CreateImageData( INT nGroutX, INT nGroutY, INT nSrcWidth, INT nSrcHeight, Gdiplus::Color clBackground, CString strPicPath )
{
	//����Ŀ��ͼƬ�������ȡ��߶ȣ�ˮ��ۿ��
	Gdiplus::Bitmap *img=new Gdiplus::Bitmap(nSrcWidth + nGroutX * 2, nSrcHeight + nGroutY * 2);
	//�����������Сλ��
	Gdiplus::Bitmap bmp(strPicPath);
	Gdiplus::Graphics buffer(img);//Gaphics���������ڴ滭��   
	buffer.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//SmoothingModeAntiAlias);
	buffer.ResetTransform();
	buffer.Clear(clBackground);
	{
		Gdiplus::ImageAttributes attImage;
		attImage.SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY);
		
		Gdiplus::Rect rtDraw(nGroutX, nGroutY, nSrcWidth, nSrcHeight);
		//ͼƬԭʼ�ߴ���bmp�ڣ����� x,y�㣬 ��ҪԭʼͼƬ��λ���� srcX, srcY, Ŀ��ͼ�������С�� srcWidth, srcHeight
		buffer.DrawImage(&bmp, rtDraw, 0, 0, bmp.GetWidth(), bmp.GetHeight(), Gdiplus::Unit::UnitPixel, &attImage);
	}
	
	return img;
}
