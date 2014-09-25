#pragma once
//////////////////////////////////////////////////////////////////////////
//�����������������߻����б���ߵľ�̬�������磺ͼƬ��ֻ����ʾ����״
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
	//������ͼ�豸�����û�����С
	void Create( HDC hdc, Gdiplus::REAL fPageWidth, Gdiplus::REAL fPageHeight);

	INT CreateImage( CArray<Gdiplus::Point>& ptList, INT nImgWidth, INT nImgHeight, CImageList& imgList);
	//�߿��С(��=ͼƬ�ڵ����Ҳ�֮�͵Ŀ�ȡ��߶�)
	Gdiplus::Bitmap* CreateImageData(INT nGroutX, INT nGroutY, INT nSrcWidth, INT nSrcHeight, Gdiplus::Color clBackground, CString strPicPath );
	void SetOffset(Gdiplus::REAL dx, Gdiplus::REAL dy){m_fOffsetX = dx; m_fOffsetY = dy;}

	Gdiplus::REAL m_fOffsetX;		//0��0��λ�ڻ�����Xλ�ã�����ת����
	Gdiplus::REAL m_fOffsetY;		//0��0��λ�ڻ�����Yλ��
	//�滭������
	Gdiplus::REAL m_fWidth;
	//�滭����߶�
	Gdiplus::REAL m_fHeight;

	HDC m_hDC;
private:
	Gdiplus::Color m_clPenColor;
	Gdiplus::Color m_clDotColor;

};

