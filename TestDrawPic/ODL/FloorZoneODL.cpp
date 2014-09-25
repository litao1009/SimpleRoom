#include "stdafx.h"
#include "FloorZoneODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "TopExp_Explorer.hxx"
#include "BRepAlgoAPI_Common.hxx"
#include "BRep_Tool.hxx"
#include "TopoDS.hxx"

#include "ISceneManager.h"
#include "IVideoDriver.h"

CFloorZoneODL::CFloorZoneODL(void)
{
	m_ObjType = EOBT_Floor_Zone;
	m_rtArea.X = 0.0f;
	m_rtArea.Y = 0.0f;
	m_rtArea.Width = 100.0f;
	m_rtArea.Height = 100.0f;

	m_rtLimitArea = m_rtArea;
	m_rtMovingArea = m_rtArea;
	m_pPrevPos.X = 0.0f;
	m_pPrevPos.Y = 0.0f;
}


CFloorZoneODL::~CFloorZoneODL(void)
{
	
}

void CFloorZoneODL::SetSideLimitArea(const Gdiplus::RectF& rtLimit)
{
	m_rtLimitArea = rtLimit;
	m_rtLimitArea.X = 0.0f;
	m_rtLimitArea.Y = 0.0f;

}

void CFloorZoneODL::SetZoneSize(Gdiplus::REAL fWidth, Gdiplus::REAL fHeight)
{
	m_rtArea.Width = fWidth;
	m_rtArea.Height = fHeight;
	m_rtMovingArea = m_rtArea;
	

	//设置所有下级的Limit
	for (auto& curItem: GetChildrenList())
	{
		curItem->SetSideLimitArea(m_rtArea);
	}
	
}

void CFloorZoneODL::CancelSideMoving()
{
	if (m_eSelectedPos!=ERSP_All)
	{
		return;
	}
	m_rtMovingArea = m_rtArea;
	CBaseODL::CancelTopMoving();
}

void CFloorZoneODL::OnSideMoveStart(const Gdiplus::PointF& ptStart)
{
	if (m_eSelectedPos!=ERSP_All)
	{
		return;
	}
	//移动的时候，只能移动下级对象和当前对象
	if (m_rtLimitArea.Equals(m_rtArea))
	{
		//当前对象和上级对象一样大(不移动)
		return;
	}
	m_ptSideStart = ptStart;
	CBaseODL::OnSideMoveStart(ptStart);
}

void CFloorZoneODL::OnSideMoving(const Gdiplus::PointF& ptOffset)
{
	if (m_eSelectedPos!=ERSP_All)
	{
		return;
	}
	//移动的时候，只能移动下级对象和当前对象
	if (m_rtLimitArea.Equals(m_rtArea))
	{
		//当前对象和上级对象一样大(不移动)
		return;
	}
	//相对坐标
	Gdiplus::PointF ptCurrent = ptOffset;

	Gdiplus::REAL fLeft = m_rtLimitArea.X;	//最小允许位置X=0
	Gdiplus::REAL fTop = m_rtLimitArea.Y;	//最小允许位置Y=0
	Gdiplus::REAL fRight = m_rtLimitArea.X + m_rtLimitArea.Width - m_rtArea.Width;	//最大允许位置X
	Gdiplus::REAL fBottom = m_rtLimitArea.Y + m_rtLimitArea.Height - m_rtArea.Height; //最大允许位置Y
	Gdiplus::RectF rtTarget;
	rtTarget.X = m_rtArea.X + ptCurrent.X;
	rtTarget.Y =m_rtArea.Y + ptCurrent.Y;
	rtTarget.Width = m_rtArea.Width;
	rtTarget.Height = m_rtArea.Height;
	if (rtTarget.X<fLeft)
	{
		rtTarget.X = fLeft;
	}
	if (rtTarget.X>fRight)
	{
		rtTarget.X = fRight;
	}
	if (rtTarget.Y<fTop)
	{
		rtTarget.Y = fTop;
	}
	if (rtTarget.Y> fBottom)
	{
		rtTarget.Y = fBottom;
	}
	CBaseODLSPtr spFloorFace =  GetParent().lock();
	if (spFloorFace->CheckSideIntersected(this->shared_from_this(), rtTarget))
	{
		//这里还需要判断一个东西，叫做 当前允许放置的窗口
		Gdiplus::RectF rtSel=rtTarget;
		if (spFloorFace->GetSideFitRect(this->shared_from_this(), false, false, rtSel))
		{
			Gdiplus::RectF rtTemp=rtTarget;
			if (rtTemp.X<rtSel.X)
			{
				rtTemp.X = rtSel.X;
			}
			if (rtTemp.Y<rtSel.Y)
			{
				rtTemp.Y = rtSel.Y;
			}
			//如果跑出去了，则不许移动过去
			if (!rtSel.Contains(rtTemp))
			{
				return;
			}
			if (spFloorFace->CheckSideIntersected(this->shared_from_this(), rtTemp))
			{
				return;
			}
			m_rtMovingArea = rtTemp;
		}
		else
		{
			//找不到可以拖到的目的地
			return;
		}
	}
	else
	{
		m_rtMovingArea = rtTarget;
	}
	//同时移动偏移量
	SetIsSideMoving(true);
	CBaseODL::OnSideMoving(ptOffset);
}

void CFloorZoneODL::OnSideMovingFinish(const Gdiplus::PointF& ptStop, bool bCommitChange )
{
	if (bCommitChange)
	{
		Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
		Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
		//移动的距离
		Gdiplus::PointF ptOffset(m_rtMovingArea.X - m_rtArea.X, m_rtMovingArea.Y - m_rtArea.Y);
		m_rtArea = m_rtMovingArea;

		m_ptSideGlobalOffset.X =m_rtArea.X + fX;
		m_ptSideGlobalOffset.Y =m_rtArea.Y + fY;
		//原始
		m_ptSideStop = ptStop;
		for ( auto& curChild : GetChildrenList() )
		{
			curChild->UpdateSideGlobalOffset(m_ptSideGlobalOffset);
		}
		RefreshRealArea();
		UpdateMesh();
	}
	else
	{
		m_rtMovingArea = m_rtArea;
	}
}

void CFloorZoneODL::OnSideReSizeStart(const Gdiplus::PointF& ptStart)
{
	if ((ERSP_All==m_eSelectedPos) ||(ERSP_None==m_eSelectedPos))
	{
		return;
	}
	m_ptSideStart = ptStart;
	SetSideStartPos(ptStart);
	SetIsSideResizing(true);
}

void CFloorZoneODL::OnSideReSizing( const Gdiplus::PointF& ptCurrent)
{
	if ((ERSP_All==m_eSelectedPos) ||(ERSP_None==m_eSelectedPos))
	{
		return;
	}
	OnSideReSizing(m_eSelectedPos, ptCurrent);
}

void CFloorZoneODL::OnSideReSizing(ENUM_RECT_SELECTED_POS ePos, const Gdiplus::PointF& ptCurrent)
{
	//找到一个最小尺寸
	bool bIsMin = ((m_rtArea.Width<=100.0f) || (m_rtArea.Height<=100.0f));
	//current:当前全局鼠标位置
	//offsetcurrent:当前鼠标在本对象内的位置
	Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
	Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
	Gdiplus::PointF ptOffsetCurrent = ptCurrent;
	//判断一下能不能Resize到目的，不能则直接返回
	//增加的偏移尺寸
	Gdiplus::PointF ptInc(ptOffsetCurrent.X - m_ptSideStart.X, ptOffsetCurrent.Y - m_ptSideStart.Y);
	//上下左右的原始位置
	//尺寸变化前，本区域的大小，和位于上级的坐标
	Gdiplus::PointF ptLeftTop(m_rtArea.X, m_rtArea.Y);
	Gdiplus::PointF ptRightTop(m_rtArea.X + m_rtArea.Width, m_rtArea.Y);
	Gdiplus::PointF ptLeftBottom(m_rtArea.X, m_rtArea.Y+ m_rtArea.Height);
	Gdiplus::PointF ptRightBottom(m_rtArea.X + m_rtArea.Width, m_rtArea.Y + m_rtArea.Height);

	int nLimitCt=0;
	//获取本对象最小只能到多小
	Gdiplus::RectF rtLimited;
	GetSideMinRect(rtLimited, nLimitCt);
	
	//不允许处于此区域内
	Gdiplus::REAL fX0 = rtLimited.GetLeft() + m_rtArea.X;
	Gdiplus::REAL fY0 = rtLimited.GetTop() + m_rtArea.Y;
	Gdiplus::REAL fX1 = rtLimited.GetRight() + m_rtArea.X;
	Gdiplus::REAL fY1 = rtLimited.GetBottom() + m_rtArea.Y;
	switch (ePos)
	{
	case ERSP_LeftTop:
		{
			//左上
			ptLeftTop.X +=ptInc.X;
			ptLeftTop.Y +=ptInc.Y;
			//右上
			ptRightTop.Y +=ptInc.Y;
			//左下
			ptLeftBottom.X +=ptInc.X;
		}
		break;
	case ERSP_RightTop:
		{
			//右上
			ptRightTop.X +=ptInc.X;
			ptRightTop.Y +=ptInc.Y;
			//左上
			ptLeftTop.Y +=ptInc.Y;
			//右下
			ptRightBottom.X +=ptInc.X;
		}
		break;
	case ERSP_RightBottom:
		{
			//右下
			ptRightBottom.X +=ptInc.X;
			ptRightBottom.Y +=ptInc.Y;
			//右上
			ptRightTop.X +=ptInc.X;
			//左下
			ptLeftBottom.Y +=ptInc.Y;
		}
		break;
	case ERSP_LeftBottom:
		{
			//左下
			ptLeftBottom.X +=ptInc.X;
			ptLeftBottom.Y +=ptInc.Y;
			//左上
			ptLeftTop.X +=ptInc.X;
			//右下
			ptRightBottom.Y +=ptInc.Y;
		}
		break;
	case ERSP_Top:
		{
			//上
			ptLeftTop.Y +=ptInc.Y;
			ptRightTop.Y +=ptInc.Y;
		}
		break;
	case ERSP_Bottom:
		{
			//6、下；
			ptLeftBottom.Y +=ptInc.Y;
			ptRightBottom.Y +=ptInc.Y;
		}
		break;
	case ERSP_Right:
		{
			//4右
			ptRightTop.X +=ptInc.X;
			ptRightBottom.X +=ptInc.X;
		}
		break;
	case ERSP_Left:
		{
			//8、左
			ptLeftTop.X +=ptInc.X;
			ptLeftBottom.X +=ptInc.X;
		}
		break;
	}
	//保证最小是个框框
	if (ptRightTop.X<ptLeftTop.X)
	{
		ptLeftBottom.X = ptLeftTop.X;
		ptRightTop.X = ptLeftTop.X + 10;
		ptRightBottom.X = ptRightTop.X;
	}
	if (ptRightBottom.Y<ptRightTop.Y)
	{
		ptRightTop.Y = ptLeftTop.Y;
		ptLeftBottom.Y = ptLeftTop.Y + 10;
		ptRightBottom.Y = ptLeftBottom.Y;
	}
	Gdiplus::REAL fLeft = m_rtLimitArea.X;	//最小允许位置X
	Gdiplus::REAL fTop = m_rtLimitArea.Y;	//最小允许位置Y
	Gdiplus::REAL fRight = m_rtLimitArea.X + m_rtLimitArea.Width;
	Gdiplus::REAL fBottom = m_rtLimitArea.Y + m_rtLimitArea.Height;
	//小于最左边
	if (ptLeftTop.X<fLeft)
	{
		ptLeftTop.X = fLeft;
		ptLeftBottom.X = fLeft;
	}
	//大于最右边
	if (ptRightTop.X>fRight)
	{
		ptRightTop.X = fRight;
		ptRightBottom.X = fRight;
	}
	if (ptLeftTop.Y<fTop)
	{
		ptLeftTop.Y = fTop;
		ptRightTop.Y = fTop;
	}
	if (ptLeftBottom.Y>fBottom)
	{
		ptLeftBottom.Y = fBottom;
		ptRightBottom.Y = fBottom;
	}
	if (nLimitCt>0)
	{
		switch (ePos)
		{
		case ERSP_LeftTop:
			{
				//左上
				if (ptInc.X>0)
				{
					//缩小
					if (fX1>=ptRightTop.X)
					{
						//右边不许动
						ptRightBottom.X = ptRightTop.X;
						//然后把所有的点靠回左边
						ptLeftTop.X = ptRightTop.X - rtLimited.Width - rtLimited.X;
						ptLeftBottom.X = ptLeftTop.X;
					}
				}
				if (ptInc.Y>0)
				{
					//缩小
					if (fY1>=ptRightBottom.Y)
					{
						//下边不许动
						ptLeftBottom.Y = ptRightBottom.Y;
						//然后把所有的点靠回左边
						ptRightTop.Y = ptRightBottom.Y - rtLimited.Height - rtLimited.Y;
						ptLeftTop.Y = ptRightTop.Y;
					}
				}
			}
			break;
		case ERSP_RightTop:
			{
				//右上
				if (ptInc.X<0)
				{
					//缩小(移动右面)
					if (fX1>=ptRightTop.X)
					{
						//左边不许动
						ptLeftBottom.X = ptLeftTop.X;
						//然后把所有的点靠回左边
						ptRightTop.X = ptLeftTop.X + rtLimited.Width + rtLimited.X;
						ptRightBottom.X = ptRightTop.X;
					}
				}
				if (ptInc.Y>0)
				{
					//缩小
					if (fY1>=ptRightBottom.Y)
					{
						//下边不许动
						ptLeftBottom.Y = ptRightBottom.Y;
						//然后把所有的点靠回左边
						ptRightTop.Y = ptRightBottom.Y - rtLimited.Height - rtLimited.Y;
						ptLeftTop.Y = ptRightTop.Y;
					}
				}
			}
			break;
		case ERSP_RightBottom:
			{
				//右下
				if (ptInc.X<0)
				{
					//缩小(移动右面)
					if (fX1>=ptRightTop.X)
					{
						//左边不许动
						ptLeftBottom.X = ptLeftTop.X;
						//然后把所有的点靠回左边
						ptRightTop.X = ptLeftTop.X + rtLimited.Width + rtLimited.X;
						ptRightBottom.X = ptRightTop.X;
					}
				}
				if (ptInc.Y<0)
				{
					//缩小(移动下面)
					if (fY1>=ptRightBottom.Y)
					{
						//上边不许动
						ptLeftTop.Y = ptRightTop.Y;
						//然后把所有的点靠回左边
						ptRightBottom.Y = ptRightTop.Y + rtLimited.Height + rtLimited.Y;
						ptLeftBottom.Y = ptRightBottom.Y;
					}
				}
			}
			break;
		case ERSP_LeftBottom:
			{
				//左下
				if (ptInc.X>0)
				{
					//缩小
					if (fX1>=ptRightTop.X)
					{
						//右边不许动
						ptRightBottom.X = ptRightTop.X;
						//然后把所有的点靠回左边
						ptLeftTop.X = ptRightTop.X - rtLimited.Width - rtLimited.X;
						ptLeftBottom.X = ptLeftTop.X;
					}
				}
				if (ptInc.Y<0)
				{
					//缩小(移动下面)
					if (fY1>=ptRightBottom.Y)
					{
						//上边不许动
						ptLeftTop.Y = ptRightTop.Y;
						//然后把所有的点靠回左边
						ptRightBottom.Y = ptRightTop.Y + rtLimited.Height + rtLimited.Y;
						ptLeftBottom.Y = ptRightBottom.Y;
					}
				}
			}
			break;
		case ERSP_Top:
			{
				//上
				if (ptInc.Y>0)
				{
					//缩小
					if (fY1>=ptRightBottom.Y)
					{
						//下边不许动
						ptLeftBottom.Y = ptRightBottom.Y;
						//然后把所有的点靠回左边
						ptRightTop.Y = ptRightBottom.Y - rtLimited.Height - rtLimited.Y;
						ptLeftTop.Y = ptRightTop.Y;
					}
				}
			}
			break;
		case ERSP_Right:
			{
				//4右
				if (ptInc.X<0)
				{
					//缩小(移动右面)
					if (fX1>=ptRightTop.X)
					{
						//左边不许动
						ptLeftBottom.X = ptLeftTop.X;
						//然后把所有的点靠回左边
						ptRightTop.X = ptLeftTop.X + rtLimited.Width + rtLimited.X;
						ptRightBottom.X = ptRightTop.X;
					}
				}
			}
			break;
		case ERSP_Bottom:
			{
				//6、下；
				if (ptInc.Y<0)
				{
					//缩小(移动下面)
					if (fY1>=ptRightBottom.Y)
					{
						//上边不许动
						ptLeftTop.Y = ptRightTop.Y;
						//然后把所有的点靠回左边
						ptRightBottom.Y = ptRightTop.Y + rtLimited.Height + rtLimited.Y;
						ptLeftBottom.Y = ptRightBottom.Y;
					}
				}
			}
			break;
		case ERSP_Left:
			{
				//8、左
				if (ptInc.X>0)
				{
					//缩小
					if (fX1>=ptRightTop.X)
					{
						//右边不许动
						ptRightBottom.X = ptRightTop.X;
						//然后把所有的点靠回左边
						ptLeftTop.X = ptRightTop.X - rtLimited.Width - rtLimited.X;
						ptLeftBottom.X = ptLeftTop.X;
					}
				}
			}
			break;
		}
	}
	
	Gdiplus::RectF rtDest(ptLeftTop.X, ptLeftTop.Y, ptRightTop.X - ptLeftTop.X, ptRightBottom.Y - ptRightTop.Y);
	//如果目标位置不在允许范围内，则不许修改
	CBaseODLSPtr spFloorFace =  GetParent().lock();
	Gdiplus::RectF rtTmp(rtDest);
	if (spFloorFace->CheckSideIntersected(this->shared_from_this(), rtTmp))
	{
		return;
	}
	Gdiplus::SizeF cOrg;
	m_rtArea.GetSize(&cOrg);
	Gdiplus::SizeF cDst;
	rtDest.GetSize(&cDst);
	if (((cDst.Width < cOrg.Width) || (cDst.Height<cOrg.Height) ) && (bIsMin))
	{
		return;
	}
	
	m_rtArea = rtDest;
	m_ptSideStart = ptCurrent;
	m_ptSideGlobalOffset.X = m_rtArea.X + fX;
	m_ptSideGlobalOffset.Y = m_rtArea.Y + fY;
	for (auto& curItem: GetChildrenList())
	{
		curItem->SetSideLimitArea(m_rtArea);
		curItem->UpdateSideGlobalOffset(m_ptSideGlobalOffset);
	}
}

void CFloorZoneODL::OnSideReSizeEnd(const Gdiplus::PointF& ptStop)
{
	SetIsSideResizing(false);
	
	m_pPrevPos.X = 0.0f;
	m_pPrevPos.Y = 0.0f;
	m_rtMovingArea = m_rtArea;
	SetSideStopPos(ptStop);
	m_ptSideStop = ptStop;

	RefreshRealArea();
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->SetSideLimitArea(m_rtArea);
	}
	UpdateMesh();
}

void CFloorZoneODL::SetIsTopSelected(bool isSelected)
{
	CBaseODL::SetIsTopSelected(isSelected);
	if (!isSelected)
	{
		m_eSelectedPos = ERSP_None;
	}
	/*for ( auto& curChild : GetChildrenList() )
	{
	curChild->SetIsSelected(isSelected);
	}*/
}

void CFloorZoneODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale )
{
	//画墙面矩形
	InitSideColor();
	//区域永远是4个点
	Gdiplus::PointF ptCenter(0.0f,0.0f);
	std::vector<Gdiplus::PointF> arrSidePoint;
	GetSidePointList(arrSidePoint);
	for (auto& curItem : arrSidePoint)
	{
		ptCenter.X += curItem.X;
		ptCenter.Y += curItem.Y;
	}
	
	Gdiplus::Pen Dot( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	ptCenter.X /=4.0f;
	ptCenter.Y /=4.0f;
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);
	Gdiplus::RectF rtDrawArea=m_rtArea;
	rtDrawArea.X = m_ptSideGlobalOffset.X;
	rtDrawArea.Y = m_ptSideGlobalOffset.Y;

	Gdiplus::RectF rtDrawMovingArea=m_rtMovingArea;
	rtDrawMovingArea.X =  m_rtMovingArea.X - m_rtArea.X + m_ptSideGlobalOffset.X;
	rtDrawMovingArea.Y =  m_rtMovingArea.Y - m_rtArea.Y + m_ptSideGlobalOffset.Y;
	if (m_spSkin)
	{
		m_spSkin->DrawImage(gcDrawer, rtDrawArea, fScale);
	}

	Gdiplus::RectF rtResizeArea(rtDrawArea.X+50.0f, rtDrawArea.Y+50.0f, rtDrawArea.Width - 100.0f, rtDrawArea.Height -100.0f);
	Gdiplus::Pen penResize( m_clResizeColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	gcDrawer.DrawRectangle(&penResize, rtResizeArea);
	if (IsSideMoving())
	{
		Gdiplus::Pen penMoving( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
		penMoving.SetDashStyle( Gdiplus::DashStyleSolid);
		gcDrawer.DrawRectangle(&penMoving, rtDrawMovingArea);
	}
	if (IsSideSelected() || IsSideResizing())
	{
		//每个转角画一个点
		for (const auto& curPnt : arrSidePoint )
		{
			gcDrawer.DrawRectangle(&Dot, (curPnt.X) - 3.0f/fScale, (curPnt.Y ) - 3.0f/fScale, 6.0f/fScale, 6.0f/fScale);
		}
	}
	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	CString strTxt;
	strTxt.Format(_T("%.2f,%.2f; %.2f,%.2f; %.2f, %.2f"), m_rtArea.X, m_rtArea.Y, rtDrawArea.X, rtDrawArea.Y, m_rtArea.Width, m_rtArea.Height );
	gcDrawer.DrawString(strTxt, strTxt.GetLength(), &myFont, arrSidePoint[0], &m_FontFormat, m_pFontBrush );//把string绘制到图
	//画这个面在俯视图下级的线段
	CBaseODL::OnDrawFace(gcDrawer, fScale);
}

void CFloorZoneODL::SetSideSize(const Gdiplus::RectF& rtTarget )
{
	Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
	Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
	m_rtArea = rtTarget;
	m_ptSideGlobalOffset.X =fX + m_rtArea.X;
	m_ptSideGlobalOffset.Y =fY + m_rtArea.Y;
	m_rtMovingArea = m_rtArea;
	//设置所有下级的Limit
	for (auto& curItem: GetChildrenList())
	{
		curItem->SetSideLimitArea(m_rtArea);
	}
}

bool CFloorZoneODL::UpdateMesh()
{
	using namespace irr;
	using namespace core;
	using namespace scene;
	using namespace video;

	auto sn = GetSceneNode();
	if ( !sn )
	{
		return false;
	}

	if ( nullptr == sn->getMesh() )
	{
		auto mesh = ODLTools::CreateSimpleRectMesh(irr::core::rectf(0,0,m_rtArea.Width,m_rtArea.Height));

		auto pntSN = GetParent().lock()->GetSceneNode();
		if ( pntSN )
		{
			auto pntMesh = pntSN->getMesh();
			if ( pntMesh )
			{
				auto pntMeshBuffer = pntMesh->getMeshBuffer(0);
				if ( irr::video::EPO_BACK != pntMeshBuffer->getMaterial().PolygonOffsetDirection )
				{
					mesh->getMeshBuffer(0)->getMaterial().PolygonOffsetFactor = pntMeshBuffer->getMaterial().PolygonOffsetFactor + 2;
				}
			}
		}

		sn->setMesh(mesh);
		mesh->drop();

		sn->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		sn->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
		sn->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, true);
		sn->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
	}
	else
	{
		ODLTools::UpdateSimpleRectMesh(sn->getMesh(), irr::core::rectf(0,0,m_rtArea.Width,m_rtArea.Height));
	}

	sn->setPosition(vector3df(m_rtArea.X, -m_rtArea.Y, 0));
	sn->CreateTriSelector();

	//设置纹理
	if ( m_spSkin && m_spSkin->HasPicData() )
	{
		auto tex = m_spSkin->GetTextureData();
		sn->setMaterialTexture(0, tex);

		auto uLen = m_spSkin->GetSkinWidth()+m_spSkin->GetGroutX();
		auto vLen = m_spSkin->GetSkinHeight()+m_spSkin->GetGroutY();
		auto uFactor = m_rtArea.Width / uLen;
		auto vFactor = m_rtArea.Height / vLen;

		irr::core::matrix4 texMat;
		texMat.setScale(irr::core::vector3df(uFactor, vFactor, 1));
		auto meshBufferPtr = sn->getMesh()->getMeshBuffer(0);
		meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
	}

	return true;
}

bool gpPntCompare(const gp_Pnt& p1, const gp_Pnt& p2)
{
	if ( p1.X() < p2.X() )
	{
		return true;
	}

	if ( p1.X() > p2.X() )
	{
		return false;
	}

	if ( p1.Y() < p2.Y() )
	{
		return true;
	}

	if ( p1.Y() > p2.Y() )
	{
		return false;
	}

	return p1.Z() < p2.Z();
}

void CFloorZoneODL::RefreshRealArea()
{

	auto& dss = GetFaceShape().GetDisSS();
	auto& face = GetFaceShape().GetFace();

	BRepBuilderAPI_MakePolygon ply;
	ply.Add(gp_Pnt(m_rtArea.X, 0, m_rtArea.Y));
	ply.Add(gp_Pnt(m_rtArea.GetRight(), 0, m_rtArea.Y));
	ply.Add(gp_Pnt(m_rtArea.GetRight(), 0, m_rtArea.GetBottom()));
	ply.Add(gp_Pnt(m_rtArea.X, 0, m_rtArea.GetBottom()));
	ply.Close();

	auto subFace = BRepBuilderAPI_MakeFace(ply.Wire()).Face();


	BRepAlgoAPI_Common bc(subFace, GetFaceShape().GetFace());
	if ( !bc.IsDone() )
	{
		return;
	}

	auto compound = bc.Shape();
	TopExp_Explorer faceExp(compound, TopAbs_FACE);
	auto resFace = TopoDS::Face(faceExp.Current());
	TopExp_Explorer wireExp(resFace, TopAbs_WIRE);

	std::vector<gp_Pnt> vec;
	for ( BRepTools_WireExplorer verExp(TopoDS::Wire(wireExp.Current())); verExp.More(); verExp.Next() )
	{
		auto pnt = BRep_Tool::Pnt(verExp.CurrentVertex());
		vec.push_back(pnt);
	}
	std::sort(vec.begin(), vec.end(), &gpPntCompare);

}

