#include "stdafx.h"

#include "DecorODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepPrimAPI_MakeBox.hxx"

#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"

CDecorODL::CDecorODL(void)
{
	m_ObjType = EOBT_Decor;
}


CDecorODL::~CDecorODL(void)
{
}

void CDecorODL::Create( float fXLen, float fYLen, float fZLen, gp_Pnt& ptPos, CString strModelPath )
{
	SetSize(fXLen, fYLen, fZLen);
	SetPos(ptPos);
	UpdateModelPath(strModelPath);
}

void CDecorODL::SetSize(float fXLen, float fYLen, float fZLen)
{
	SetHoleSize(fXLen,fYLen,fZLen);
	m_rtTopArea.Width = fXLen;
	m_rtTopArea.Height = fZLen;
	m_rtMovingTopArea.Width = m_rtTopArea.Width;
	m_rtMovingTopArea.Height = m_rtTopArea.Height;
}

void CDecorODL::SetPos( const gp_Pnt& ptPos )
{
	SetHolePosition(ptPos);

	m_ptTopStart = ptPos;
	m_ptTopStop = ptPos;
	m_rtTopArea.X = static_cast<Gdiplus::REAL>(ptPos.X());
	m_rtTopArea.Y = static_cast<Gdiplus::REAL>(ptPos.Z());
	m_rtMovingTopArea.X = m_rtTopArea.X;
	m_rtMovingTopArea.Y = m_rtTopArea.Y;
	m_fYPos = static_cast<float>(ptPos.Y());
}

void CDecorODL::UpdateModelPath( CString strModelPath )
{
	m_strModelPath = strModelPath;
}

void CDecorODL::CancelTopMoving()
{
	m_rtMovingTopArea = m_rtTopArea;
	CBaseODL::CancelTopMoving();
}

void CDecorODL::OnTopMoveStart(const gp_Pnt& stInfo)
{
	SetIsTopMoving(true);
}

void CDecorODL::OnTopMoving(const gp_Pnt& stInfo)
{
	CBaseODL::OnTopMoving(stInfo);
	m_rtMovingTopArea = m_rtTopArea;
	m_rtMovingTopArea.X += static_cast<Gdiplus::REAL>(stInfo.X());
	m_rtMovingTopArea.Y += static_cast<Gdiplus::REAL>(stInfo.Z());
}

void CDecorODL::OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange )
{
	if (bCommitChange)
	{
		OnTopMoving(stInfo);
		SetIsTopMoving(false);
		m_rtTopArea = m_rtMovingTopArea;
		CBaseODL::OnTopMovingFinish(stInfo, bCommitChange);
	}
	else
	{
		m_rtMovingTopArea = m_rtTopArea;
	}
}

bool CDecorODL::HasTopCollisionWith( const TopoDS_Shape& theShape )
{
	TopoDS_Wire wallPathWire;	//路径线
	{
		BRepBuilderAPI_MakePolygon wallPath;
		wallPath.Add(gp_Pnt(m_rtTopArea.X, m_fYPos, m_rtTopArea.Y));
		wallPath.Add(gp_Pnt(m_rtTopArea.GetRight(), m_fYPos, m_rtTopArea.Y));
		wallPath.Add(gp_Pnt(m_rtTopArea.GetRight(), m_fYPos, m_rtTopArea.GetBottom()));
		wallPath.Add(gp_Pnt(m_rtTopArea.X, m_fYPos, m_rtTopArea.GetBottom()));
		wallPathWire = wallPath.Wire();
	}
	TopoDS_Face fs = (BRepBuilderAPI_MakeFace(wallPathWire));
	SetFaceShape(fs);

	return CBaseODL::HasTopCollisionWith(theShape);
}

void CDecorODL::OnDrawTop( Gdiplus::Graphics& gcDrawer, float fScale )
{
	//画这个面在俯视图下级的线段
	InitTopColor();
	CBaseODL::OnDrawTop(gcDrawer, fScale);
	//画本身的内容
	
	Gdiplus::Pen Dot( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	Gdiplus::Matrix curMat;
	gcDrawer.GetTransform(&curMat);
	auto mCopy = curMat.Clone();
	Gdiplus::Matrix mr;
	mr.RotateAt(static_cast<Gdiplus::REAL>(GetHoleRotation().Y() * M_1_PI * 180), Gdiplus::PointF(m_rtTopArea.X, m_rtTopArea.Y));
	mCopy->Multiply(&mr);

	gcDrawer.ResetTransform();
	gcDrawer.SetTransform(mCopy);

	gcDrawer.DrawRectangle(&pen, m_rtTopArea);

	if (IsTopMoving())
	{
		Gdiplus::Pen penMoving( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
		penMoving.SetDashStyle( Gdiplus::DashStyleSolid);
		Gdiplus::Matrix mMove;
		mMove.RotateAt(static_cast<Gdiplus::REAL>(GetHoleRotation().Y() * M_1_PI * 180), Gdiplus::PointF(m_rtMovingTopArea.X, m_rtMovingTopArea.Y));
		auto mMoveCopy = curMat.Clone();
		mMoveCopy->Multiply(&mMove);
		gcDrawer.SetTransform(mMoveCopy);
		gcDrawer.DrawRectangle(&penMoving, m_rtMovingTopArea);
		delete mMoveCopy;
	}

	gcDrawer.SetTransform(&curMat);
	delete mCopy;

	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
	if (GetHasLength())
	{
		if (m_ptTopStart.IsEqual(m_ptTopStop, Precision::Confusion()))
		{
			return;
		}
		CString strText;

		gp_Dir curDir(gp_Vec(m_ptTopStart, m_ptTopStop));
		auto curAngle = curDir.AngleWithRef(gp::DX(), gp::DY());
		if ( std::abs(curAngle) > M_PI_2 )
		{
			curAngle += M_PI;
		}

		gp_Pnt midPnt((m_ptTopStop.X()+m_ptTopStart.X())/2, (m_ptTopStop.Y()+m_ptTopStart.Y())/2, (m_ptTopStop.Z()+m_ptTopStart.Z())/2);

		auto tmpPnt = midPnt.Rotated(gp::OY(), curAngle);

		Standard_Real fLength= m_ptTopStart.Distance(m_ptTopStop);
		strText.Format(_T("%.1f"), fLength);

		//需要长度显示

		Gdiplus::PointF ptCur(static_cast<Gdiplus::REAL>(tmpPnt.X()), static_cast<Gdiplus::REAL>(tmpPnt.Z()));

		gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(curAngle * M_1_PI * 180));

		gcDrawer.DrawString(strText, strText.GetLength(), &myFont, ptCur, &format, m_pFontBrush );//把string绘制到图上
		gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(-curAngle * M_1_PI * 180));
	}
	
}

//////////////////////////////////////////////////////////////////////////
//在面上移动 

void CDecorODL::CancelSideMoving()
{
	m_rtMovingArea = m_rtArea;
	CBaseODL::CancelTopMoving();
}

void CDecorODL::OnSideMoveStart(const Gdiplus::PointF& ptStart)
{
	//获取当前对象在当前面的位置和大小，设置到m_rtArea中
	m_rtMovingArea = m_rtArea;

	m_ptSideStart = ptStart;
	
	CBaseODL::OnSideMoveStart(ptStart);
}

void CDecorODL::OnSideMoving(const Gdiplus::PointF& ptOffset)
{
	m_rtMovingArea = m_rtArea;
	m_rtMovingArea.X+=ptOffset.X;
	m_rtMovingArea.Y += ptOffset.Y;
	
	//同时移动偏移量
	SetIsSideMoving(true);
	CBaseODL::OnSideMoving(ptOffset);
}

void CDecorODL::OnSideMovingFinish(const Gdiplus::PointF& ptStop, bool bCommitChange )
{
	if (bCommitChange)
	{
		//设置洞的位置到m_rtArea;
		SetIsSideMoving(false);
		Gdiplus::REAL fX = m_ptSideGlobalOffset.X - m_rtArea.X;
		Gdiplus::REAL fY = m_ptSideGlobalOffset.Y - m_rtArea.Y;
		//移动的距离
		Gdiplus::PointF ptOffset(m_rtMovingArea.X - m_rtArea.X, m_rtMovingArea.Y - m_rtArea.Y);
		m_rtArea = m_rtMovingArea;

		m_ptSideGlobalOffset.X =m_rtArea.X + fX;
		m_ptSideGlobalOffset.Y =m_rtArea.Y + fY;
		UpdateMesh();
	}
	else
	{
		m_rtMovingArea = m_rtArea;
	}
}

void CDecorODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale )
{
	if (m_rtArea.Width<=0.0001f)
	{
		m_rtArea.X = static_cast<Gdiplus::REAL>(m_ptSideGlobalOffset.X);
		m_rtArea.Y = static_cast<Gdiplus::REAL>(m_ptSideGlobalOffset.Y);
		m_rtArea.Width = static_cast<Gdiplus::REAL>(GetHoleSize().X());
		m_rtArea.Height = static_cast<Gdiplus::REAL>(GetHoleSize().Y());
		m_rtMovingArea = m_rtArea;
	}

	//画墙面矩形
	InitSideColor();
	//画本身的内容

	Gdiplus::PointF ptCenter(m_rtArea.Width/2.0f, m_rtArea.Height/2.0f);

	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	gcDrawer.DrawRectangle(&pen, m_rtArea);

	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
	if (IsSideMoving())
	{
		gcDrawer.DrawRectangle(&pen, m_rtMovingArea);
	}
	if (GetHasLength())
	{
		//画宽
		//画高
		CString strText;

		std::vector<gp_Pnt> arrPt;
		arrPt.emplace_back(gp_Pnt(0.0f, 0.0f, 0.0f));
		arrPt.emplace_back(gp_Pnt(m_rtArea.Width, 0.0f, 0.0f));
		arrPt.emplace_back(gp_Pnt(m_rtArea.Width, m_rtArea.Height, 0.0f));

		std::adjacent_find(arrPt.begin(), arrPt.end(), [&strText,&gcDrawer,&myFont,&format,this](gp_Pnt& pt0, gp_Pnt& pt1)
		{
			gp_Dir curDir(gp_Vec(pt0, pt1));
			auto curAngle = curDir.AngleWithRef(gp::DX(), gp::DY());
			if ( std::abs(curAngle) > M_PI_2 )
			{
				curAngle += M_PI;
			}

			gp_Pnt midPnt((pt1.X()+pt0.X())/2, (pt1.Y()+pt0.Y())/2, (pt1.Z()+pt0.Z())/2);

			auto tmpPnt = midPnt.Rotated(gp::OY(), curAngle);

			Standard_Real fLength= pt0.Distance(pt1);
			strText.Format(_T("%.1f"), fLength);

			//需要长度显示

			Gdiplus::PointF ptCur(static_cast<Gdiplus::REAL>(tmpPnt.X()), static_cast<Gdiplus::REAL>(tmpPnt.Z()));

			gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(curAngle * M_1_PI * 180));

			gcDrawer.DrawString(strText, strText.GetLength(), &myFont, ptCur, &format, m_pFontBrush );//把string绘制到图上
			gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(-curAngle * M_1_PI * 180));

			return false;
		});
	}
	CString strIndex = _T("");
	strIndex.Format(_T("%d"), GetIndex());
	format.SetAlignment(Gdiplus::StringAlignmentCenter);    //文本排列方式，即在对应位置居中、靠左、靠右
	gcDrawer.DrawString(strIndex, strIndex.GetLength(), &myFont, ptCenter, &format, m_pFontBrush );//把string绘制到图

	//画这个面在俯视图下级的线段
	CBaseODL::OnDrawFace(gcDrawer, fScale );
}

bool CDecorODL::UpdateMesh()
{
	return false;
	auto rc = GetSceneNode()->GetRenderContext().lock();
	auto mesh = rc->Smgr_->getMesh(m_strModelPath.GetBuffer());
	m_strModelPath.ReleaseBuffer();

	assert(mesh);

	if ( mesh )
	{
		GetSceneNode()->setMesh(mesh);
		return true;
	}

	return false;
}
