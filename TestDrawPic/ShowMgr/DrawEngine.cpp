#include "stdafx.h"
#include "DrawEngine.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif

CDrawEngine::CDrawEngine(void)
{
	m_gdiplusToken = 0;
	GdiplusStartupInput gdiplusStartupInput;
//	m_gGraphics=NULL;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	m_pParentWnd=nullptr;
	m_fTopOffsetX = 0.0f;
	m_fTopOffsetY = 0.0f;

	m_fSideOffsetX = 0.0f;
	m_fSideOffsetY = 0.0f;

	m_fWidth = 1000.0f;
	m_fHeight = 1000.0f;
	m_fScale = 1.0f;
	m_hDC = NULL;
	m_vtViewType = EVT_TOP;
	
	m_strText=_T("");
	m_bIsDrag = FALSE;
}

CDrawEngine::~CDrawEngine(void)
{
	if (m_pParentWnd)
	{
		::ReleaseDC(m_pParentWnd->GetSafeHwnd(), m_hDC);
	}
	GdiplusShutdown( m_gdiplusToken );
}

void CDrawEngine::SetDC(HDC hDC)
{
	if ( hDC )
	{
		m_hDC = hDC;
	}
}

void CDrawEngine::Create(Gdiplus::REAL fPageWidth, Gdiplus::REAL fPageHeight)
{
	
	if (fPageWidth>100)
	{
		m_fWidth = fPageWidth;
	}
	if (fPageHeight>100)
	{
		m_fHeight = fPageHeight;
	}
}

void CDrawEngine::OnDrawTop(CBaseODLSPtr pObjSPtr, Gdiplus::REAL fViewPortWidth, Gdiplus::REAL fViewPortHeight)
{
	if (!m_hDC)
	{
		return;
	}
	HDC hDC = m_hDC;
	//最小画布大小，100,100
	m_fViewPortWidth = fViewPortWidth;
	m_fViewPortHeight = fViewPortHeight;
	
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, (INT)fViewPortWidth, (INT)fViewPortHeight);

	SelectObject(hMemDC, hMemBitmap);

	Graphics buffer(hMemDC);//Gaphics对象引用内存画布   
	buffer.SetSmoothingMode(SmoothingModeHighQuality);//SmoothingModeAntiAlias);
	buffer.ResetTransform();
	
	buffer.ScaleTransform(m_fScale, m_fScale);
	//buffer.TranslateTransform(-m_fOffsetX, -m_fOffsetY);
	//Gdiplus::REAL offsetx = float(m_fViewPortWidth) * ( 1.0f - m_fScale ) / ( 2.0f * m_fScale );
	//Gdiplus::REAL offsety = float(m_fViewPortHeight) * ( 1.0f - m_fScale ) / ( 2.0f * m_fScale );

	Gdiplus::REAL offsetx = m_fViewPortWidth / 2.0f / m_fScale;// - m_fViewPortWidth / 2.0f;
	Gdiplus::REAL offsety = m_fViewPortHeight / 2.0f / m_fScale;// - m_fViewPortHeight/ 2.0f;
	offsetx += m_fTopOffsetX;
	offsety += m_fTopOffsetY;
	buffer.TranslateTransform(offsetx, offsety);
	
	buffer.Clear(Color::White);//清除内存画布并以白色填充，当然也可以是其它颜色   
	
	//画所有的对象
	pObjSPtr->OnDrawTop(buffer, m_fScale);


#pragma region 画标注

	Gdiplus::FontFamily fontFamily(_T("楷体"));
	Gdiplus::Font myFont(&fontFamily, 9 / m_fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));  //半透明+文字RGB颜色

	Gdiplus::StringFormat formatTop;
	formatTop.SetAlignment(Gdiplus::StringAlignmentCenter);
	Gdiplus::StringFormat formatRight;
	formatRight.SetAlignment(Gdiplus::StringAlignmentFar);
	Gdiplus::Pen pen(Gdiplus::Color::Black);

	Gdiplus::PointF ptUp(0.0f, - m_fViewPortHeight/2.0f/m_fScale-m_fTopOffsetY);
	Gdiplus::PointF ptDown(0.0f, 20.0f/m_fScale - m_fViewPortHeight/2.0f/m_fScale - m_fTopOffsetY);

	Gdiplus::PointF ptRight(-m_fTopOffsetX + m_fViewPortWidth /2.0f / m_fScale , 0.0f);
	Gdiplus::PointF ptLeft(-m_fTopOffsetX + m_fViewPortWidth /2.0f / m_fScale - 20.0f/m_fScale, 0.0f);

	CString strTxt1=_T("");
	CString strTxt2=_T("");

	buffer.DrawLine(&pen, ptUp, ptDown);
	buffer.DrawLine(&pen, ptRight, ptLeft);
	strTxt1=_T("0");
	strTxt2=_T("0");
	
	buffer.DrawString(strTxt1, strTxt1.GetLength(), &myFont, ptDown, &formatTop, &blackBrush );
	
	buffer.DrawString(strTxt2, strTxt2.GetLength(), &myFont, ptLeft, &formatTop, &blackBrush );

	Gdiplus::REAL fHeightHole = 20.0f / m_fScale;	//10倍
	Gdiplus::REAL fHeightSep = 10.0f / m_fScale;	//普通

	int nRound = 100;
	if (m_fScale * nRound<5.0f)
	{
		nRound *=5;
	}
	if (m_fScale * nRound >50.0f)
	{
		nRound /= 5;
	}
	int nStepX = static_cast<int>(m_fViewPortWidth/nRound/m_fScale + std::abs(m_fTopOffsetX / nRound));

	for (int i=1; i< nStepX; i++)
	{
		Gdiplus::PointF pt1(ptUp.X - i * nRound, ptUp.Y);

		Gdiplus::PointF pt2(ptUp.X + i * nRound, ptUp.Y);

		if (i%10==0)
		{
			int nTmp = i* nRound;
			strTxt1.Format(_T("%d"), -nTmp );
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X, pt1.Y + fHeightHole);
			pt1.Y +=fHeightHole;
			buffer.DrawString(strTxt1, strTxt1.GetLength(), &myFont, pt1, &formatTop, &blackBrush );
			strTxt2.Format(_T("%d"), nTmp );
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X, pt2.Y + fHeightHole);
			pt2.Y +=fHeightHole;
			buffer.DrawString(strTxt2, strTxt2.GetLength(), &myFont, pt2, &formatTop, &blackBrush );
		}
		else
		{
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X, pt1.Y + fHeightSep);
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X, pt2.Y + fHeightSep);
		}
	}
	int nStepY = static_cast<int>(m_fViewPortHeight/nRound/m_fScale + std::abs(m_fTopOffsetY / 100));
	for (int i=1; i< nStepY; i++)
	{
		Gdiplus::PointF pt1(ptRight.X, ptRight.Y - i * nRound);
		Gdiplus::PointF pt2(ptRight.X, ptRight.Y + i * nRound);
		if (i%10==0)
		{
			int nTmp = i* nRound;
			strTxt1.Format(_T("%d"), -nTmp );
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X - fHeightHole, pt1.Y);
			pt1.X -=fHeightHole;
			buffer.DrawString(strTxt1, strTxt1.GetLength(), &myFont, pt1, &formatRight, &blackBrush );

			strTxt2.Format(_T("%d"), nTmp );
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X - fHeightHole, pt2.Y);
			pt2.X -=fHeightHole;
			buffer.DrawString(strTxt2, strTxt2.GetLength(), &myFont, pt2, &formatRight, &blackBrush );
		}
		else
		{
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X - fHeightSep, pt1.Y);
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X - fHeightSep, pt2.Y);
		}
	}
#pragma endregion 画标注
	
	if (!m_strText.IsEmpty())
	{
		Gdiplus::FontFamily fontFamily(_T("楷体"));
		Gdiplus::Font myFont(&fontFamily, 20 / m_fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
		Gdiplus::SolidBrush blackBrush(Color(255, 0, 0, 0));  //半透明+文字RGB颜色
		
		Gdiplus::StringFormat format;                         
		format.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
		buffer.DrawString(m_strText, m_strText.GetLength(), &myFont, m_ptTextPoint, &format, &blackBrush );//把string绘制到图上
	}
	if (m_bIsDrag)
	{
		INT nX=static_cast<INT>(m_pBeginPoint.X);
		INT nY = static_cast<INT>(m_pBeginPoint.Y);
		INT nWidth = static_cast<INT>(abs(m_pEndPoint.X - m_pBeginPoint.X));
		INT nHeight = static_cast<INT>(abs(m_pEndPoint.Y - m_pBeginPoint.Y));
		Pen m_dragPen(Color(255,255,00,255),1 / m_fScale);
		m_dragPen.SetDashStyle(Gdiplus::DashStyleDash );
		if (m_pBeginPoint.X<m_pEndPoint.X)
		{
			if (m_pBeginPoint.Y>=m_pEndPoint.Y)
			{
				//第一象限：右上
				nY = static_cast<INT>(m_pEndPoint.Y);
			}
		}
		else
		{
			if (m_pBeginPoint.Y<m_pEndPoint.Y)
			{
				//第三象限：左下
				nX=static_cast<INT>(m_pEndPoint.X);
			}
			else
			{
				//第二象限：左上
				nX=static_cast<INT>(m_pEndPoint.X);
				nY=static_cast<INT>(m_pEndPoint.Y);
			}
		}
		buffer.DrawRectangle(&m_dragPen, nX, nY, nWidth, nHeight);
	}
	
	DeleteObject(hMemBitmap);  
	
	BitBlt(hDC, 0, 0, (INT)m_fViewPortWidth, (INT)m_fViewPortHeight, hMemDC, 0, 0, SRCCOPY);

	buffer.ReleaseHDC(hMemDC);
	DeleteDC(hMemDC);

}

void CDrawEngine::TranslateTopOffsetPos(Gdiplus::PointF& ptPos)
{
	Gdiplus::REAL offsetx =  m_fViewPortWidth / 2.0f;
	Gdiplus::REAL offsety =  m_fViewPortHeight / 2.0f;
	offsetx += m_fTopOffsetX * m_fScale;
	offsety += m_fTopOffsetY * m_fScale;

	ptPos.X -= offsetx;
	ptPos.Y -= offsety;	
	ptPos.X /= m_fScale;
	ptPos.Y /= m_fScale;
}

void CDrawEngine::TranslateSideOffsetPos(Gdiplus::PointF& ptPos)
{
	Gdiplus::REAL offsetx = m_fViewPortWidth / 2.0f;
	Gdiplus::REAL offsety = m_fViewPortHeight / 2.0f;
	offsetx += m_fSideOffsetX * m_fScale;
	offsety += m_fSideOffsetY * m_fScale;

	ptPos.X -= offsetx;
	ptPos.Y -= offsety;	
	ptPos.X /= m_fScale;
	ptPos.Y /= m_fScale;
}

void CDrawEngine::SetScaleOffset(const Gdiplus::REAL& fScaleOffset)
{
	Gdiplus::REAL fScale = fScaleOffset;
	//最多缩小100倍
	if ((fScale+m_fScale)<=0.01f)
	{
		//缩放比例不小于0
		return;
	}
	if (fScale>0)
	{
		//放大
		if (m_fScale<10.0f)
		{
			m_fScale=m_fScale + fScale;
		}
	}
	else
	{
		//缩小
		if (m_fScale>0.01f)
		{
			m_fScale=m_fScale + fScale;
		}
	}
}


void CDrawEngine::NormalToScale( Gdiplus::PointF& pt )
{
	pt.X = pt.X * m_fScale;
	pt.Y = pt.Y * m_fScale;
}

void CDrawEngine::ScaleToNormal( Gdiplus::PointF& pt )
{
	pt.X = pt.X / m_fScale;
	pt.Y = pt.Y / m_fScale;
}

void CDrawEngine::SetText( CString strText, Gdiplus::PointF ptPos )
{
	{
		m_strText = strText; 
		m_ptTextPoint= ptPos;
	}
}

CDrawEngine& CDrawEngine::GetInstance()
{
	static	CDrawEngine sInstance;
	return sInstance;
}

void CDrawEngine::OnDrawFace( CBaseODLSPtr pObjSPtr, Gdiplus::REAL fViewPortWidth, Gdiplus::REAL fViewPortHeight )
{
	if (!m_hDC)
	{
		return;
	}
	HDC hDC = m_hDC;
	//最小画布大小，100,100
	m_fViewPortWidth = fViewPortWidth;
	m_fViewPortHeight = fViewPortHeight;
	HDC hMemDC = CreateCompatibleDC(hDC);  
	HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, (INT)fViewPortWidth, (INT)fViewPortHeight);

	SelectObject(hMemDC, hMemBitmap);
	Graphics buffer(hMemDC);//Gaphics对象引用内存画布   
	buffer.SetSmoothingMode(SmoothingModeHighQuality);//SmoothingModeAntiAlias);

	//buffer.SetInterpolationMode(Gdiplus::InterpolationModeDefault);
	buffer.ResetTransform();
	buffer.ScaleTransform(m_fScale, m_fScale);
	Gdiplus::REAL fOffsetX = 0.0f;
	Gdiplus::REAL fOffsetY = 0.0f;
	
	Gdiplus::REAL offsetx = m_fViewPortWidth /  2.0f / m_fScale;
	Gdiplus::REAL offsety = m_fViewPortHeight / 2.0f / m_fScale;
	offsetx += m_fSideOffsetX;
	offsety += m_fSideOffsetY;
	buffer.TranslateTransform(offsetx, offsety);

	buffer.Clear(Color::White);//清除内存画布并以白色填充，当然也可以是其它颜色   
	
	pObjSPtr->OnDrawFace(buffer, m_fScale);


#pragma region 画标注

	Gdiplus::FontFamily fontFamily(_T("楷体"));
	Gdiplus::Font myFont(&fontFamily, 9 / m_fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));  //半透明+文字RGB颜色

	Gdiplus::StringFormat formatTop;
	formatTop.SetAlignment(Gdiplus::StringAlignmentCenter);
	Gdiplus::StringFormat formatRight;
	formatRight.SetAlignment(Gdiplus::StringAlignmentFar);
	Gdiplus::Pen pen(Gdiplus::Color::Red);

	Gdiplus::PointF ptUp(0.0f, - m_fViewPortHeight/2.0f/m_fScale-m_fSideOffsetY);
	Gdiplus::PointF ptDown(0.0f, 20.0f/m_fScale - m_fViewPortHeight/2.0f/m_fScale - m_fSideOffsetY);

	Gdiplus::PointF ptRight(-m_fSideOffsetX + m_fViewPortWidth /2.0f / m_fScale , 0.0f);
	Gdiplus::PointF ptLeft(-m_fSideOffsetX + m_fViewPortWidth /2.0f / m_fScale - 20.0f/m_fScale, 0.0f);

	CString strTxt1=_T("");
	CString strTxt2=_T("");

	buffer.DrawLine(&pen, ptUp, ptDown);
	buffer.DrawLine(&pen, ptRight, ptLeft);
	strTxt1=_T("0");
	strTxt2=_T("0");

	buffer.DrawString(strTxt1, strTxt1.GetLength(), &myFont, ptDown, &formatTop, &blackBrush );

	buffer.DrawString(strTxt2, strTxt2.GetLength(), &myFont, ptLeft, &formatTop, &blackBrush );

	Gdiplus::REAL fHeightHole = 20.0f / m_fScale;	//10倍
	Gdiplus::REAL fHeightSep = 10.0f / m_fScale;	//普通

	int nRound = 100;
	if (m_fScale * nRound<5.0f)
	{
		nRound *=5;
	}
	if (m_fScale * nRound >50.0f)
	{
		nRound /= 5;
	}
	int nStepX = static_cast<int>(m_fViewPortWidth/nRound/m_fScale + std::abs(m_fSideOffsetX / nRound));

	for (int i=1; i< nStepX; i++)
	{
		Gdiplus::PointF pt1(ptUp.X - i * nRound, ptUp.Y);

		Gdiplus::PointF pt2(ptUp.X + i * nRound, ptUp.Y);

		if (i%10==0)
		{
			int nTmp = i* nRound;
			strTxt1.Format(_T("%d"), -nTmp );
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X, pt1.Y + fHeightHole);
			pt1.Y +=fHeightHole;
			buffer.DrawString(strTxt1, strTxt1.GetLength(), &myFont, pt1, &formatTop, &blackBrush );
			strTxt2.Format(_T("%d"), nTmp );
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X, pt2.Y + fHeightHole);
			pt2.Y +=fHeightHole;
			buffer.DrawString(strTxt2, strTxt2.GetLength(), &myFont, pt2, &formatTop, &blackBrush );
		}
		else
		{
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X, pt1.Y + fHeightSep);
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X, pt2.Y + fHeightSep);
		}
	}
	int nStepY = static_cast<int>(m_fViewPortHeight/nRound/m_fScale + std::abs(m_fSideOffsetY / 100));
	for (int i=1; i< nStepY; i++)
	{
		Gdiplus::PointF pt1(ptRight.X, ptRight.Y - i * nRound);
		Gdiplus::PointF pt2(ptRight.X, ptRight.Y + i * nRound);
		if (i%10==0)
		{
			int nTmp = i* nRound;
			strTxt1.Format(_T("%d"), -nTmp );
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X - fHeightHole, pt1.Y);
			pt1.X -=fHeightHole;
			buffer.DrawString(strTxt1, strTxt1.GetLength(), &myFont, pt1, &formatRight, &blackBrush );

			strTxt2.Format(_T("%d"), nTmp );
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X - fHeightHole, pt2.Y);
			pt2.X -=fHeightHole;
			buffer.DrawString(strTxt2, strTxt2.GetLength(), &myFont, pt2, &formatRight, &blackBrush );
		}
		else
		{
			buffer.DrawLine(&pen, pt1.X, pt1.Y, pt1.X - fHeightSep, pt1.Y);
			buffer.DrawLine(&pen, pt2.X, pt2.Y, pt2.X - fHeightSep, pt2.Y);
		}
	}
#pragma endregion 画标注

	if (!m_strText.IsEmpty())
	{
		Gdiplus::FontFamily fontFamily(_T("楷体"));
		Gdiplus::Font myFont(&fontFamily, 20 / m_fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
		Gdiplus::SolidBrush blackBrush(Color(255, 0, 0, 0));  //半透明+文字RGB颜色

		Gdiplus::StringFormat format;                         
		format.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
		buffer.DrawString(m_strText, m_strText.GetLength(), &myFont, Gdiplus::PointF(m_ptTextPoint.X - fOffsetX, m_ptTextPoint.Y - fOffsetY), &format, &blackBrush );//把string绘制到图上
	}
	if (m_bIsDrag)
	{
		INT nX=static_cast<INT>(m_pBeginPoint.X - fOffsetX);
		INT nY = static_cast<INT>(m_pBeginPoint.Y - fOffsetY);
		INT nWidth = static_cast<INT>(abs(m_pEndPoint.X - m_pBeginPoint.X));
		INT nHeight = static_cast<INT>(abs(m_pEndPoint.Y - m_pBeginPoint.Y));
		Pen m_dragPen(Color(255,255,00,255),1 / m_fScale);
		m_dragPen.SetDashStyle(Gdiplus::DashStyleDash );
		if (m_pBeginPoint.X<m_pEndPoint.X)
		{
			if (m_pBeginPoint.Y<m_pEndPoint.Y)
			{
				//第四象限：右下
			}
			else
			{
				//第一象限：右上
				nY = static_cast<INT>(m_pEndPoint.Y);
			}
		}
		else
		{
			if (m_pBeginPoint.Y<m_pEndPoint.Y)
			{
				//第三象限：左下
				nX=static_cast<INT>(m_pEndPoint.X);
			}
			else
			{
				//第二象限：左上
				nX=static_cast<INT>(m_pEndPoint.X);
				nY=static_cast<INT>(m_pEndPoint.Y);
			}
		}
		buffer.DrawRectangle(&m_dragPen, nX, nY, nWidth, nHeight);
	}

	DeleteObject(hMemBitmap);  

	BitBlt(hDC, 0, 0, (INT)m_fViewPortWidth, (INT)m_fViewPortHeight, hMemDC, 0, 0, SRCCOPY);

	buffer.ReleaseHDC(hMemDC);
	DeleteDC(hMemDC);
}

