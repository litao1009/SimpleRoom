// CtrlFuncView.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "CtrlFuncView.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "gce_MakeLin.hxx"
#include "gp_Dir.hxx"
#include "gp_Vec.hxx"
// CCtrlFuncView

IMPLEMENT_DYNCREATE(CCtrlFuncView, CView)

CCtrlFuncView::CCtrlFuncView()
{
	m_bIsDrawing=FALSE;
	m_pGuideLine = nullptr;
	m_pGuideRect = nullptr;
	m_eCurOpt = EOT_Create_None;
	m_eCurViewType = EVT_TOP;
	m_bIsSubViewerHide=false;

	m_bLButtonDown=false;
	m_bMButtonDown=false;

	m_pCurEditObj = nullptr;
	m_pCurMoveObj = nullptr;

	m_fWallHeight = 2800.0f;
	m_fWallThick = 150.0f;
}

CCtrlFuncView::~CCtrlFuncView()
{
}

BEGIN_MESSAGE_MAP(CCtrlFuncView, CView)
//	ON_WM_KEYUP()
//	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCtrlFuncView 绘图

void CCtrlFuncView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CCtrlFuncView 诊断

#ifdef _DEBUG
void CCtrlFuncView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCtrlFuncView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCtrlFuncView 消息处理程序

bool CCtrlFuncView::GetFocused(CBaseODLSPtr pObj, CBaseODLSPtr& pFocused, std::vector<ENUM_OBOJECT_TYPE>& arrType)
{
	for (auto& curItem: pObj->GetChildrenList() )
	{
		if (curItem->IsHide())
		{
			continue;
		}
		if (curItem->IsTopFocused())
		{
			for (auto& curType:arrType)
			{
				if (curType == curItem->GetType())
				{
					//竖起来
					pFocused = curItem;
					return true;
				}
			}
		}
		if (GetFocused(curItem, pFocused, arrType))
		{
			return true;
		}
	}
	return false;
}

bool CCtrlFuncView::SetPointInObjFocused(CBaseODLSPtr pObj, gp_Pnt pt, CBaseODLSPtr pSelected)
{
	//立面图下判断对象
	gp_Pnt ptTop = pt;
	ptTop.SetY(2800.0f);
	gp_Pnt ptBottom = pt;
	ptBottom.SetY(0.0f);
	auto verTop = BRepBuilderAPI_MakeVertex(ptTop).Vertex();
	auto verBottom = BRepBuilderAPI_MakeVertex(ptBottom).Vertex();
	auto ptLine = BRepBuilderAPI_MakeEdge(ptTop, ptBottom).Edge();
	for (auto& curItem: pObj->GetChildrenList() )
	{
		if (curItem->IsHide())
		{
			continue;
		}
		switch (curItem->GetType())
		{
		case EOBT_OBJ_Group:
			{
				//是分组，啥都不做
				curItem->SetIsTopFocused(false);
			}
			break;
		case EOBT_Wall:
		case EOBT_Beam:
		case EOBT_Pillar:
			{
				//墙、梁柱，不可以设置本对象Focused
				curItem->SetIsTopFocused(false);
			}
			break;
		case EOBT_Wall_Face:
			{
				//墙、梁柱的面
				ENUM_SIDE_POS esP = curItem->GetSide();
				switch (pObj->GetType())
				{
				case EOBT_Wall:
				case EOBT_Pillar:
					{
						//是墙、柱，可以选中四周，不能选中上下
						if ((esP==ESP_Up) || (esP==ESP_Down))
						{
							curItem->SetIsTopFocused(false);
							continue;
						}
						if (curItem->HasTopCollisionWith(verTop))
						{
							//竖起来
							if (!pSelected)
							{
								curItem->SetIsTopFocused(true);
								pSelected = curItem;
							}
							else
							{
								curItem->SetIsTopFocused(false);
							}
						}
						else
						{
							curItem->SetIsTopFocused(false);
						}
					}
					break;
				case EOBT_Beam:
					{
						//梁，可以选择四周，下
						if ((esP==ESP_Up))
						{
							curItem->SetIsTopFocused(false);
							continue;
						}
						if (curItem->HasTopCollisionWith(ptLine))
						{
							//竖起来
							if (!pSelected)
							{
								curItem->SetIsTopFocused(true);
								pSelected = curItem;
							}
							else
							{
								curItem->SetIsTopFocused(false);
							}
						}
						else
						{
							curItem->SetIsTopFocused(false);
						}
					}
				}
			}
			break;
		case EOBT_Floor:
			{
				if (curItem->HasTopCollisionWith(verBottom))
				{
					curItem->SetIsTopFocused(true);
				}
				else
				{
					curItem->SetIsTopFocused(false);
				}
			}
			break;
		default:
			{
				curItem->SetIsTopFocused(false);
			}
			break;
		}
		SetPointInObjFocused(curItem, pt, pSelected);
	}
	return false;
}

CBaseODLSPtr CCtrlFuncView::GetTopOnMouseObj(const TopoDS_Shape& inShape,CBaseODLSPtr pParent, CBaseODLSPtr spExcept)
{
	for (auto& spItem:pParent->GetChildrenList())
	{
		if (spItem->IsHide())
		{
			continue;
		}
		if (spItem==spExcept)
		{
			continue;
		}
		if ((spItem->GetType()==EOBT_Floor) || (spItem->GetType()==EOBT_Wall) || (spItem->GetType()==EOBT_Beam) ||(spItem->GetType()==EOBT_Pillar) || (spItem->GetType()==EOBT_Camera)||(spItem->GetType()==ENUM_OBOJECT_TYPE::EOBT_Decor))
		{
			if (spItem->HasTopCollisionWith(inShape))
			{
				return spItem;
			}
		}
		CBaseODLSPtr sp=GetTopOnMouseObj(inShape, spItem, spExcept);
		if (sp)
		{
			return sp;
		}
	}
	return nullptr;
}

void CCtrlFuncView::GetTopOnMouseObj(const TopoDS_Shape& inShape, CBaseODLSPtr pParent, CBaseODLSPtr spExcept, std::vector<CBaseODLSPtr>& arrObj)
{
	for (auto& spItem:pParent->GetChildrenList())
	{

		if (spItem->IsHide())
		{
			continue;
		}
		if (spExcept==spItem)
		{
			continue;
		}
		if ((spItem->GetType()==EOBT_Floor) || (spItem->GetType()==EOBT_Wall) || (spItem->GetType()==EOBT_Beam) ||(spItem->GetType()==EOBT_Pillar) || (spItem->GetType()==EOBT_Camera)||(spItem->GetType()==ENUM_OBOJECT_TYPE::EOBT_Decor))
		{
			if (spItem->HasTopCollisionWith(inShape))
			{
				arrObj.push_back(spItem);
			}
		}
		GetTopOnMouseObj(inShape, spItem, spExcept, arrObj);
	}
}

void CCtrlFuncView::CreateWallLineStart(Gdiplus::PointF& ptPos)
{
#pragma region 画墙
	//要创建墙
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (!m_pGuideLine)
	{
		m_pGuideLine = std::make_shared<CGuideLineODL>();
		m_pObjMgr->AddChild(m_pGuideLine);
		m_pGuideLine->SetTopStartPos(stInfo);
	}
	else
	{
		CGuideLineODLSPtr& pGuide = std::dynamic_pointer_cast<CGuideLineODL>(m_pGuideLine);
		pGuide->SetTopContinuePos(stInfo);
	}
	m_bIsDrawing= TRUE;
#pragma endregion 画墙Line
}

void CCtrlFuncView::CreateWallLineOnMove(Gdiplus::PointF& ptPos)
{
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (m_pGuideLine)
	{
		m_pGuideLine->SetTopCurrentPos(stInfo);
	}
}

void CCtrlFuncView::CreateWallLineFinish(Gdiplus::PointF& ptPos)
{
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (m_pGuideLine)
	{
		//根据CurrentObj的点位置，生成墙壁
		std::vector<gp_Pnt> arrPoint;
		m_pGuideLine->GetPointList(arrPoint);

		auto pGroup=std::make_shared<CObjGroupODL>();
		pGroup->InitByParent(m_pObjMgr, "WallLineGroupODL");

		CShapeTool::CreateWall(arrPoint, -gp::DY(), m_fWallHeight, m_fWallThick, pGroup);

		//最后完成
		CBaseODLSPtr bsPtr = std::dynamic_pointer_cast<CBaseODL>(m_pGuideLine);
		m_pObjMgr->RemoveChild(bsPtr);
		m_pGuideLine = nullptr;
	}
}

void CCtrlFuncView::CreateWallRectOnMove( Gdiplus::PointF& ptPos )
{
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (m_pGuideRect)
	{
		m_pGuideRect->SetTopCurrentPos(stInfo);
	}
}

void CCtrlFuncView::CreateWallRect( Gdiplus::PointF& ptPos )
{
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (!m_pGuideRect)
	{
		m_pGuideRect = std::make_shared<CGuideRectODL>();
		m_pObjMgr->AddChild(m_pGuideRect);
		m_pGuideRect->SetTopStartPos(stInfo);
		m_bIsDrawing= TRUE;
	}
	else
	{
		//完成
		if (m_bIsDrawing)
		{
			//结束画墙
			m_bIsDrawing=FALSE;
			m_pGuideRect->SetTopStopPos(stInfo);
			if (m_pGuideRect)
			{
				//根据CurrentObj的点位置，生成墙壁
				std::vector<gp_Pnt> arrPoint;
				m_pGuideRect->GetTopPointList(arrPoint);
				
				auto pGroup=std::make_shared<CObjGroupODL>();
				pGroup->InitByParent(m_pObjMgr, "WallRectGroupODL");

				CShapeTool::CreateWall(arrPoint, -gp::DY(), m_fWallHeight, m_fWallThick, pGroup);

				//最后完成
				CBaseODLSPtr bsPtr = std::dynamic_pointer_cast<CBaseODL>(m_pGuideRect);
				m_pObjMgr->RemoveChild(bsPtr);
				m_pGuideRect = nullptr;
			}
		}
		m_eCurOpt = EOT_Create_None;
	}
}

void CCtrlFuncView::CreateBeamRect( Gdiplus::PointF& ptPos )
{
	//要创建梁
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (!m_pGuideRect)
	{
		m_pGuideRect = std::make_shared<CGuideRectODL>();
		m_pObjMgr->AddChild(m_pGuideRect);
		m_pGuideRect->SetTopStartPos(stInfo);
		m_bIsDrawing= TRUE;
	}
	else
	{
		//完成
		if (m_bIsDrawing)
		{
			//结束画墙
			m_bIsDrawing=FALSE;
			m_pGuideRect->SetTopStopPos(stInfo);
			if (m_pGuideRect)
			{
				//根据CurrentObj的点位置，生成墙壁
				Gdiplus::RectF rtSize;
				m_pGuideRect->GetSideRect(rtSize);
				std::vector<gp_Pnt> arrPoint;
				m_pGuideRect->GetTopPointList(arrPoint);
				for (auto& it:arrPoint)
				{
					it.SetY(1400.0f);
				}
				auto pGroup=std::make_shared<CObjGroupODL>();
				pGroup->InitByParent(m_pObjMgr, "BeamRectGroupODL");
				CWallODLSPtr spBeam = std::static_pointer_cast<CWallODL>(CShapeTool::CreateBeamPillar(arrPoint,  400.0f, pGroup));
				if (spBeam)
				{
					spBeam->SetType(EOBT_Beam);
					float fXLen = rtSize.Width;
					float fYLen = 400.0f;
					float fZLen = rtSize.Height;
					spBeam->SetSize(fXLen, fYLen, fZLen);
					spBeam->SetYPos(1400.0f);
				}

				//最后完成
				CBaseODLSPtr bsPtr = std::dynamic_pointer_cast<CBaseODL>(m_pGuideRect);
				m_pObjMgr->RemoveChild(bsPtr);
				m_pGuideRect = nullptr;
			}
		}
		m_eCurOpt = EOT_Create_None;
	}
}

void CCtrlFuncView::CreatePillarRect( Gdiplus::PointF& ptPos )
{
	//要创建墙
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	if (!m_pGuideRect)
	{
		m_pGuideRect = std::make_shared<CGuideRectODL>();
		m_pObjMgr->AddChild(m_pGuideRect);
		m_pGuideRect->SetTopStartPos(stInfo);
		m_bIsDrawing= TRUE;
	}
	else
	{
		//完成
		if (m_bIsDrawing)
		{
			//结束画墙
			m_bIsDrawing=FALSE;
			m_pGuideRect->SetTopStopPos(stInfo);
			if (m_pGuideRect)
			{
				Gdiplus::RectF rtSize;
				m_pGuideRect->GetSideRect(rtSize);
				//根据CurrentObj的点位置，生成墙壁
				std::vector<gp_Pnt> arrPoint;
				m_pGuideRect->GetTopPointList(arrPoint);

				auto pGroup=std::make_shared<CObjGroupODL>();
				pGroup->InitByParent(m_pObjMgr, "PillarRectGroupODL");

				CWallODLSPtr spPillar = std::static_pointer_cast<CWallODL>(CShapeTool::CreateBeamPillar(arrPoint, m_fWallHeight, pGroup));
				if (spPillar)
				{
					spPillar->SetType(EOBT_Pillar);
					float fXLen = rtSize.Width;
					float fYLen = m_fWallHeight;
					float fZLen = rtSize.Height;
					spPillar->SetSize(fXLen, fYLen, fZLen);
					spPillar->SetYPos(0.0f);
				}

				//最后完成
				CBaseODLSPtr bsPtr = std::dynamic_pointer_cast<CBaseODL>(m_pGuideRect);
				m_pObjMgr->RemoveChild(bsPtr);
				m_pGuideRect = nullptr;
			}
		}
		m_eCurOpt = EOT_Create_None;
	}
}

void CCtrlFuncView::OnTopMoveStart( Gdiplus::PointF& ptPos )
{
	m_pMoveStartPoint = ptPos;
	CString strText=_T("");
	//清除异常状态
	gp_Pnt stInfo;
	stInfo.SetX(ptPos.X);
	stInfo.SetY(0);
	stInfo.SetZ(ptPos.Y);
	m_pCurMoveObj = nullptr;
	auto vertex = BRepBuilderAPI_MakeVertex(stInfo).Vertex();
	auto line = gce_MakeLin(stInfo, gp::DY()).Value();
	auto edge = BRepBuilderAPI_MakeEdge(line).Edge();
	m_pObjMgr->SetIsTopSelected(false,true);
	std::vector<CBaseODLSPtr> arrObj;
	
	CBaseODLSPtr spCam=nullptr;
	CBaseODLSPtr spFloor=nullptr;
	CBaseODLSPtr spWall=nullptr;
	CBaseODLSPtr spBeam=nullptr;
	CBaseODLSPtr spPillar=nullptr;
	CBaseODLSPtr spDecor=nullptr;
	GetTopOnMouseObj(edge, m_pObjMgr, nullptr, arrObj);
	if (arrObj.size()>0)
	{
		for (auto& curObj:arrObj)
		{
			//同一个位置超过2个对象的，首先处理摄像机，再处理梁，再处理柱头，再处理地板
			if ((curObj->GetType()==EOBT_Camera) && (!spCam))
			{
				spCam = curObj;
			}
			else if ((curObj->GetType()==EOBT_Decor) && (!spDecor))
			{
				spDecor = curObj;
			}
			else
			if ((curObj->GetType()==EOBT_Beam) && (!spBeam))
			{
				spBeam = curObj;
			}
			else
			if ((curObj->GetType()==EOBT_Floor) && (!spFloor))
			{
				spFloor = curObj;
			}
			else
			if ((curObj->GetType()==EOBT_Wall) && (!spWall))
			{
				spWall = curObj;
			}
			else
			if ( (curObj->GetType()==EOBT_Pillar) && (!spPillar))
			{
				spPillar = curObj;
			}
		}
		if (spCam)
		{
			//照相机移动，不关联移动
			spCam->SetIsTopSelected(true);
			m_pCurMoveObj = spCam;
		}
		else if (spDecor)
		{
			spDecor->SetIsTopSelected(true);
			m_pCurMoveObj = spDecor;
		}
		else if (spBeam)
		{
			spBeam->SetIsTopSelected(true);
			m_pCurMoveObj = spBeam;
		}
		else if (spPillar)
		{
			spPillar->SetIsTopSelected(true);
			m_pCurMoveObj = spPillar;
		}
		else if (spFloor)
		{
			//选中的如果是地板，那么移动的是分组
			spFloor->SetIsTopSelected(true);
			auto& sp = spFloor->GetParent();
			auto& spSet = sp.lock();
			m_pCurMoveObj = spSet;
		}
		else if (spWall)
		{
			spWall->SetIsTopSelected(true);
			auto& sp = spWall->GetParent();
			auto& spSet = sp.lock();
			m_pCurMoveObj = spSet;
		}
		if (m_pCurMoveObj)
		{
			//会自动变形的Decor，不主动移动
			if (spDecor!=m_pCurMoveObj)
			{
				m_pCurMoveObj->OnTopMoveStart(stInfo);
			}
		}
	}
	m_pCurEditObj=nullptr;
	SetPointInObjFocused(m_pObjMgr, stInfo, m_pCurEditObj);
}

int CCtrlFuncView::OnTopMoveGoing( Gdiplus::PointF& ptPos )
{
	Gdiplus::PointF ptCur = ptPos - m_pMoveStartPoint;
	HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
	::SetCursor(hCur);
	if (m_pMoveStartPoint.Equals(ptPos))
	{
		return 0;
	}
	int nCount = 0;
	//左键下去,判断有没有东西被选中，没有则显示 Drag区域
	if (m_pCurMoveObj)
	{
		if (m_pCurMoveObj->GetType()==EOBT_Decor)
		{
			gp_Pnt stInfo;
			stInfo.SetX(ptPos.X);
			stInfo.SetY(0);
			stInfo.SetZ(ptPos.Y);
			//判断当前装饰品是否处于墙位置内
			CDecorODLSPtr spDecor = std::static_pointer_cast<CDecorODL>(m_pCurMoveObj);
			
			auto vertex = BRepBuilderAPI_MakeVertex(stInfo).Vertex();
			auto line = gce_MakeLin(stInfo, gp::DY()).Value();
			auto edge = BRepBuilderAPI_MakeEdge(line).Edge();
			gp_XYZ szDecor = spDecor->GetRawSize();

			CBaseODLSPtr spRet = GetTopOnMouseObj(edge, m_pObjMgr, m_pCurMoveObj);
			if ((spRet) &&(spRet->GetType()==EOBT_Wall))
			{
				//在墙里面
				CWallODLSPtr spWall = std::static_pointer_cast<CWallODL>(spRet);
				auto spDecParent = spDecor->GetParent().lock();
				if (spDecParent!=spRet)
				{
					spRet->AddChild(spDecor);
					spDecParent->UpdateMesh();
				}
				auto& start = spRet->GetTopStart();
				auto& stop = spRet->GetTopStop();
				gp_Pnt ptPosGP (ptPos.X, 0.0, ptPos.Y);

				auto edge = BRepBuilderAPI_MakeEdge(BRepBuilderAPI_MakeVertex(start), BRepBuilderAPI_MakeVertex(stop)).Edge();
				BRepExtrema_DistShapeShape dss(edge, BRepBuilderAPI_MakeVertex(ptPosGP));

				assert(0 != dss.NbSolution());

				auto& adjustPnt = dss.PointOnShape1(1);
				if (adjustPnt.IsEqual(ptPosGP, Precision::Confusion()))
				{
					//2个点相等，不变形
					gp_Pnt ptPosGP (ptPos.X, 0.0, ptPos.Y);
					spDecor->SetHoleRotation(0,0,0);
					spDecor->SetSize(static_cast<float>(szDecor.X()), static_cast<float>(szDecor.Y()), static_cast<float>(szDecor.Z()));
					spDecor->SetPos(ptPosGP);
				}
				else
				{
					
					gp_Dir dir(stop.XYZ()-start.XYZ());
					auto angle = dir.AngleWithRef(gp::DX(), gp::DY());
					//angle = std::fmod(angle, M_PI);
					spDecor->SetHoleRotation(0, static_cast<Gdiplus::REAL>(angle), 0);

					gp_XYZ szWall = spWall->GetSize();
					spDecor->SetSize(static_cast<float>(szDecor.X()), static_cast<float>(szDecor.Y()), static_cast<float>(szWall.Z()));

					gp_Dir wallDir(gp_Vec(adjustPnt,ptPosGP));
					auto wallVec = wallDir.XYZ().Multiplied(szWall.Z());
					gp_Pnt ptFinal =adjustPnt.XYZ()+wallVec;
					spDecor->SetPos(ptFinal);
				}
				spRet->UpdateMesh();

			}
			else if (spRet)
			{
				gp_Pnt ptPosGP (ptPos.X, 0.0, ptPos.Y);
				spDecor->SetHoleRotation(0,0,0);
				spDecor->SetSize(static_cast<float>(szDecor.X()), static_cast<float>(szDecor.Y()), static_cast<float>(szDecor.Z()));
				spDecor->SetPos(ptPosGP);
				auto spDecParent = spDecor->GetParent().lock();
				if (spDecParent!=spRet)
				{
					spRet->AddChild(spDecor);
					spDecParent->UpdateMesh();
				}
				spRet->UpdateMesh();
				spDecor->UpdateMesh();
			}
		}
		else
		{

			gp_Pnt stInfo;
			stInfo.SetX(ptCur.X);
			stInfo.SetY(0);
			stInfo.SetZ(ptCur.Y);
			m_pCurMoveObj->OnTopMoving(stInfo);
		}
		nCount++;
	}
	return nCount;
}

int CCtrlFuncView::OnTopMoveEnd( Gdiplus::PointF& ptPos )
{
	int nCount=0;
	Gdiplus::PointF ptCur = ptPos - m_pMoveStartPoint;
	if (m_pCurMoveObj)
	{
		m_pCurMoveObj->GetParent().lock()->UpdateMesh();
		if (m_pCurMoveObj->GetType()==EOBT_Decor)
		{
			//如果当前移动的是装饰，修改到当前所属的对象中
			m_pCurMoveObj->SetIsTopMoving(false);
		}
		else
		{
			gp_Pnt stInfo;
			stInfo.SetX(ptCur.X);
			stInfo.SetY(0);
			stInfo.SetZ(ptCur.Y);

			m_pCurMoveObj->OnTopMovingFinish(stInfo,true);
			nCount++;
		}
	}
	return nCount;
}

void CCtrlFuncView::OnTopMoveScene( Gdiplus::PointF& ptPos )
{
	HCURSOR hCur  =  LoadCursor( NULL  , IDC_HAND ) ;
	::SetCursor(hCur);
	Gdiplus::PointF ptX(ptPos);
	float fX = CDrawEngine::GetInstance().GetTopOffsetX();
	float fY = CDrawEngine::GetInstance().GetTopOffsetY();
	ptX = ptX -  m_pMoveStartPoint;
	CDrawEngine::GetInstance().SetTopOffset(fX+ptX.X, fY+ptX.Y);
}

CBaseODLSPtr CCtrlFuncView::GetSidePointInLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj)
{
	if (!spObj)
	{
		return nullptr;
	}
	for (auto& curItem : spObj->GetChildrenList() )
	{
		CBaseODLSPtr spOD = GetSidePointInLeafObj(ptPos, curItem);
		if (spOD)
		{
			return spOD;
		}
		else
		{
			ENUM_RECT_SELECTED_POS ePos = curItem->IsSidePointInside(ptPos);
			if (ePos==ERSP_All)
			{
				return curItem;
			}
		}
	}
	return nullptr;
}

int CCtrlFuncView::GetSidePointAtLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj, std::vector<CBaseODLSPtr>& arrResizeObj)
{
	if (!spObj)
	{
		return 0;
	}
	std::vector<CBaseODLSPtr> arrSubObj;
	for (auto& curItem : spObj->GetChildrenList() )
	{
		Gdiplus::PointF pt(ptPos);
		if (GetSidePointAtLeafObj(ptPos, curItem, arrSubObj)>0)
		{
			arrResizeObj = arrSubObj;
			return arrSubObj.size();
		}
		else
		{
			ENUM_RECT_SELECTED_POS ePos = curItem->IsSidePointInside(pt);
			if ((ePos!=ERSP_None) && (ePos!=ERSP_All))
			{
				arrResizeObj.push_back(curItem);
			}
		}
	}
	return arrResizeObj.size();
}

CBaseODLSPtr CCtrlFuncView::GetSidePointAtLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj)
{
	if (!spObj)
	{
		return nullptr;
	}
	std::vector<CBaseODLSPtr> arrSubObj;
	for (auto& curItem : spObj->GetChildrenList() )
	{
		CBaseODLSPtr sp=GetSidePointAtLeafObj(ptPos, curItem);
		if (sp)
		{
			return sp;
		}
		else
		{
			ENUM_RECT_SELECTED_POS ePos = curItem->IsSidePointInside(ptPos);
			if ((ePos!=ERSP_None) && (ePos!=ERSP_All))
			{
				return curItem;
			}
		}
	}
	return nullptr;
}

int CCtrlFuncView::SetSideSelectedObj(Gdiplus::PointF& ptPos, CBaseODLSPtr spObj)
{
	int nCount = 0;
	if (!spObj)
	{
		return 0;
	}
	for (auto& curItem : spObj->GetChildrenList() )
	{
		int nSub =SetSideSelectedObj(ptPos, curItem);
		if (nSub>0)
		{
			curItem->SetIsSideSelected(false);
			nCount +=nSub;
		}
		else
		{
			ENUM_RECT_SELECTED_POS ePos = curItem->IsSidePointInside(ptPos);
			if (ePos==ERSP_All)
			{
				nCount++;
			}
			curItem->SetIsSideSelected(ePos==ERSP_All);
		}
	}
	return nCount;
}

void CCtrlFuncView::OnSideMoveStart( Gdiplus::PointF& ptPos )
{
	//这个函数只给区域选中使用
	m_pCurResizeObj = nullptr;
	m_pCurMoveObj = nullptr;
	m_pMoveStartPoint = ptPos;
	//清除异常状态
	m_pObjMgr->SetClearSideState(true);
	//1次只能1个操作，要么变形，要么移动
	if (!m_pCurEditObj)
	{
		return;
	}
	if (m_pCurEditObj->GetType()==EOBT_Wall_Face)
	{
		//当前对象是墙面，看看是不是选中的Decor
		CWallFaceODLSPtr spWallFace = std::static_pointer_cast<CWallFaceODL>(m_pCurEditObj);
		//发现有装饰，看看鼠标在里边没有
		for (auto& wpHole:spWallFace->GetHoleList())
		{
			CBaseODLSPtr spHole = wpHole.lock();
			if (spHole->IsSidePointInside(ptPos))
			{
				m_pCurMoveObj = spHole;
				m_pCurMoveObj->OnSideMoveStart(m_pMoveStartPoint);
				break;
			}
		}
	}
	if (!m_pCurMoveObj)
	{
		//选择
		m_pCurResizeObj = GetSidePointAtLeafObj(m_pMoveStartPoint, m_pCurEditObj);
		if (m_pCurResizeObj)
		{
			m_pCurResizeObj->OnSideReSizeStart(m_pMoveStartPoint);
		}
		else
		{
			m_pCurMoveObj= GetSidePointInLeafObj(m_pMoveStartPoint, m_pCurEditObj);
			if (m_pCurMoveObj)
			{
				m_pCurMoveObj->OnSideMoveStart(m_pMoveStartPoint);
			}
		}
	}
}

int CCtrlFuncView::OnSideMoveGoing( Gdiplus::PointF& ptPos )
{
	Gdiplus::PointF ptCur = ptPos - m_pMoveStartPoint;
	int nCount = 0;
	//左键下去,判断有没有东西被选中，没有则显示 Drag区域
	//鼠标左键按下时，选中的是谁，现在还应该是谁
	CBaseODLSPtr sp=nullptr;
	if (m_bLButtonDown)
	{
		if (m_pCurResizeObj)
		{
			sp = m_pCurResizeObj;
			m_pCurResizeObj->OnSideReSizing(ptPos);
			nCount++;
		}
		else
			if (m_pCurMoveObj)
			{
				sp = m_pCurMoveObj;
				m_pCurMoveObj->OnSideMoving(ptCur);
				nCount++;
			}
	}
	else
	{
		sp = GetSidePointAtLeafObj(ptPos, m_pCurEditObj);
		if (!sp)
		{
			sp = GetSidePointInLeafObj(ptPos, m_pCurEditObj);
		}
			
	}
	if (sp)
	{
		HCURSOR hCur =  LoadCursor( NULL  , IDC_ARROW ) ;;
		//有东西在，我要边鼠标指针
		switch (sp->GetSideSelectedPos())
		{
		case ERSP_LeftTop:
		case ERSP_RightBottom:
			{
				//左上
				hCur =  LoadCursor( NULL  , IDC_SIZENWSE ) ;
			}
			break;
		case ERSP_Top:
		case ERSP_Bottom:
			{
				//上下
				hCur =  LoadCursor( NULL  , IDC_SIZENS ) ;
			}
			break;
		case ERSP_RightTop:
		case ERSP_LeftBottom:
			{
				//右上zuoxia
				hCur =  LoadCursor( NULL  , IDC_SIZENESW ) ;
			}
			break;
		case ERSP_Right:
		case ERSP_Left:
			{
				//左右
				hCur =  LoadCursor( NULL  , IDC_SIZEWE ) ;
			}
			break;
		case ERSP_All:
			{
				hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
			}
			break;
		default:
			{
				hCur =  LoadCursor( NULL  , IDC_ARROW ) ;
			}
			break;
		}
		::SetCursor(hCur);
	}
	
	return nCount;
}

int CCtrlFuncView::OnSideMoveEnd( Gdiplus::PointF& ptPos )
{
	int nCount=0;
	if (!m_pMoveStartPoint.Equals(ptPos))
	{
		Gdiplus::PointF ptCur = ptPos - m_pMoveStartPoint;
		//开始的时候选的是谁，现在还是谁
		if (m_pCurResizeObj)
		{
			m_pCurResizeObj->OnSideReSizeEnd(ptPos);
		}
		else
		if (m_pCurMoveObj)
		{
			m_pCurMoveObj->OnSideMovingFinish(ptPos,true);
			if (m_pCurMoveObj->GetType()==EOBT_Decor)
			{
				auto spParent = m_pCurMoveObj->GetParent().lock();
				if (m_pCurEditObj->GetType()==EOBT_Wall_Face)
				{
					Gdiplus::RectF rtRet;//返回的当前Decor在面上的相对位置和大小
					m_pCurMoveObj->GetSideRect(rtRet);
					//上级对象是Wall_Face，完了，要更新洞
					//Update Hole size and Offset
					auto withHoles = std::static_pointer_cast<IWithHoleODL>(spParent);
					auto hole = std::static_pointer_cast<IHoleODL>(m_pCurMoveObj);
					std::static_pointer_cast<IWithHoleODL>(m_pCurEditObj)->UpdateHoleRect(std::make_tuple(hole, gp_Pnt2d(rtRet.X, rtRet.Y), gp_XY(rtRet.Width, rtRet.Height)));
					spParent->UpdateMesh();
				}
			}
			nCount++;
		}
	}
	return nCount;
}

void CCtrlFuncView::OnSideMoveScene( Gdiplus::PointF& ptPos )
{
	HCURSOR hCur  =  LoadCursor( NULL  , IDC_HAND ) ;
	::SetCursor(hCur);
	Gdiplus::PointF ptX(ptPos);
	float fX = CDrawEngine::GetInstance().GetSideOffsetX();
	float fY = CDrawEngine::GetInstance().GetSideOffsetY();
	ptX = ptX -  m_pMoveStartPoint;
	CDrawEngine::GetInstance().SetSideOffset(fX+ptX.X, fY+ptX.Y);
}

void CCtrlFuncView::OnDrawRect(Gdiplus::PointF ptBegin, Gdiplus::PointF ptEnd)
{
	m_pMoveStartPoint = ptBegin;
	m_pMoveStopPoint = ptEnd;
	//if (!bIsDrag)
	{
		if (m_eCurViewType==EVT_TOP)
		{

			//bool bIsCut = m_pMoveStartPoint.X>m_pMoveStopPoint.X;
			////判断设置的独立对象是否被选中
			//gp_Pnt stStart(ptBegin.X,0,ptBegin.Y);
			//gp_Pnt stStop(ptEnd.X,0,ptEnd.Y);

			//BRepBuilderAPI_MakePolygon ply;
			//ply.Add(stStart);
			//ply.Add(gp_Pnt(stStart.X(), stStart.Y(), stStop.Z()));
			//ply.Add(stStop);
			//ply.Add(gp_Pnt(stStop.X(), stStop.Y(), stStart.Z()));

			//TopoDS_Face bFace;
			//if ( Standard_True == ply.IsDone() )
			//{
			//	ply.Close();
			//	bFace = BRepBuilderAPI_MakeFace(ply.Wire());
			//}

			//auto vertex = BRepBuilderAPI_MakeVertex(stStart).Vertex();

			//BRepExtrema_DistShapeShape dss;
			//dss.LoadS2(bFace);

			//for (auto& curItem: m_pObjMgr->GetChildrenList() )
			//{
			//	if (curItem->IsTopCreating())
			//	{
			//		//有对象正在画，暂定不执行选择操作
			//		return;
			//	}

			//	if ( bFace.IsNull() )
			//	{
			//		curItem->SetIsTopSelected(curItem->HasTopCollisionWith( vertex) );
			//		continue;
			//	}

			//	if (bIsCut)
			//	{
			//		curItem->SetIsTopSelected(curItem->HasTopCollisionWith( bFace) );
			//	}
			//	else
			//	{
			//		curItem->SetIsTopSelected(curItem->IsFaceInShape(dss) );
			//	}
			//}
		}
	}
}


//void CCtrlFuncView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnKeyUp(nChar, nRepCnt, nFlags);
//}


//void CCtrlFuncView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnKeyDown(nChar, nRepCnt, nFlags);
//}


void CCtrlFuncView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//左键双击
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{

		}
		break;
	case EVT_FACE:
		{
			if (m_pCurEditObj)
			{
				Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
				CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);
				//获取当前被选中的对象
				CBaseODLSPtr spThis= GetSidePointAtLeafObj(pt, m_pCurEditObj);
				if (!spThis)
				{
					spThis= GetSidePointInLeafObj(pt, m_pCurEditObj);
				}
				if (spThis)
				{
					switch (spThis->GetType())
					{
					case EOBT_Wall_Face_Zone:
					case EOBT_Floor_Zone:
						{
							//Resize TO Parent's sub all dot inside
							auto sp=spThis->GetParent().lock();

							Gdiplus::RectF rtSelf;
							spThis->GetSideRect(rtSelf);
							switch (spThis->GetSideSelectedPos())
							{
							case ERSP_LeftTop:
								{
									//this在左上
									Gdiplus::REAL fXPos = 0.0f;
									Gdiplus::REAL fYPos = 0.0f;
									sp->GetSideMinLimitXPos(spThis, fXPos);
									sp->GetSideMinLimitYPos(spThis, fYPos);
									Gdiplus::REAL fRight = rtSelf.GetRight();
									Gdiplus::REAL fBottom = rtSelf.GetBottom();
									rtSelf.X = fXPos;
									rtSelf.Y = fYPos;
									rtSelf.Width = fRight - fXPos;
									rtSelf.Height = fBottom - fYPos;

								}
								break;
							case ERSP_Top:
								{
									//this在上
									Gdiplus::REAL fYPos = 0.0f;
									sp->GetSideMinLimitYPos(spThis, fYPos);
									Gdiplus::REAL fBottom = rtSelf.GetBottom();
									rtSelf.Y = fYPos;
									rtSelf.Height = fBottom - fYPos;

								}
								break;
							case ERSP_RightTop:
								{
									//this在右上
									Gdiplus::REAL fXPos = 0.0f;
									Gdiplus::REAL fYPos = 0.0f;
									sp->GetSideMaxLimitXPos(spThis, fXPos);
									sp->GetSideMinLimitYPos(spThis, fYPos);
									Gdiplus::REAL fLeft = rtSelf.GetLeft();
									Gdiplus::REAL fBottom = rtSelf.GetBottom();
									rtSelf.Y = fYPos;
									rtSelf.Width = fXPos - fLeft;
									rtSelf.Height = fBottom - fYPos;

								}
								break;
							case ERSP_Right:
								{
									//this在右,x1
									Gdiplus::REAL fXPos = 0.0f;
									sp->GetSideMaxLimitXPos(spThis, fXPos);
									Gdiplus::REAL fLeft = rtSelf.GetLeft();
									Gdiplus::REAL fBottom = rtSelf.GetBottom();
									rtSelf.Width = fXPos - fLeft;

								}
								break;
							case ERSP_RightBottom:
								{
									//this在右下
									Gdiplus::REAL fXPos = 0.0f;	//x1,y1
									Gdiplus::REAL fYPos = 0.0f;
									sp->GetSideMaxLimitXPos(spThis, fXPos);
									sp->GetSideMaxLimitYPos(spThis, fYPos);
									Gdiplus::REAL fLeft = rtSelf.GetLeft();
									Gdiplus::REAL fTop = rtSelf.GetTop();
									rtSelf.Width = fXPos - fLeft;
									rtSelf.Height = fYPos - fTop;

								}
								break;
							case ERSP_Bottom:
								{
									//this在下,y1
									Gdiplus::REAL fYPos = 0.0f;
									sp->GetSideMaxLimitYPos(spThis, fYPos);
									Gdiplus::REAL fTop = rtSelf.GetTop();
									rtSelf.Height = fYPos - fTop;

								}
								break;
							case ERSP_LeftBottom:
								{
									//this在左下
									Gdiplus::REAL fXPos = 0.0f;	//x0,y1
									Gdiplus::REAL fYPos = 0.0f;
									sp->GetSideMinLimitXPos(spThis, fXPos);
									sp->GetSideMaxLimitYPos(spThis, fYPos);
									Gdiplus::REAL fRight = rtSelf.GetRight();
									Gdiplus::REAL fTop = rtSelf.GetTop();
									rtSelf.X = fXPos;
									rtSelf.Width = fRight - fXPos;
									rtSelf.Height = fYPos - fTop;

								}
								break;
							case ERSP_Left:
								{
									//this在左,x0
									Gdiplus::REAL fXPos = 0.0f;
									sp->GetSideMinLimitXPos(spThis, fXPos);
									Gdiplus::REAL fRight = rtSelf.GetRight();
									rtSelf.X = fXPos;
									rtSelf.Width = fRight - fXPos;

								}
								break;
							case ERSP_All:
								{
									//最小点全屏
									Gdiplus::REAL fXPos1=0.0f;
									Gdiplus::REAL fXPos2=0.0f;
									Gdiplus::RectF rtRet;
									sp->GetSideFitRect(spThis, true, false, rtRet);
									if (rtRet.Width>1.0f)
									{
										rtSelf = rtRet;
									}
								}
								break;
							}
							spThis->SetSideSize(rtSelf);
							spThis->UpdateMesh();
						}
						break;
					}
				}
			}
		}
		break;
	}
	CView::OnLButtonDblClk(nFlags, point);
}
