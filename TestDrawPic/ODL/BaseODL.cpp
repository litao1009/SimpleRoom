#include "stdafx.h"

#include "BaseODL.h"
#include "WallFaceZoneODL.h"
#include "FloorZoneODL.h"

#include "SceneNode/CDataSceneNode.h"

#include "BRepExtrema_DistShapeShape.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepFeat.hxx"
#include "TopoDS.hxx"
#include "GCE_MakeLin.hxx"
#include "BRep_Tool.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "TopExp_Explorer.hxx"
#include "TopExp.hxx"
#include "Precision.hxx"
#include "BRepPrimAPI_MakePrism.hxx"

#include <algorithm>

CBaseODL::CBaseODL(void)
{
	m_bIsCanDelete = true;

	m_bIsTopSelected=false;
	m_bIsTopFocused = false;
	m_bIsTopMoving=false;
	m_bIsTopCreating=false;
	m_bIsTopResizing = false;

	m_bIsCurrentHotspot=false;

	m_bIsLocked=false;
	m_bIsHide = false;

	m_eSelectedPos = ERSP_None;
	m_nIndex = 0;

	m_ObjType = EOBT_Base;
	m_clPenColor = Gdiplus::Color::Black;
	m_clDotColor = Gdiplus::Color::Black;

	m_clResizeColor = Gdiplus::Color::LemonChiffon;
	m_spSkin = nullptr;

	m_pFontFamily = new Gdiplus::FontFamily(_T("����"));
	m_pFontBrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0));  //��͸��+����RGB��ɫ
	
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);
}


CBaseODL::~CBaseODL(void)
{
	delete m_pFontFamily;
	delete m_pFontBrush;
}

void CBaseODL::InitTopColor()
{
	m_clPenColor = Gdiplus::Color::Black;
	m_clDotColor = Gdiplus::Color::Black;
	if (IsTopCreating())
	{
		//��һ��Ŀ��λ�õĶ���
		m_clPenColor = Gdiplus::Color::Red;
		m_clDotColor = Gdiplus::Color::Red;
	}
	if (IsTopSelected())
	{
		m_clPenColor = Gdiplus::Color::Brown;
		m_clDotColor = Gdiplus::Color::Brown;
	}

	if (IsTopFocused())
	{
		//��һ��Ŀ��λ�õĶ���
		m_clPenColor = Gdiplus::Color(0,255,255);
		m_clDotColor = Gdiplus::Color(0,255,255);
	}
	if (IsTopMoving())
	{
		//��һ��Ŀ��λ�õĶ���
		m_clPenColor = Gdiplus::Color::Green;
		m_clDotColor = Gdiplus::Color::Green;
	}
}

void CBaseODL::OnDrawTop( Gdiplus::Graphics& gcDrawer, float fScale)
{
	if (IsHide())
	{
		return;
	}
	InitTopColor();
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnDrawTop(gcDrawer, fScale);
	}
}

void CBaseODL::InitSideColor()
{
	m_clPenColor = Gdiplus::Color::Black;
	m_clDotColor = Gdiplus::Color::Black;
	if (IsSideMoving())
	{
		//��һ��Ŀ��λ�õĶ���
		m_clPenColor = Gdiplus::Color::Green;
		m_clDotColor = Gdiplus::Color::Green;
	}
	if (IsSideResizing())
	{
		m_clPenColor = Gdiplus::Color::Blue;
		m_clDotColor = Gdiplus::Color::Blue;
	}
	if (IsSideSelected())
	{
		m_clPenColor = Gdiplus::Color::Brown;
		m_clDotColor = Gdiplus::Color::Brown;
	}
	if (IsSideFocused())
	{
		//��һ��Ŀ��λ�õĶ���
		m_clPenColor = Gdiplus::Color(0,255,255);
		m_clDotColor = Gdiplus::Color(0,255,255);
	}
}

void CBaseODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale)
{
	if (IsHide())
	{
		return;
	}
	InitSideColor();
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnDrawFace(gcDrawer, fScale);
	}
}

void CBaseODL::OnTopMoveStart(const gp_Pnt& stInfo)
{
	if (IsLocked())
	{
		return;
	}
	SetIsTopMoving(true);
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnTopMoveStart(stInfo);
	}
}

void CBaseODL::OnTopMoving(const gp_Pnt& stInfo)
{
	if (IsLocked())
	{
		return;
	}
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnTopMoving(stInfo);
	}
}

void CBaseODL::OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange )
{
	if (IsLocked())
	{
		return;
	}

	m_bIsTopMoving=false;
	if ( !bCommitChange )
	{
		return;
	}
	
	if ( m_FaceShape.IsValid() )
	{
		gp_Trsf newPos;
		newPos.SetTranslation(gp_Pnt(0,0,0), stInfo);
		GetFaceShape().SetTranslation(newPos);
	}

	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnTopMovingFinish(stInfo, bCommitChange);
	}
}

void CBaseODL::CancelTopMoving()
{
	m_bIsTopMoving=false;
	//�޸����е������λ��

	for ( auto& curChild : GetChildrenList() )
	{
		curChild->CancelTopMoving();
	}
}

CBaseODLSPtr CBaseODL::CreateSub(const Gdiplus::PointF& ptPos, CSkinUnitODLSPtr spSkin)
{
	//ȫ������- ����X,Y��ƫ��ֵ=��ǰ �����ڵ�λ������
	Gdiplus::PointF ptCurrent = ptPos - m_ptSideGlobalOffset;
	//�ڵ�ǰ������괴��һ����ʼ����
	Gdiplus::RectF rtSubAreaNew(ptCurrent.X , ptCurrent.Y, 1.0f, 1.0f);
	Gdiplus::RectF rtSubAreaOld(ptCurrent.X, ptCurrent.Y, 1.0f, 1.0f);
	//�������СΪ0
	if (ptCurrent.X<0.000001f)
	{
		rtSubAreaOld.X = 0.00001f;
		rtSubAreaNew.X = 0.00001f;
	}
	if (ptCurrent.Y<0.000001f)
	{
		rtSubAreaOld.Y = 0.00001f;
		rtSubAreaNew.Y = 0.00001f;
	}
	//�ڵ�ǰ�����ڲ����¼����Σ��ó�����ʹ������Ĵ�С�����Ҿ�����Ҫ������վ���
	if (!GetSideFitRect(nullptr, false, false, rtSubAreaNew))
	{
		AfxMessageBox(_T("���ܴ�����"));
		return nullptr;
	}
	//��ʼ�����¼�����
	CBaseODLSPtr spZone=nullptr;
	if ((m_ObjType==EOBT_Wall_Face) || (m_ObjType==EOBT_Wall_Face_Zone))
	{
		CWallFaceZoneODLSPtr spWallZone = std::make_shared<CWallFaceZoneODL>();
		spZone = spWallZone;
	}
	else if ((m_ObjType==EOBT_Floor) || (m_ObjType==EOBT_Floor_Zone))
	{
		CFloorZoneODLSPtr spFloorZone = std::make_shared<CFloorZoneODL>();
		spZone = spFloorZone;
	}
	else if (m_ObjType==EOBT_Sky)
	{
		return nullptr;
	}
	else
	{
		return nullptr;
	}
	Gdiplus::REAL fSkinWidth = spSkin->GetSkinWidth();
	Gdiplus::REAL fSkinHeight = spSkin->GetSkinHeight();
	{
		//����Ƥ����С

		spZone->SetSkin(spSkin);
		//����½��ĵ��ڿ�������Χ��
		//�����������ڣ�˵��������Ѿ����������ˣ��������ĵ���Ǹղŷ��صĿ��þ��εĵ�
		if (rtSubAreaNew.Contains(rtSubAreaOld))
		{
			//�ѵ�ǰ���Ͻǵĵ��Ƶ�׼����������ĵ��λ��
			rtSubAreaNew.Width = rtSubAreaNew.GetRight() - rtSubAreaOld.X;
			rtSubAreaNew.Height = rtSubAreaNew.GetBottom() - rtSubAreaOld.Y;
			rtSubAreaNew.X = rtSubAreaOld.X;
			rtSubAreaNew.Y = rtSubAreaOld.Y;
		}
		//����������Ŀ����з�Χ�ڱ����ߴ緶Χ�ڣ�0�㿪ʼ
		Gdiplus::RectF rtLimit(0.0f, 0.0f, m_rtArea.Width, m_rtArea.Height);
		spZone->SetSideLimitArea(rtLimit);
		//Ȼ���ж��Ƿ񱻴����Ķ���ȵ�ǰ����ĳ�������
		Gdiplus::REAL fWidth = fSkinWidth;
		Gdiplus::REAL fHeight = fSkinHeight;
		//����������ͼƬ���ڳ���
		if (fWidth>m_rtArea.Width)
		{
			fWidth = m_rtArea.Width;
		}
		if (fHeight>m_rtArea.Height)
		{
			fHeight = m_rtArea.Height;
		}
		//Ĭ��ͼƬ=һ����ԪͼƬ��С
		if (rtSubAreaNew.Width<fSkinWidth)
		{
			fWidth = rtSubAreaNew.Width;
		}
		if (rtSubAreaNew.Height<fSkinHeight)
		{
			fHeight = rtSubAreaNew.Height;
		}
		//���õ�ǰ�����Ķ����С��Ҫô=Ƥ����С�����Ƥ��̫�󣬾�=������С
		rtSubAreaNew.Width = fWidth;
		rtSubAreaNew.Height = fHeight;
		//����������ͼƬλ�ó�������
		if (rtSubAreaNew.X+rtSubAreaNew.Width>m_rtArea.Width)
		{
			rtSubAreaNew.X = m_rtArea.Width - rtSubAreaNew.Width;
		}
		if (rtSubAreaNew.Y+rtSubAreaNew.Height>m_rtArea.Height)
		{
			rtSubAreaNew.Y = m_rtArea.Height - rtSubAreaNew.Height;
		}
		//������Ϊ���λ�ö�ƫ�Ƶ���������
		//�޸ı���������ĳߴ�����λ��
		spZone->SetSideSize(rtSubAreaNew);
		//�����¼������ƫ��λ��
		Gdiplus::PointF ptSubOffset(rtSubAreaNew.X, rtSubAreaNew.Y);
		spZone->SetSideGolbalOffset(ptSubOffset + m_ptSideGlobalOffset);
		if ((m_ObjType==EOBT_Wall_Face))
		{
			spZone->InitByParent(shared_from_this(), "WallFaceZoneODL");
		}
		else if (m_ObjType==EOBT_Wall_Face_Zone)
		{
			spZone->InitByParent(shared_from_this(), "SubWallFaceZoneODL");
		}
		else if (m_ObjType==EOBT_Floor)
		{
			spZone->InitByParent(shared_from_this(), "FloorZoneODL");
		}
		else if (m_ObjType==EOBT_Floor_Zone)
		{
			spZone->InitByParent(shared_from_this(), "SubFloorZoneODL");
		}
		spZone->UpdateMesh();
	}
	return spZone;
}

bool CBaseODL::CheckSideIntersected(CBaseODLSPtr spExcept, Gdiplus::RectF& rtCheck)
{
	Gdiplus::PointF ptChkCenter((rtCheck.GetLeft() + rtCheck.GetRight()) / 2.0f, (rtCheck.GetTop() + rtCheck.GetBottom()/2.0f));
	Gdiplus::REAL fChkWidth = rtCheck.Width;
	Gdiplus::REAL fChkHeight = rtCheck.Height;

	for (auto& spItem: GetChildrenList())
	{
		if (spItem==spExcept)
		{
			continue;
		}
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);
		if (rtMe.IntersectsWith(rtCheck))
		{
			//���ǰ����ƶ����Ϸ���λ�ú�Щ
			Gdiplus::PointF ptMeCenter((rtMe.GetLeft() + rtMe.GetRight()) / 2.0f, (rtMe.GetTop() + rtMe.GetBottom()/2.0f));
			Gdiplus::REAL fMeWidth = rtMe.Width;
			Gdiplus::REAL fMeHeight = rtMe.Height;

			Gdiplus::REAL fOffsetY = (ptChkCenter.Y - ptMeCenter.Y);
			Gdiplus::REAL fOffsetX = (ptChkCenter.X -ptMeCenter.X );
			Gdiplus::REAL fArc = static_cast<Gdiplus::REAL>(std::atan( fOffsetY / fOffsetX) * 180 * M_1_PI);
			bool bIsRight= (fOffsetX>=0.0001f);	//true://���ұ�
			bool bIsBottom =(fOffsetY>=0.0001f);	//���±�
			if (std::abs(fArc)<=45.0f)
			{
				//����2��
				if (bIsRight)
				{
					rtCheck.X = rtMe.GetRight();
				}
				else
				{
					rtCheck.X = rtMe.X - rtCheck.Width;
				}
			}
			else
			{
				if (std::abs(fArc)>=45.0f)
				{
					//����2��
					if (bIsBottom)
					{
						rtCheck.Y = rtMe.GetBottom();
					}
					else
					{
						rtCheck.Y = rtMe.Y - rtCheck.Height;
					}
				}
			}
			return true;
		}
	}
	return false;
}

ENUM_RECT_SELECTED_POS CBaseODL::IsSidePointInside(const Gdiplus::PointF& ptPos )
{
	//1�����ϣ�2���ϣ�3�����ϣ�4���ң�5�����£�6���£�7�����£�8����10�����м�)
	Gdiplus::PointF ptCurrent = ptPos - m_ptSideGlobalOffset;
	ptCurrent.X +=m_rtArea.X;
	ptCurrent.Y +=m_rtArea.Y;
	ENUM_RECT_SELECTED_POS eRet= ERSP_None;
	//����ھ�����
	if (m_rtArea.Contains(ptCurrent))
	{
		eRet = ERSP_All;
	}
	//����ڱ���
	Gdiplus::REAL fLeft =m_rtArea.GetLeft();
	Gdiplus::REAL fRight = m_rtArea.GetRight();
	Gdiplus::REAL fTop = m_rtArea.GetTop();
	Gdiplus::REAL fBottom = m_rtArea.GetBottom();
	Gdiplus::REAL fDetectLen = 50.0f;
	if (
		(std::fabs(ptCurrent.X - fLeft)<fDetectLen) && 
		(std::fabs(ptCurrent.Y - fTop)<fDetectLen)
		)
	{
		//����:xin 0,1;y in 0.1
		if (eRet==ERSP_All)
		{
			eRet = ERSP_LeftTop;
		}
	}
	else
		if (
			(ptCurrent.X-fLeft>fDetectLen) && (fRight-ptCurrent.X>fDetectLen) &&
			(std::fabs(ptCurrent.Y - fTop)<fDetectLen)
			)
		{
			//��
			if (eRet==ERSP_All)
			{
				eRet = ERSP_Top;
			}
		}
		else
			if (
				(std::fabs(ptCurrent.X - fRight)<fDetectLen) && 
				(std::fabs(ptCurrent.Y - fTop)<fDetectLen)
				)
			{
				//����
				if (eRet==ERSP_All)
				{
					eRet = ERSP_RightTop;
				}
			}
			else
				if (
					(ptCurrent.Y-fTop>fDetectLen) && (fBottom - ptCurrent.Y>fDetectLen) &&
					(std::fabs(ptCurrent.X - fRight)<fDetectLen)
					)
				{
					//��
					if (eRet==ERSP_All)
					{
						eRet = ERSP_Right;
					}
				}
				else
					if (
						(std::fabs(ptCurrent.X - fRight)<fDetectLen) && 
						(std::fabs(ptCurrent.Y - fBottom)<fDetectLen)
						)
					{
						//����
						if (eRet==ERSP_All)
						{
							eRet = ERSP_RightBottom;
						}
					}
					else
						if (
							(ptCurrent.X-fLeft>fDetectLen) && (fRight - ptCurrent.X >fDetectLen) &&
							(std::fabs(ptCurrent.Y - fBottom)<fDetectLen)
							)
						{
							//��
							if (eRet==ERSP_All)
							{
								eRet = ERSP_Bottom;
							}
						}
						else
							if (
								(std::fabs(ptCurrent.X - fLeft)<fDetectLen) && 
								(std::fabs(ptCurrent.Y - fBottom)<fDetectLen)
								)
							{
								//����
								if (eRet==ERSP_All)
								{
									eRet = ERSP_LeftBottom;
								}
							}
							else
								if (
									(ptCurrent.Y - fTop>fDetectLen) && (fBottom - ptCurrent.Y>fDetectLen) &&
									(std::fabs(ptCurrent.X - fLeft)<fDetectLen)
									)
								{
									//��
									if (eRet==ERSP_All)
									{
										eRet = ERSP_Left;
									}
								}
								m_eSelectedPos = eRet;

								return eRet;
}

void CBaseODL::OnSideMoveStart(const Gdiplus::PointF& ptStart)
{
	if (IsLocked())
	{
		return;
	}
	m_bIsSideMoving=true;

	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnSideMoveStart(ptStart);
	}
}

void CBaseODL::OnSideMoving(const Gdiplus::PointF& ptOffset)
{
	if (IsLocked())
	{
		return;
	}
	
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnSideMoving(ptOffset);
	}
}

void CBaseODL::OnSideMovingFinish(const Gdiplus::PointF& ptOffset, bool bCommitChange )
{
	m_bIsSideMoving=false;
	if (IsLocked())
	{
		return;
	}

	if ( !bCommitChange )
	{
		return;
	}

	for ( auto& curChild : GetChildrenList() )
	{
		curChild->OnSideMovingFinish(ptOffset, bCommitChange);
	}
}

void CBaseODL::UpdateSideGlobalOffset(const Gdiplus::PointF& ptOffset)
{
	//�ϼ�ƫ��
	m_ptSideGlobalOffset = ptOffset;
	//����λ��
	m_ptSideGlobalOffset.X += m_rtArea.X;
	m_ptSideGlobalOffset.Y += m_rtArea.Y;
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->UpdateSideGlobalOffset(m_ptSideGlobalOffset);
	}
}

void CBaseODL::SetIsTopSelected( bool isTopSelected)
{
	m_bIsTopSelected = isTopSelected;
	if (IsHide())
	{
		m_bIsTopSelected=false;
	}
}

void CBaseODL::SetIsTopSelected( bool isTopSelected,bool bSetAll)
{
	m_bIsTopSelected = isTopSelected;
	if (IsHide())
	{
		m_bIsTopSelected=false;
	}
	if (bSetAll)
	{
		for (auto&curItem:GetChildrenList())
		{
			curItem->SetIsTopSelected(isTopSelected, bSetAll);
		}
	}
}

void CBaseODL::SetIsTopFocused( bool bIsFocused,bool bSetAll)
{
	m_bIsTopFocused = bIsFocused;
	if (IsHide())
	{
		m_bIsTopFocused=false;
	}
	if (bSetAll)
	{
		for (auto&curItem:GetChildrenList())
		{
			curItem->SetIsTopFocused(m_bIsTopFocused, bSetAll);
		}
	}
}
void CBaseODL::SetIsSideSelected( bool isSideSelected)
{
	m_bIsSideSelected = isSideSelected;
	if (IsHide())
	{
		m_bIsSideSelected=false;
	}
}


bool CBaseODL::IsFaceInShape( BRepExtrema_DistShapeShape& dss )
{
	if ( !m_FaceShape.IsValid() )
	{
		return false;
	}

	auto& m = m_FaceShape.GetVertex();
	for ( auto index=1; index<=m.Extent(); ++index )
	{
		auto& curVertex = m.FindKey(index);

		dss.LoadS1(curVertex);
		dss.Perform();
		
		if ( 0 == dss.NbSolution() || dss.Value() > Precision::Confusion() )
		{
			return false;
		}
	}

	return true;
}

bool CBaseODL::HasTopCollisionWith( const TopoDS_Shape& theShape )
{
	if ( !m_FaceShape.IsValid() )
	{
		return false;
	}

	auto& dss = GetFaceShape().GetDisSS();
	dss.LoadS1(theShape);
	dss.Perform();

	if ( 0 == dss.NbSolution() )
	{
		return false;
	}

	auto val = dss.Value();
	return val < 50;
}

TopoDS_Shape CBaseODL::GetEdgeByPoint( gp_Pnt ptPos )
{
	if ( !m_FaceShape.IsValid() )
	{
		return TopoDS_Shape();
	}

	auto& wire = m_FaceShape.GetWires().FindKey(1);

	BRepExtrema_DistShapeShape dss(BRepBuilderAPI_MakeVertex(ptPos), wire);

	assert(BRepExtrema_IsOnEdge == dss.SupportTypeShape2(1));

	return dss.SupportOnShape2(1);
}

//////////////////////////////////////////////////////////////////////////
//����������
//////////////////////////////////////////////////////////////////////////

void CBaseODL::GetSidePointList(std::vector<Gdiplus::PointF>& arrPoint)
{
	//��ȡȫ������ϵ��
	arrPoint.emplace_back(Gdiplus::PointF(m_ptSideGlobalOffset.X, m_ptSideGlobalOffset.Y));
	arrPoint.emplace_back(Gdiplus::PointF(m_ptSideGlobalOffset.X + m_rtArea.Width, m_ptSideGlobalOffset.Y));
	arrPoint.emplace_back(Gdiplus::PointF(m_ptSideGlobalOffset.X + m_rtArea.Width, m_ptSideGlobalOffset.Y + m_rtArea.Height));
	arrPoint.emplace_back(Gdiplus::PointF(m_ptSideGlobalOffset.X, m_ptSideGlobalOffset.Y + m_rtArea.Height));
}

//�������ȡ���óߴ�λ��
bool CBaseODL::GetSideFitRect( CBaseODLSPtr spSelect, bool bUpdateSelf, bool bIsMax, Gdiplus::RectF& rtSelf )
{
	//Ĭ������ȫ��
	Gdiplus::RectF rtParent = m_rtArea;
	//�����С
	if (spSelect && bUpdateSelf)
	{
		//�����ǰָ���˴������
		spSelect->GetSideRect(rtSelf);
	}

	//���򣬸��ݴ���rtSelf�����������ͣ����λ��
	Gdiplus::REAL fLeft = rtSelf.GetLeft();
	Gdiplus::REAL fTop = rtSelf.GetTop();
	Gdiplus::REAL fRight = rtSelf.GetRight();
	Gdiplus::REAL fBottom = rtSelf.GetBottom();

	Gdiplus::REAL fX0 = 0.0f;
	Gdiplus::REAL fX1 = rtParent.Width;
	Gdiplus::REAL fY0 = 0.0f;
	Gdiplus::REAL fY1 = rtParent.Height;
	//��������X0/X1,Y0/Y1
	for (auto& spItem: GetChildrenList())
	{
		if (spItem==spSelect)
		{
			continue;
		}
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);

		Gdiplus::REAL fTmpLeft= rtMe.GetLeft();
		Gdiplus::REAL fTmpRight = rtMe.GetRight();
		Gdiplus::REAL fTmpTop = rtMe.GetTop();
		Gdiplus::REAL fTmpBottom = rtMe.GetBottom();

		//�ѵ�ǰ�����õ�λ�ñ��浽fX,fY��

		//1����ѡ�ж���λ�ڵ�ǰ���������ʱ
		if (bIsMax)
		{
			if ((fLeft >= fTmpRight ) && 
				(
				(fTop>=fTmpTop && fTop<=fTmpBottom ) || 
				(fBottom>=fTmpTop && fBottom<=fTmpBottom ) || 
				( (fTop<=fTmpTop) && (fBottom>=fTmpBottom) ))
				)
			{
				//����෶Χ��
				if (fX0 <= fTmpRight)
				{
					fX0 = fTmpRight;
				}
			}
		}
		else
		{
			if ((fLeft >= fTmpRight ) )
			{
				//����෶Χ��
				if (fX0 <= fTmpRight)
				{
					fX0 = fTmpRight;
				}
			}
		}
		if (bIsMax)
		{
			if (
				(fRight <= fTmpLeft) && 
				(
				(fTop>=fTmpTop && fTop<=fTmpBottom ) || 
				(fBottom>=fTmpTop && fBottom<=fTmpBottom ) || 
				( (fTop<=fTmpTop) && (fBottom>=fTmpBottom) )
				)
				)
			{
				//����෶Χ��
				if (fX1 >= fTmpLeft)
				{
					fX1 = fTmpLeft;
				}
			}
		}
		else
		{
			if (
				(fRight <= fTmpLeft)
				)
			{
				//����෶Χ��
				if (fX1 >= fTmpLeft)
				{
					fX1 = fTmpLeft;
				}
			}
		}
		if (bIsMax)
		{
			if ((fTop >= fTmpBottom) &&
				(
				(fLeft>=fTmpLeft && fLeft<=fTmpRight ) || 
				(fRight>=fTmpLeft && fRight<=fTmpRight ) || 
				( (fLeft<=fTmpLeft) && (fRight>=fTmpRight) )
				)
				)
			{
				//���ϲ෶Χ��
				if (fY0 <= fTmpBottom)
				{
					fY0 = fTmpBottom;
				}
			}
		}
		else
		{
			if ((fTop >= fTmpBottom) 
				)
			{
				//���ϲ෶Χ��
				if (fY0 <= fTmpBottom)
				{
					fY0 = fTmpBottom;
				}
			}
		}
		if (bIsMax)
		{
			if ((fBottom <= fTmpTop) && 
				(
				(fLeft>=fTmpLeft && fLeft<=fTmpRight ) || 
				(fRight>=fTmpLeft && fRight<=fTmpRight ) || 
				( (fLeft<=fTmpLeft) && (fRight>=fTmpRight) )
				)
				)
			{
				//���ϲ෶Χ��
				if (fY1 >= fTmpTop)
				{
					fY1 = fTmpTop;
				}
			}
		}
		else
		{
			if ((fBottom <= fTmpTop) 
				)
			{
				//���ϲ෶Χ��
				if (fY1 >= fTmpTop)
				{
					fY1 = fTmpTop;
				}
			}
		}
	}
	if ((std::abs(fX1 - fX0)<=0.0001f) || (std::abs(fY1 - fY0)<=0.0001f))
	{
		return false;
	}
	rtSelf.X = fX0;
	rtSelf.Y = fY0;
	rtSelf.Width = std::fabs(fX1 - fX0);
	rtSelf.Height = std::fabs(fY1 - fY0);
	return true;
}

bool CBaseODL::GetSideMinLimitXPos( CBaseODLSPtr spSelect,  Gdiplus::REAL& fXPos )
{
	Gdiplus::RectF rtParent;
	GetSideRect(rtParent);
	//�����С
	Gdiplus::RectF rtSelf;
	spSelect->GetSideRect(rtSelf);

	//������õķ�Χ�����ʹ�þ����е����
	Gdiplus::REAL fLeft = rtSelf.GetLeft();
	Gdiplus::REAL fTop = rtSelf.GetTop();
	Gdiplus::REAL fBottom = rtSelf.GetBottom();
	Gdiplus::REAL fRight = rtSelf.GetRight();

	Gdiplus::REAL fX0 = 0.0f;

	for (auto& spItem: GetChildrenList())
	{
		if (spItem==spSelect)
		{
			continue;
		}
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);
		Gdiplus::REAL fTmpLeft= rtMe.GetLeft();
		Gdiplus::REAL fTmpRight = rtMe.GetRight();
		Gdiplus::REAL fTmpTop = rtMe.GetTop();
		Gdiplus::REAL fTmpBottom = rtMe.GetBottom();
		//ֻ�ұȵ�ǰ�����leftXС�����ں���ƽ�������ڵģ��򱻵�ǰ���󳬹��߶����µ�
		if ((fLeft >= fTmpRight ) && 
			(
			(fTop>=fTmpTop && fTop<=fTmpBottom ) || 
			(fBottom>=fTmpTop && fBottom<=fTmpBottom ) || 
			( (fTop<=fTmpTop) && (fBottom>=fTmpBottom) ))
			)
		{
			//����෶Χ��
			if (fX0 <= fTmpRight)
			{
				fX0 = fTmpRight;
			}
		}

	}
	fXPos = fX0;

	return true;
}

bool CBaseODL::GetSideMaxLimitXPos( CBaseODLSPtr spSelect,   Gdiplus::REAL& fXPos )
{
	Gdiplus::RectF rtParent;
	GetSideRect(rtParent);
	//�����С
	Gdiplus::RectF rtSelf;
	spSelect->GetSideRect(rtSelf);
	//������õķ�Χ�����ʹ�þ����е����
	Gdiplus::REAL fLeft = rtSelf.GetLeft();
	Gdiplus::REAL fTop = rtSelf.GetTop();
	Gdiplus::REAL fBottom = rtSelf.GetBottom();
	Gdiplus::REAL fRight = rtSelf.GetRight();

	Gdiplus::REAL fX1 = rtParent.Width;

	for (auto& spItem: GetChildrenList())
	{
		if (spItem==spSelect)
		{
			continue;
		}
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);
		Gdiplus::REAL fTmpLeft= rtMe.GetLeft();
		Gdiplus::REAL fTmpRight = rtMe.GetRight();
		Gdiplus::REAL fTmpTop = rtMe.GetTop();
		Gdiplus::REAL fTmpBottom = rtMe.GetBottom();
		//ֻ�ұȵ�ǰ�����leftX�����ں���ƽ�������ڵ�
		if (
			(fRight <= fTmpLeft) && 
			(
			(fTop>=fTmpTop && fTop<=fTmpBottom ) || 
			(fBottom>=fTmpTop && fBottom<=fTmpBottom ) || 
			( (fTop<=fTmpTop) && (fBottom>=fTmpBottom) )
			)
			)
		{
			//����෶Χ��
			if (fX1 >= fTmpLeft)
			{
				fX1 = fTmpLeft;
			}
		}

	}
	fXPos = fX1;
	return true;
}

bool CBaseODL::GetSideMinLimitYPos( CBaseODLSPtr spSelect,  Gdiplus::REAL& fYPos )
{
	Gdiplus::RectF rtParent;
	GetSideRect(rtParent);
	//�����С
	Gdiplus::RectF rtSelf;
	spSelect->GetSideRect(rtSelf);

	//������õķ�Χ�����ʹ�þ����е����
	Gdiplus::REAL fLeft = rtSelf.GetLeft();
	Gdiplus::REAL fTop = rtSelf.GetTop();
	Gdiplus::REAL fBottom = rtSelf.GetBottom();
	Gdiplus::REAL fRight = rtSelf.GetRight();

	Gdiplus::REAL fY0 = 0.0f;

	for (auto& spItem: GetChildrenList())
	{
		if (spItem==spSelect)
		{
			continue;
		}
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);
		Gdiplus::REAL fTmpLeft= rtMe.GetLeft();
		Gdiplus::REAL fTmpRight = rtMe.GetRight();
		Gdiplus::REAL fTmpTop = rtMe.GetTop();
		Gdiplus::REAL fTmpBottom = rtMe.GetBottom();
		//ֻ�ұȵ�ǰ�����leftXС�����ں���ƽ�������ڵ�
		if ((fTop >= fTmpBottom) &&
			(
			(fLeft>=fTmpLeft && fLeft<=fTmpRight ) || 
			(fRight>=fTmpLeft && fRight<=fTmpRight ) || 
			( (fLeft<=fTmpLeft) && (fRight>=fTmpRight) )
			)
			)
		{
			//���ϲ෶Χ��
			if (fY0 <= fTmpBottom)
			{
				fY0 = fTmpBottom;
			}
		}

	}
	fYPos = fY0;
	return true;
}

bool CBaseODL::GetSideMaxLimitYPos( CBaseODLSPtr spSelect,  Gdiplus::REAL& fYPos )
{
	Gdiplus::RectF rtParent;
	GetSideRect(rtParent);
	//�����С
	Gdiplus::RectF rtSelf;
	spSelect->GetSideRect(rtSelf);

	//������õķ�Χ�����ʹ�þ����е����
	Gdiplus::REAL fLeft = rtSelf.GetLeft();
	Gdiplus::REAL fTop = rtSelf.GetTop();
	Gdiplus::REAL fBottom = rtSelf.GetBottom();
	Gdiplus::REAL fRight = rtSelf.GetRight();

	Gdiplus::REAL fY1 = rtParent.Height;

	for (auto& spItem: GetChildrenList())
	{
		if (spItem==spSelect)
		{
			continue;
		}
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);
		Gdiplus::REAL fTmpLeft= rtMe.GetLeft();
		Gdiplus::REAL fTmpRight = rtMe.GetRight();
		Gdiplus::REAL fTmpTop = rtMe.GetTop();
		Gdiplus::REAL fTmpBottom = rtMe.GetBottom();
		//ֻ�ұȵ�ǰ�����leftXС�����ں���ƽ�������ڵ�
		if ((fBottom <= fTmpTop) && 
			(
			(fLeft>=fTmpLeft && fLeft<=fTmpRight ) || 
			(fRight>=fTmpLeft && fRight<=fTmpRight ) || 
			( (fLeft<=fTmpLeft) && (fRight>=fTmpRight) )
			)
			)
		{
			//���ϲ෶Χ��
			if (fY1 >= fTmpTop)
			{
				fY1 = fTmpTop;
			}
		}
	}
	fYPos = fY1;
	return true;
}

bool CBaseODL::GetSideMinRect( Gdiplus::RectF& rtLimit ,int& nCount)
{
	Gdiplus::REAL fMinX = 0.0f;
	Gdiplus::REAL fMaxX = 0.0f;
	Gdiplus::REAL fMinY = 0.0f;
	Gdiplus::REAL fMaxY = 0.0f;
	//��������X0/X1,Y0/Y1
	nCount=0;
	for (auto& spItem: GetChildrenList())
	{
		Gdiplus::RectF rtMe;
		spItem->GetSideRect(rtMe);
		
		Gdiplus::REAL fTmpLeft= rtMe.GetLeft();
		Gdiplus::REAL fTmpRight = rtMe.GetRight();
		Gdiplus::REAL fTmpTop = rtMe.GetTop();
		Gdiplus::REAL fTmpBottom = rtMe.GetBottom();
		if (std::fabs(fMinX)<=0.0001f)
		{
			fMinX = fTmpLeft;
		}
		if (std::fabs(fMaxX)<=0.0001f)
		{
			fMaxX = fTmpRight;
		}
		if (std::fabs(fMinY)<=0.0001f)
		{
			fMinY = fTmpTop;
		}
		if (std::fabs(fMaxY)<=0.0001f)
		{
			fMaxY = fTmpBottom;
		}
		//С����С��
		if (fMinX >= fTmpLeft)
		{
			fMinX = fTmpLeft;
		}
		//��������
		if (fMaxX <= fTmpRight)
		{
			fMaxX = fTmpRight;
		}
		if (fMinY >= fTmpTop)
		{
			fMinY = fTmpTop;
		}
		if (fMaxY <= fTmpBottom)
		{
			fMaxY = fTmpBottom;
		}
		nCount++;
	}
	if ((std::fabs(fMaxX - fMinX)<=0.0001f) || (std::fabs(fMaxY - fMinY)<=0.0001f))
	{
		return false;
	}
	rtLimit.X = fMinX;
	rtLimit.Y = fMinY;
	rtLimit.Width = std::fabs(fMaxX - fMinX);
	rtLimit.Height = std::fabs(fMaxY - fMinY);
	return true;
}

void CBaseODL::RemoveSub( CBaseODLSPtr spSub )
{
	if (spSub)
	{
		if (spSub->IsCanDelete())
		{
			RemoveChild(spSub);
		}
	}
}

void CBaseODL::RemoveSelf()
{
	if (IsCanDelete())
	{
		RemoveFromParent();
	}
}

void CBaseODL::SetClearSideState( bool bAllSet )
{
	m_bIsSideFocused=false;
	m_bIsSideMoving=false;
	m_bIsSideResizing=false;
	m_bIsSideSelected=false;
	if (bAllSet)
	{
		for (auto&curItem:GetChildrenList())
		{
			curItem->SetClearSideState(bAllSet);
		}
	}
}
