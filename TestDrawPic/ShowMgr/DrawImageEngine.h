#pragma once
//////////////////////////////////////////////////////////////////////////
//这个类拿来画窗口里边或者列表里边的静态对象，例如：图片、只需显示的形状
//////////////////////////////////////////////////////////////////////////
#include "ODL/BaseODL.h"
#include <gdiplus.h>

#include "resource.h"
class CDrawImageEngine
{
public:
	CDrawImageEngine(void);
	~CDrawImageEngine(void);
public:
	static	CDrawImageEngine&	GetInstance();
public:
	//创建画图设备，设置画布大小
	void Create( HDC hdc, Gdiplus::REAL fPageWidth, Gdiplus::REAL fPageHeight);

	INT CreateImage( CArray<Gdiplus::Point>& ptList, INT nImgWidth, INT nImgHeight, CImageList& imgList);
	//边框大小(槽=图片内的左右槽之和的宽度、高度)
	Gdiplus::Bitmap* CreateImageData(INT nGroutX, INT nGroutY, INT nSrcWidth, INT nSrcHeight, Gdiplus::Color clBackground, CString strPicPath );
	void SetOffset(Gdiplus::REAL dx, Gdiplus::REAL dy){m_fOffsetX = dx; m_fOffsetY = dy;}

	Gdiplus::REAL m_fOffsetX;		//0，0点位于画布的X位置，数据转换用
	Gdiplus::REAL m_fOffsetY;		//0，0点位于画布的Y位置
	//绘画区域宽度
	Gdiplus::REAL m_fWidth;
	//绘画区域高度
	Gdiplus::REAL m_fHeight;

	HDC m_hDC;
private:
	Gdiplus::Color m_clPenColor;
	Gdiplus::Color m_clDotColor;

};

