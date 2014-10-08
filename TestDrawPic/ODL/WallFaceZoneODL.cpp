#include "stdafx.h"

#include "WallFaceZoneODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "TopExp_Explorer.hxx"

#include "ISceneManager.h"
#include "IVideoDriver.h"

#include <string>

CWallFaceZoneODL::CWallFaceZoneODL(void)
{
	m_ObjType = EOBT_Wall_Face_Zone;
	m_rtArea.X = 0.0f;
	m_rtArea.Y = 0.0f;
	m_rtArea.Width = 100.0f;
	m_rtArea.Height = 100.0f;
	
	m_rtLimitArea = m_rtArea;
	m_rtMovingArea = m_rtArea;
	m_pPrevPos.X = 0.0f;
	m_pPrevPos.Y = 0.0f;
	
}


CWallFaceZoneODL::~CWallFaceZoneODL(void)
{
}

void CWallFaceZoneODL::SetSideLimitArea(const Gdiplus::RectF& rtLimit)
{
	m_rtLimitArea = rtLimit;
	m_rtLimitArea.X = 0.0f;
	m_rtLimitArea.Y = 0.0f;
	
}

void CWallFaceZoneODL::SetZoneSize(Gdiplus::REAL fWidth, Gdiplus::REAL fHeight)
{
	m_rtArea.Width = fWidth;
	m_rtArea.Height = fHeight;
	m_rtMovingArea = m_rtArea;
	

	//���������¼���Limit
	for (auto& curItem: GetChildrenList())
	{
		curItem->SetSideLimitArea(m_rtArea);
	}
	
}

void CWallFaceZoneODL::CancelSideMoving()
{
	if (m_eSelectedPos!=ERSP_All)
	{
		return;
	}
	m_rtMovingArea = m_rtArea;
	CBaseODL::CancelTopMoving();
}

void CWallFaceZoneODL::OnSideMoveStart(const Gdiplus::PointF& ptStart)
{
	if (m_eSelectedPos!=ERSP_All)
	{
		return;
	}
	//�ƶ���ʱ��ֻ���ƶ��¼�����͵�ǰ����
	if (m_rtLimitArea.Equals(m_rtArea))
	{
		//��ǰ������ϼ�����һ����(���ƶ�)
		return;
	}
	m_ptSideStart = ptStart;
	CBaseODL::OnSideMoveStart(ptStart);
}

void CWallFaceZoneODL::OnSideMoving(const Gdiplus::PointF& ptOffset)
{
	if (m_eSelectedPos!=ERSP_All)
	{
		return;
	}
	//�ƶ���ʱ��ֻ���ƶ��¼�����͵�ǰ����
	if (m_rtLimitArea.Equals(m_rtArea))
	{
		//��ǰ������ϼ�����һ����(���ƶ�)
		return;
	}
	//�������
	Gdiplus::PointF ptCurrent = ptOffset;

	Gdiplus::REAL fLeft = m_rtLimitArea.X;	//��С����λ��X=0
	Gdiplus::REAL fTop = m_rtLimitArea.Y;	//��С����λ��Y=0
	Gdiplus::REAL fRight = m_rtLimitArea.X + m_rtLimitArea.Width - m_rtArea.Width;	//�������λ��X
	Gdiplus::REAL fBottom = m_rtLimitArea.Y + m_rtLimitArea.Height - m_rtArea.Height; //�������λ��Y
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
	CBaseODLSPtr spWallFace =  GetParent().lock();
	if (spWallFace->CheckSideIntersected(this->shared_from_this(), rtTarget))
	{
		//���ﻹ��Ҫ�ж�һ������������ ��ǰ������õĴ���
		Gdiplus::RectF rtSel=rtTarget;
		if (spWallFace->GetSideFitRect(this->shared_from_this(), false, false, rtSel))
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
			//����ܳ�ȥ�ˣ������ƶ���ȥ
			if (!rtSel.Contains(rtTemp))
			{
				return;
			}
			if (spWallFace->CheckSideIntersected(this->shared_from_this(), rtTemp))
			{
				return;
			}
			m_rtMovingArea = rtTemp;
		}
		else
		{
			//�Ҳ��������ϵ���Ŀ�ĵ�
			return;
		}
	}
	else
	{
		m_rtMovingArea = rtTarget;
	}
	//ͬʱ�ƶ�ƫ����
	SetIsSideMoving(true);
	CBaseODL::OnSideMoving(ptOffset);
}

void CWallFaceZoneODL::OnSideMovingFinish(const Gdiplus::PointF& ptStop, bool bCommitChange )
{
	if (bCommitChange)
	{
		Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
		Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
		//�ƶ��ľ���
		Gdiplus::PointF ptOffset(m_rtMovingArea.X - m_rtArea.X, m_rtMovingArea.Y - m_rtArea.Y);
		m_rtArea = m_rtMovingArea;

		m_ptSideGlobalOffset.X =m_rtArea.X + fX;
		m_ptSideGlobalOffset.Y =m_rtArea.Y + fY;
		//ԭʼ
		m_ptSideStop = ptStop;
		
		for ( auto& curChild : GetChildrenList() )
		{
			curChild->UpdateSideGlobalOffset(m_ptSideGlobalOffset);
		}
		UpdateMesh();
	}
	else
	{
		m_rtMovingArea = m_rtArea;
	}
}

void CWallFaceZoneODL::OnSideReSizeStart(const Gdiplus::PointF& ptStart)
{
	if ((ERSP_All==m_eSelectedPos) ||(ERSP_None==m_eSelectedPos))
	{
		return;
	}
	m_ptSideStart = ptStart;
	SetSideStartPos(ptStart);
	SetIsSideResizing(true);
}

void CWallFaceZoneODL::OnSideReSizing( const Gdiplus::PointF& ptCurrent)
{
	if ((ERSP_All==m_eSelectedPos) ||(ERSP_None==m_eSelectedPos))
	{
		return;
	}
	OnSideReSizing(m_eSelectedPos, ptCurrent);
}

void CWallFaceZoneODL::OnSideReSizing(ENUM_RECT_SELECTED_POS ePos, const Gdiplus::PointF& ptCurrent)
{
	//�ҵ�һ����С�ߴ�
	bool bIsMin = ((m_rtArea.Width<=100.0f) || (m_rtArea.Height<=100.0f));
	//current:��ǰȫ�����λ��
	//offsetcurrent:��ǰ����ڱ������ڵ�λ��
	Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
	Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
	Gdiplus::PointF ptOffsetCurrent = ptCurrent;
	//�ж�һ���ܲ���Resize��Ŀ�ģ�������ֱ�ӷ���
	//���ӵ�ƫ�Ƴߴ�
	Gdiplus::PointF ptInc(ptOffsetCurrent.X - m_ptSideStart.X, ptOffsetCurrent.Y - m_ptSideStart.Y);
	//�������ҵ�ԭʼλ��

	Gdiplus::PointF ptLeftTop(m_rtArea.X, m_rtArea.Y);
	Gdiplus::PointF ptRightTop(m_rtArea.X + m_rtArea.Width, m_rtArea.Y);
	Gdiplus::PointF ptLeftBottom(m_rtArea.X, m_rtArea.Y+ m_rtArea.Height);
	Gdiplus::PointF ptRightBottom(m_rtArea.X + m_rtArea.Width, m_rtArea.Y + m_rtArea.Height);

	int nLimitCt=0;
	//��ȡ��������Сֻ�ܵ���С
	Gdiplus::RectF rtLimited;
	GetSideMinRect(rtLimited, nLimitCt);
	
	//�������ڴ�������
	Gdiplus::REAL fX0 = rtLimited.GetLeft() + m_rtArea.X;
	Gdiplus::REAL fY0 = rtLimited.GetTop() + m_rtArea.Y;
	Gdiplus::REAL fX1 = rtLimited.GetRight() + m_rtArea.X;
	Gdiplus::REAL fY1 = rtLimited.GetBottom() + m_rtArea.Y;
	switch (ePos)
	{
	case ERSP_LeftTop:
		{
			//����
			ptLeftTop.X +=ptInc.X;
			ptLeftTop.Y +=ptInc.Y;
			//����
			ptRightTop.Y +=ptInc.Y;
			//����
			ptLeftBottom.X +=ptInc.X;
		}
		break;
	case ERSP_RightTop:
		{
			//����
			ptRightTop.X +=ptInc.X;
			ptRightTop.Y +=ptInc.Y;
			//����
			ptLeftTop.Y +=ptInc.Y;
			//����
			ptRightBottom.X +=ptInc.X;
		}
		break;
	case ERSP_RightBottom:
		{
			//����
			ptRightBottom.X +=ptInc.X;
			ptRightBottom.Y +=ptInc.Y;
			//����
			ptRightTop.X +=ptInc.X;
			//����
			ptLeftBottom.Y +=ptInc.Y;
		}
		break;
	case ERSP_LeftBottom:
		{
			//����
			ptLeftBottom.X +=ptInc.X;
			ptLeftBottom.Y +=ptInc.Y;
			//����
			ptLeftTop.X +=ptInc.X;
			//����
			ptRightBottom.Y +=ptInc.Y;
		}
		break;
	case ERSP_Top:
		{
			//��
			ptLeftTop.Y +=ptInc.Y;
			ptRightTop.Y +=ptInc.Y;
		}
		break;
	case ERSP_Bottom:
		{
			//6���£�
			ptLeftBottom.Y +=ptInc.Y;
			ptRightBottom.Y +=ptInc.Y;
		}
		break;
	case ERSP_Right:
		{
			//4��
			ptRightTop.X +=ptInc.X;
			ptRightBottom.X +=ptInc.X;
		}
		break;
	case ERSP_Left:
		{
			//8����
			ptLeftTop.X +=ptInc.X;
			ptLeftBottom.X +=ptInc.X;
		}
		break;
	}
	//��֤��С�Ǹ����
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
	Gdiplus::REAL fLeft = m_rtLimitArea.X;	//��С����λ��X
	Gdiplus::REAL fTop = m_rtLimitArea.Y;	//��С����λ��Y
	Gdiplus::REAL fRight = m_rtLimitArea.X + m_rtLimitArea.Width;
	Gdiplus::REAL fBottom = m_rtLimitArea.Y + m_rtLimitArea.Height;
	//С�������
	if (ptLeftTop.X<fLeft)
	{
		ptLeftTop.X = fLeft;
		ptLeftBottom.X = fLeft;
	}
	//�������ұ�
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
				//����
				if (ptInc.X>0)
				{
					//��С
					if (fX1>=ptRightTop.X)
					{
						//�ұ߲���
						ptRightBottom.X = ptRightTop.X;
						//Ȼ������еĵ㿿�����
						ptLeftTop.X = ptRightTop.X - rtLimited.Width - rtLimited.X;
						ptLeftBottom.X = ptLeftTop.X;
					}
				}
				if (ptInc.Y>0)
				{
					//��С
					if (fY1>=ptRightBottom.Y)
					{
						//�±߲���
						ptLeftBottom.Y = ptRightBottom.Y;
						//Ȼ������еĵ㿿�����
						ptRightTop.Y = ptRightBottom.Y - rtLimited.Height - rtLimited.Y;
						ptLeftTop.Y = ptRightTop.Y;
					}
				}
			}
			break;
		case ERSP_RightTop:
			{
				//����
				if (ptInc.X<0)
				{
					//��С(�ƶ�����)
					if (fX1>=ptRightTop.X)
					{
						//��߲���
						ptLeftBottom.X = ptLeftTop.X;
						//Ȼ������еĵ㿿�����
						ptRightTop.X = ptLeftTop.X + rtLimited.Width + rtLimited.X;
						ptRightBottom.X = ptRightTop.X;
					}
				}
				if (ptInc.Y>0)
				{
					//��С
					if (fY1>=ptRightBottom.Y)
					{
						//�±߲���
						ptLeftBottom.Y = ptRightBottom.Y;
						//Ȼ������еĵ㿿�����
						ptRightTop.Y = ptRightBottom.Y - rtLimited.Height - rtLimited.Y;
						ptLeftTop.Y = ptRightTop.Y;
					}
				}
			}
			break;
		case ERSP_RightBottom:
			{
				//����
				if (ptInc.X<0)
				{
					//��С(�ƶ�����)
					if (fX1>=ptRightTop.X)
					{
						//��߲���
						ptLeftBottom.X = ptLeftTop.X;
						//Ȼ������еĵ㿿�����
						ptRightTop.X = ptLeftTop.X + rtLimited.Width + rtLimited.X;
						ptRightBottom.X = ptRightTop.X;
					}
				}
				if (ptInc.Y<0)
				{
					//��С(�ƶ�����)
					if (fY1>=ptRightBottom.Y)
					{
						//�ϱ߲���
						ptLeftTop.Y = ptRightTop.Y;
						//Ȼ������еĵ㿿�����
						ptRightBottom.Y = ptRightTop.Y + rtLimited.Height + rtLimited.Y;
						ptLeftBottom.Y = ptRightBottom.Y;
					}
				}
			}
			break;
		case ERSP_LeftBottom:
			{
				//����
				if (ptInc.X>0)
				{
					//��С
					if (fX1>=ptRightTop.X)
					{
						//�ұ߲���
						ptRightBottom.X = ptRightTop.X;
						//Ȼ������еĵ㿿�����
						ptLeftTop.X = ptRightTop.X - rtLimited.Width - rtLimited.X;
						ptLeftBottom.X = ptLeftTop.X;
					}
				}
				if (ptInc.Y<0)
				{
					//��С(�ƶ�����)
					if (fY1>=ptRightBottom.Y)
					{
						//�ϱ߲���
						ptLeftTop.Y = ptRightTop.Y;
						//Ȼ������еĵ㿿�����
						ptRightBottom.Y = ptRightTop.Y + rtLimited.Height + rtLimited.Y;
						ptLeftBottom.Y = ptRightBottom.Y;
					}
				}
			}
			break;
		case ERSP_Top:
			{
				//��
				if (ptInc.Y>0)
				{
					//��С
					if (fY1>=ptRightBottom.Y)
					{
						//�±߲���
						ptLeftBottom.Y = ptRightBottom.Y;
						//Ȼ������еĵ㿿�����
						ptRightTop.Y = ptRightBottom.Y - rtLimited.Height - rtLimited.Y;
						ptLeftTop.Y = ptRightTop.Y;
					}
				}
			}
			break;
		case ERSP_Right:
			{
				//4��
				if (ptInc.X<0)
				{
					//��С(�ƶ�����)
					if (fX1>=ptRightTop.X)
					{
						//��߲���
						ptLeftBottom.X = ptLeftTop.X;
						//Ȼ������еĵ㿿�����
						ptRightTop.X = ptLeftTop.X + rtLimited.Width + rtLimited.X;
						ptRightBottom.X = ptRightTop.X;
					}
				}
			}
			break;
		case ERSP_Bottom:
			{
				//6���£�
				if (ptInc.Y<0)
				{
					//��С(�ƶ�����)
					if (fY1>=ptRightBottom.Y)
					{
						//�ϱ߲���
						ptLeftTop.Y = ptRightTop.Y;
						//Ȼ������еĵ㿿�����
						ptRightBottom.Y = ptRightTop.Y + rtLimited.Height + rtLimited.Y;
						ptLeftBottom.Y = ptRightBottom.Y;
					}
				}
			}
			break;
		case ERSP_Left:
			{
				//8����
				if (ptInc.X>0)
				{
					//��С
					if (fX1>=ptRightTop.X)
					{
						//�ұ߲���
						ptRightBottom.X = ptRightTop.X;
						//Ȼ������еĵ㿿�����
						ptLeftTop.X = ptRightTop.X - rtLimited.Width - rtLimited.X;
						ptLeftBottom.X = ptLeftTop.X;
					}
				}
			}
			break;
		}
	}
	
	Gdiplus::RectF rtDest(ptLeftTop.X, ptLeftTop.Y, ptRightTop.X - ptLeftTop.X, ptRightBottom.Y - ptRightTop.Y);
	//���Ŀ��λ�ò�������Χ�ڣ������޸�
	CBaseODLSPtr spWallFace =  GetParent().lock();
	Gdiplus::RectF rtTmp(rtDest);
	if (spWallFace->CheckSideIntersected(this->shared_from_this(), rtTmp))
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

void CWallFaceZoneODL::OnSideReSizeEnd(const Gdiplus::PointF& ptStop)
{
	SetIsSideResizing(false);
	
	m_pPrevPos.X = 0.0f;
	m_pPrevPos.Y = 0.0f;
	m_rtMovingArea = m_rtArea;
	SetSideStopPos(ptStop);
	//�ϼ��Ĵ�С��λ�ö�����
	//ԭʼ
	m_ptSideStop = ptStop;

	UpdateMesh();
}

void CWallFaceZoneODL::SetIsTopSelected(bool isSelected)
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


void CWallFaceZoneODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale )
{
	//��ǽ�����
	CBaseODL::InitSideColor();
	//������Զ��4����
	
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
		//�Ȱ�ÿ��ͼƬ�����걣������
	}
	//gcDrawer.DrawRectangle(&pen, rtDrawArea);
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
		//ÿ��ת�ǻ�һ����
		for (const auto& curPnt : arrSidePoint )
		{
			gcDrawer.DrawRectangle(&Dot, (curPnt.X) - 3.0f/fScale, (curPnt.Y ) - 3.0f/fScale, 6.0f/fScale, 6.0f/fScale);
		}
	}
	
	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //�ڶ����������С
	
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	CString strTxt;
	strTxt.Format(_T("%.2f,%.2f; %.2f, %.2f"), m_rtArea.X, m_rtArea.Y, m_rtArea.Width, m_rtArea.Height );
	gcDrawer.DrawString(strTxt, strTxt.GetLength(), &myFont, arrSidePoint[0], &m_FontFormat, m_pFontBrush );//��string���Ƶ�ͼ
	//��������ڸ���ͼ�¼����߶�
	CBaseODL::OnDrawFace(gcDrawer, fScale);
}

void CWallFaceZoneODL::SetSideSize(const Gdiplus::RectF& rtTarget )
{
	Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
	Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
	m_rtArea = rtTarget;
	m_ptSideGlobalOffset.X =fX + m_rtArea.X;
	m_ptSideGlobalOffset.Y =fY + m_rtArea.Y;
	m_rtMovingArea = m_rtArea;
	//���������¼���Limit
	for (auto& curItem: GetChildrenList())
	{
		curItem->SetSideLimitArea(m_rtArea);
	}
}

bool CWallFaceZoneODL::UpdateMesh()
{
	auto pnt = GetParent().lock();
	while ( pnt )
	{
		if ( EOBT_Wall_Face == pnt->GetType() )
		{
			auto holes = std::static_pointer_cast<IWithHoleODL>(pnt);
			SetHoleList(holes->GetHoleList());
			GetRectList() = holes->GetRectList();
			break;
		}

		pnt = pnt->GetParent().lock();
	}

	return UpdateHoleShape();
}

bool CWallFaceZoneODL::UpdateHoleShape()
{
	{//dispatch
		for ( auto& curChild : GetChildrenList() )
		{
			if ( EOBT_Wall_Face_Zone == curChild->GetType() )
			{
				assert(std::dynamic_pointer_cast<IWithHoleODL>(curChild));

				auto withHole = std::static_pointer_cast<IWithHoleODL>(curChild);
				withHole->SetHoleList(GetHoleList());
				withHole->GetRectList() = GetRectList();
				withHole->UpdateHoleShape();
			}
		}
	}

	auto sn = GetSceneNode();	

	if ( !sn )
	{
		return false;
	}

	BRepBuilderAPI_MakePolygon mp;
	mp.Add(gp_Pnt(m_rtArea.X, -m_rtArea.Y, 0));
	mp.Add(gp_Pnt(m_rtArea.X+m_rtArea.Width, -m_rtArea.Y, 0));
	mp.Add(gp_Pnt(m_rtArea.X+m_rtArea.Width, -m_rtArea.Y-m_rtArea.Height, 0));
	mp.Add(gp_Pnt(m_rtArea.X, -m_rtArea.Y-m_rtArea.Height, 0));
	mp.Close();	

	auto face = BRepBuilderAPI_MakeFace(mp.Wire()).Shape();

	for ( auto& curRect : GetRectList() )
	{
		auto& pos = std::get<1>(curRect);
		auto& size = std::get<2>(curRect);

		BRepBuilderAPI_MakePolygon s;
		s.Add(gp_Pnt(pos.X(), -pos.Y(), 0));
		s.Add(gp_Pnt(pos.X()+size.X(), -pos.Y(), 0));
		s.Add(gp_Pnt(pos.X()+size.X(), -pos.Y()-size.Y(), 0));
		s.Add(gp_Pnt(pos.X(),-pos.Y()-size.Y(), 0));
		s.Close();

		auto subFace = BRepBuilderAPI_MakeFace(s.Wire()).Face();

		BRepAlgoAPI_Cut bc(face, subFace);
		face = bc.Shape();
	}

	TopExp_Explorer exp(face, TopAbs_FACE);
	auto shape = exp.Current();
	
	auto mesh = ODLTools::CreateMesh(shape);

	auto pntSN = GetParent().lock()->GetSceneNode();
	if ( pntSN )
	{
		auto meshBufferPtr = mesh->getMeshBuffer(0); 
		meshBufferPtr->getMaterial().PolygonOffsetDirection = irr::video::EPO_FRONT;
		meshBufferPtr->getMaterial().PolygonOffsetFactor = 1;

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

	sn->CreateTriSelector();

	//��������
	if ( m_spSkin && m_spSkin->HasPicData() )
	{
		auto tex = m_spSkin->GetTextureData();
		sn->setMaterialTexture(0, tex);

		auto uLen = m_spSkin->GetSkinWidth()+m_spSkin->GetGroutX();
		auto vLen = m_spSkin->GetSkinHeight()+m_spSkin->GetGroutY();
		auto uFactor = m_rtArea.Width / uLen;
		auto vFactor = m_rtArea.Height / vLen;

		irr::core::matrix4 texMat;
		texMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
		auto meshBufferPtr = sn->getMesh()->getMeshBuffer(0);
		meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
	}

	return true;
}