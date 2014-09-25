#ifndef BaseODL_h__
#define BaseODL_h__

#pragma once
#include "..\\PublicDefine.h"
#include <vector>
#include <gdiplus.h>
#include <math.h>

#include "ODLTools.h"
#include "BaseODLFwd.h"
#include "BLL/BaseBLL.h"
#include "SceneNode/TSceneNode.h"
#include "TopoShape/CTopoShape.h"

#include "Geom_Curve.hxx"
#include "gp_pnt.hxx"

#include "TopoDS_Vertex.hxx"

#include <boost/optional.hpp>
#include "SkinUnitODL.h"

class CBaseODL : public TSceneNode<CBaseODL>
{
public:

	CBaseODL(void);
	virtual ~CBaseODL(void);

public:
	//////////////////////////////////////////////////////////////////////////
	//整体数据对象、处理函数
	//////////////////////////////////////////////////////////////////////////

	//根据点序列创建对象
	void	Create();
	bool			IsTopCreating(){return m_bIsTopCreating;};
	void			SetIsTopCreating(const bool& val) { m_bIsTopCreating = val; }

	virtual void RemoveSub(CBaseODLSPtr spSub);
	virtual void RemoveSelf();

	virtual ENUM_OBOJECT_TYPE GetType() {return m_ObjType; }
	void SetType(ENUM_OBOJECT_TYPE etType){m_ObjType = etType;}

	//皮肤总是由外部创建
	virtual CBaseODLSPtr	CreateSub(const Gdiplus::PointF& ptPos, CSkinUnitODLSPtr spSkin);

	virtual void SetSkin(CSkinUnitODLSPtr spSkin){m_spSkin=spSkin;};
	virtual CSkinUnitODLSPtr GetSkin()
	{
		if (!m_spSkin)
		{
			m_spSkin = std::make_shared<CSkinUnitODL>();
		}
		return m_spSkin;
	};

	bool& IsCanDelete() { return m_bIsCanDelete; }
	void SetIsCanDelete(const bool& val) { m_bIsCanDelete = val; }


	TopoDS_Shape	GetEdgeByPoint(gp_Pnt ptPos);

	//Rect包含Object
	virtual bool	IsFaceInShape(BRepExtrema_DistShapeShape& dss);

	//相交判断
	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);
	virtual	bool	UpdateMesh() { return false; }

	bool			IsHide() const { return m_bIsHide; }
	void			SetIsHide(const bool& val) { m_bIsHide = val; }

	bool			IsLocked() { return m_bIsLocked; }
	void			SetIsLocked(const bool& val) { m_bIsLocked = val; }

	bool			GetHasMark() const { return m_bHasMark; }
	void			SetHasMark(const bool& val) { m_bHasMark = val; }

	int&			GetIndex() { return m_nIndex; }
	void			SetIndex(const int& val) { m_nIndex = val; }

	ENUM_SIDE_POS GetSide() { return m_eSide; }
	void SetSide(const ENUM_SIDE_POS& val) { m_eSide = val; }

	const CTopoShape& GetFaceShape() const { return m_FaceShape; }
	CTopoShape&	GetFaceShape() { return m_FaceShape; }
	void			SetFaceShape(const CTopoShape& val) { m_FaceShape = val; }

	bool			GetHasLength() const { return m_bHasLength; }
	void			SetHasLength(const bool& val) { m_bHasLength = val; }
	//当前点需要一个着重符号：表示热点
	virtual void SetIsCurrentHotspot(bool bIsCurrentHotspot){m_bIsCurrentHotspot=bIsCurrentHotspot;}
	virtual bool IsCurrentHotspot(){return m_bIsCurrentHotspot;}

protected:
	Gdiplus::FontFamily* m_pFontFamily;
	Gdiplus::SolidBrush* m_pFontBrush;
	Gdiplus::StringFormat m_FontFormat;

	CBaseBLLWPtr m_wpBaseBLL;

	CTopoShape	m_FaceShape;

	//对象的皮肤
	CSkinUnitODLSPtr m_spSkin;

	//当前对象的类型
	ENUM_OBOJECT_TYPE m_ObjType;

	bool m_bIsHide;

	bool m_bIsLocked;

	ENUM_SIDE_POS m_eSide;		//1、上；2、下；3、竖着
	int m_nIndex;				//位于上级对象下的Index

	bool m_bIsCanDelete;
	//对象创建中
	bool m_bIsTopCreating;


	bool m_bHasMark;			//有标注
	bool m_bHasLength;			//有长度显示
	bool m_bIsCurrentHotspot;
public:
	//////////////////////////////////////////////////////////////////////////
	//俯视图数据对象、处理函数
	//////////////////////////////////////////////////////////////////////////

	//参数1：gdi对象；
	//参数2：创建步骤
	//standby:绘制完整数据
	//begin:设置开始点
	//moving:绘制开始点到当前位置的数据
	//end:绘制结束，以后绘制的对象都是完整数据
	virtual void	OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	void InitTopColor();
	virtual void	OnTopMoveStart(const gp_Pnt& stInfo);
	//移动过程
	virtual void	OnTopMoving(const gp_Pnt& stInfo);

	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	//强制停止移动，并回到原来的位置
	virtual void	CancelTopMoving();

	//设置创建开始的点，并设置为创建中
	virtual void	SetTopStartPos(gp_Pnt ptStart)
	{
		m_ptTopStart = ptStart;
		m_ptTopStop = ptStart;
		m_ptTopCurrent = ptStart;
	}

	const gp_Pnt& GetTopStart() const { return m_ptTopStart; }

	const gp_Pnt& GetTopStop() const { return m_ptTopStop; }

	//设置创建结束的点，并设置为创建完成
	virtual void	SetTopStopPos(gp_Pnt ptStop) {	m_ptTopStop = ptStop; }
	//设置当前鼠标所在的点，
	virtual void	SetTopCurrentPos(gp_Pnt ptCurrent){ m_ptTopCurrent = ptCurrent;}

	bool			IsTopMoving(){return m_bIsTopMoving;};
	virtual void	SetIsTopMoving(bool bIsTopMoving){m_bIsTopMoving = bIsTopMoving;};

	bool			IsTopSelected(){return m_bIsTopSelected;};
	virtual void	SetIsTopSelected(bool isTopSelected);

	void	SetIsTopSelected(bool isTopSelected, bool bSetAll);

	bool			IsTopFocused() const { return m_bIsTopFocused; }
	void			SetIsTopFocused(const bool& val) { m_bIsTopFocused = val; }

	void			SetIsTopFocused(bool bIsFocused, bool bSetAll );

	bool			IsTopResizing(){return m_bIsTopResizing;};
	void			SetIsTopResizing(const bool& val) { m_bIsTopResizing = val; }

	virtual void	GetTopPointList(std::vector<gp_Pnt>& arrPoint){arrPoint = m_arrTopPoint;}
	virtual int		GetTopPointCount(){return m_arrTopPoint.size();};

	//强制修改点序列
	virtual void	SetTopPointList(std::vector<gp_Pnt> arrPoint){m_arrTopPoint = arrPoint; m_arrMovingTopPoint=m_arrTopPoint;};
protected:

	//开始画的点的位置
	gp_Pnt m_ptTopStart;
	//画完的时候的点的位置
	gp_Pnt m_ptTopStop;
	//当前点的位置
	gp_Pnt m_ptTopCurrent;

	//对象移动中
	bool m_bIsTopMoving;
	//对象被选中
	bool m_bIsTopSelected;
	//对象是焦点
	bool m_bIsTopFocused;
	//对象尺寸变化中
	bool m_bIsTopResizing;
	//俯视图点
	std::vector<gp_Pnt> m_arrMovingTopPoint;
	std::vector<gp_Pnt> m_arrTopPoint;
public:
	//////////////////////////////////////////////////////////////////////////
	//面正视图数据对象、处理函数
	//////////////////////////////////////////////////////////////////////////
	void InitSideColor();
	//画某个对象的正对面(墙面、地面、天花板面)
	//天花板、地板显示的数据表示：从房间内观察
	//墙面数据：从墙外部观察(Offset:位于上级的偏移)
	virtual void	OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

	//判断点是否在区域内(Zone专用，2维；0、不在；
	//1、左上；2、上；3、右上；4、右；5、右下；6、下；7、左下；8、左；10、在中间)
	virtual ENUM_RECT_SELECTED_POS IsSidePointInside(const Gdiplus::PointF& ptPos);

	virtual void	OnSideMoveStart(const Gdiplus::PointF& ptStart);
	virtual void	OnSideMoving(const Gdiplus::PointF& ptOffset);

	virtual void	OnSideMovingFinish(const Gdiplus::PointF& ptOffset, bool bCommitChange);
	//强制停止移动，并回到原来的位置
	virtual void	CancelSideMoving(){};
	//
	virtual void	OnSideReSizeStart( const Gdiplus::PointF& ptStart){};
	//修改尺寸中
	virtual void	OnSideReSizing( const Gdiplus::PointF& ptCurrent){};
	//修改尺寸完成
	virtual void	OnSideReSizeEnd( const Gdiplus::PointF& ptEnd){};

	//获取当前对象的相对位置和尺寸
	virtual void	GetSideRect(Gdiplus::RectF& rtSelf){rtSelf = m_rtArea;};
	//获取本人可以变的最小尺寸
	virtual bool	GetSideMinRect(Gdiplus::RectF& rtLimit,int& nCount);
	//获取对象允许放置的区域,返回false，不能放置
	virtual bool	GetSideFitRect(CBaseODLSPtr spSelect, bool bUpdateSelf, bool bIsMax, Gdiplus::RectF& rtSelf);
	//获取最小可用的X坐标
	virtual bool	GetSideMinLimitXPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);
	//获取最大可用的X坐标
	virtual bool	GetSideMaxLimitXPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);
	//获取最小可用的Y坐标
	virtual bool	GetSideMinLimitYPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);
	//获取最大可用的Y坐标
	virtual bool	GetSideMaxLimitYPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);

	virtual ENUM_RECT_SELECTED_POS		GetSideSelectedPos(){return m_eSelectedPos;};

	virtual void	GetSidePointList(std::vector<Gdiplus::PointF>& arrPoint);
	virtual int		GetSidePointCount(){return 0;};

	virtual void	SetSideGolbalOffset(const Gdiplus::PointF& ptOffset){m_ptSideGlobalOffset = ptOffset;};

	virtual void	UpdateSideGlobalOffset(const Gdiplus::PointF& ptOffset);

	virtual void	SetSideStartPos(const Gdiplus::PointF& ptStart){m_ptSideStart = ptStart;};
	virtual void	SetSideStopPos(const Gdiplus::PointF& ptStop){m_ptSideStop = ptStop;};


	//检查传入矩形与已经存在的对象是否有裁切，除去传入的对象(不判断他)
	virtual bool	CheckSideIntersected(CBaseODLSPtr spExcept, Gdiplus::RectF& rtCheck);
	virtual void	SetSideSize(const Gdiplus::RectF& rtTarget){};
	virtual void	SetSideLimitArea(const Gdiplus::RectF& rtLimit){};

	bool			IsSideMoving(){return m_bIsSideMoving;};
	virtual void	SetIsSideMoving(bool bIsSideMoving){m_bIsSideMoving = bIsSideMoving;};

	bool			IsSideSelected(){return m_bIsSideSelected;};
	virtual void	SetIsSideSelected(bool isSideSelected);

	bool			IsSideFocused() const { return m_bIsSideFocused; }
	void			SetIsSideFocused(const bool& val) { m_bIsSideFocused = val; }

	bool			IsSideResizing(){return m_bIsSideResizing;};
	void			SetIsSideResizing(const bool& val) { m_bIsSideResizing = val; }
	//清除所有状态
	void SetClearSideState(bool bAllSet);
protected:
	Gdiplus::Color m_clPenColor;
	Gdiplus::Color m_clDotColor;
	Gdiplus::Color m_clResizeColor;
	//面视图上的信息
	Gdiplus::PointF m_ptSideStart;
	Gdiplus::PointF m_ptSideStop;

	//当前对象相对于顶级对象的偏移(X0,Y0点，计算结果，包括本级对象位于上级的偏移)
	Gdiplus::PointF m_ptSideGlobalOffset;

	ENUM_RECT_SELECTED_POS m_eSelectedPos;
	//对象移动中
	bool m_bIsSideMoving;
	//对象被选中
	bool m_bIsSideSelected;
	//对象是焦点
	bool m_bIsSideFocused;
	//对象尺寸变化中
	bool m_bIsSideResizing;
	//本级对象的大小，位置始终为当前对象与上级对象的偏移
	Gdiplus::RectF m_rtTopArea;
	Gdiplus::RectF m_rtMovingTopArea;
	Gdiplus::RectF m_rtLimitTopArea;		//区域只能在此范围内移动(墙面)

	Gdiplus::RectF m_rtArea;
	Gdiplus::RectF m_rtMovingArea;
	Gdiplus::RectF m_rtLimitArea;		//区域只能在此范围内移动(墙面)
	//面上的区域的点
	std::vector<Gdiplus::PointF>	m_arrSidePoint;
protected:
	
	//缩放比例
	//float m_fScale;
	
	
};

#endif // BaseODL_h__
