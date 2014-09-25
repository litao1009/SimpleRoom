#pragma once

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "gp_pnt.hxx"
#include "bnd_box.hxx"
#include "ODL/DesignODL.h"
#include "ODL/BaseODL.h"

using namespace Gdiplus;


class CDrawEngine
{
public:

	static	CDrawEngine&	GetInstance();

public:

	CDrawEngine(void);
	~CDrawEngine(void);

public:
	void NormalToScale(Gdiplus::PointF& pt);
	void ScaleToNormal(Gdiplus::PointF& pt);
public:
	void SetDC(HDC hDC);
	//创建画图设备，设置画布大小
	void Create(Gdiplus::REAL fPageWidth, Gdiplus::REAL fPageHeight);
	//设置显示的视图方式
	void SetViewType(ENUM_ViewType vtType){m_vtViewType = vtType;};
	//执行画对象操作
	void OnDrawTop(CBaseODLSPtr pObjSPtr, Gdiplus::REAL fViewPortWidth, Gdiplus::REAL fViewPortHeight);

	//画墙面或者地板
	//墙面画矩形框、地板画整个地板的多边形
	//操作过程中的显示需要画
	void OnDrawFace(CBaseODLSPtr pObjSPtr, Gdiplus::REAL fViewPortWidth, Gdiplus::REAL fViewPortHeight);
	
	//////////////////////////////////////////////////////////////////////////
	//俯视图下偏移值
	//设置当前视觉的位置
	void SetTopOffset(Gdiplus::REAL dx, Gdiplus::REAL dy){m_fTopOffsetX = dx;m_fTopOffsetY = dy;}
	void SetTopOffset(INT nOffsetX, INT nOffsetY){SetTopOffset(Gdiplus::REAL(nOffsetX), Gdiplus::REAL(nOffsetY));}
	const Gdiplus::REAL& GetTopOffsetX() const { return m_fTopOffsetX; }
	void SetTopOffsetX(const Gdiplus::REAL& val) { m_fTopOffsetX = val; }

	const Gdiplus::REAL& GetTopOffsetY() const { return m_fTopOffsetY; }
	void SetTopOffsetY(const Gdiplus::REAL& val) { m_fTopOffsetY = val; }

	void TranslateTopOffsetPos(Gdiplus::PointF& ptPos);
	
	//////////////////////////////////////////////////////////////////////////
	//面编辑下偏移值

	void SetSideOffset(Gdiplus::REAL dx, Gdiplus::REAL dy){m_fSideOffsetX = dx;m_fSideOffsetY = dy;};
	
	const Gdiplus::REAL& GetSideOffsetX() const { return m_fSideOffsetX; }
	void SetSideOffsetX(const Gdiplus::REAL& val) { m_fSideOffsetX = val; }

	const Gdiplus::REAL& GetSideOffsetY() const { return m_fSideOffsetY; }
	void SetSideOffsetY(const Gdiplus::REAL& val) { m_fSideOffsetY = val; }

	void TranslateSideOffsetPos(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	//立面图偏移值
	
	//把文字显示到指定的位置
	void SetText(CString strText, Gdiplus::PointF ptPos);


	const BOOL& GetIsDrag() const { return m_bIsDrag; }
	BOOL& GetIsDrag() { return m_bIsDrag; }
	void SetIsDrag(const BOOL& val) { m_bIsDrag = val; }

	void SetDragPos(Gdiplus::PointF& stStart, Gdiplus::PointF& stStop, bool bIsDrag){m_pBeginPoint = stStart; m_pEndPoint = stStop; m_bIsDrag = bIsDrag;}
public:
	const Gdiplus::REAL& GetScale() const { return m_fScale; }
	void SetScale(const Gdiplus::REAL& fScale){m_fScale = fScale;}

	void SetScaleOffset(const Gdiplus::REAL& fScaleOffset);


	void GetPageSize(Gdiplus::REAL& fWidth, Gdiplus::REAL& fHeight){fWidth = m_fWidth; fHeight = m_fHeight;};
private:
	//绘画区域宽度
	Gdiplus::REAL m_fWidth;
	//绘画区域高度
	Gdiplus::REAL m_fHeight;
	HDC m_hDC;
	Gdiplus::PointF m_pBeginPoint;
	Gdiplus::PointF m_pEndPoint;
	BOOL m_bIsDrawing;
	BOOL m_bIsDrag;
	CWnd* m_pParentWnd;
	CString m_strText;
	Gdiplus::PointF m_ptTextPoint;
	//当前视觉位置
	ENUM_ViewType m_vtViewType;
	
private:
	//Graphics* m_gGraphics;
	GdiplusStartupInput		gdiplusStartupInput;
	ULONG_PTR				m_gdiplusToken;
	Gdiplus::REAL m_fTopOffsetX;		//0，0点位于画布的X位置，数据转换用
	Gdiplus::REAL m_fTopOffsetY;		//0，0点位于画布的Y位置

	Gdiplus::REAL m_fSideOffsetX;		//0，0点位于画布的X位置，数据转换用
	Gdiplus::REAL m_fSideOffsetY;		//0，0点位于画布的Y位置

	Gdiplus::REAL m_fViewPortWidth;		//可视区域宽度
	Gdiplus::REAL m_fViewPortHeight;		//可视区域高度

	Gdiplus::REAL m_fScale;			//画布缩放比例
};