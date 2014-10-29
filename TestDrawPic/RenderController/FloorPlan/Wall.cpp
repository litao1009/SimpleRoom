#include "stdafx.h"

#include "Wall.h"
#include "Corner.h"
#include "GraphController.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"
#include "gp_Pln.hxx"
#include "gp_Ax3.hxx"
#include "GeomAPI.hxx"

#include <algorithm>

class	WallSorter
{
public:

	bool	operator() (const WallSPtr& wall1, const WallSPtr& wall2)
	{
		auto wall1FirstCorner = wall1->GetFirstCorner().lock();
		auto wall1SecondCorner = wall1->GetSecondCorner().lock();
		auto wall2FirstCorner = wall2->GetFirstCorner().lock();
		auto wall2SecondCorner = wall2->GetSecondCorner().lock();

		if ( wall1FirstCorner != RefCorner_ )
		{
			wall1FirstCorner.swap(wall1SecondCorner);
			assert(wall1FirstCorner == RefCorner_);
		}

		if ( wall2FirstCorner != RefCorner_ )
		{
			wall2FirstCorner.swap(wall2SecondCorner);
			assert(wall2FirstCorner == RefCorner_);
		}

		gp_Dir d1 = gp_Vec(wall1FirstCorner->GetPosition(), wall1SecondCorner->GetPosition());
		gp_Dir d2 = gp_Vec(wall2FirstCorner->GetPosition(), wall2SecondCorner->GetPosition());

		auto angle1 = d1.AngleWithRef(gp::DX(), gp::DY().Reversed());
		auto angle2 = d2.AngleWithRef(gp::DX(), gp::DY().Reversed());

		angle1 = angle1 < 0 ? M_PI * 2 + angle1 : angle1;
		angle2 = angle2 < 0 ? M_PI * 2 + angle2 : angle2;
		
		return angle1 < angle2;
	}

	CornerSPtr		RefCorner_;
};

class	Wall::Imp
{
public:
	static	void	CalculateSideMesh(const WallSPtr& wall, const CornerSPtr& corner)
	{
		auto& meshPoints_ = wall->MeshPoints_;

		auto beginCorner = wall->GetFirstCorner().lock();
		auto endCorner = wall->GetSecondCorner().lock();

		if ( beginCorner != corner )
		{
			beginCorner.swap(endCorner);
		}

		assert(beginCorner == corner);

		auto curBeginPos = beginCorner->GetPosition();
		auto curEndPos = endCorner->GetPosition();

		gp_Dir curDir = gp_Vec(curBeginPos, curEndPos);
		auto curThickDir = curDir.Crossed(gp::DY().Reversed());
		gp_Vec curThickVec = gp_Vec(curThickDir) * wall->GetThickness()/2;

		auto wallsAtCorner = GraphController::GetInstance().GetWallsOnCorner(corner);
		assert(!wallsAtCorner.empty());

		{
			auto p1 = curThickVec.XYZ() + curBeginPos.XYZ();
			auto p2 = curBeginPos.XYZ();
			auto p3 = curThickVec.Reversed().XYZ() + curBeginPos.XYZ();

			if ( corner == wall->GetFirstCorner().lock() )
			{
				meshPoints_[0] = p1;
				meshPoints_[5] = p2;
				meshPoints_[4] = p3;
			}
			else
			{
				meshPoints_[3] = p1;
				meshPoints_[2] = p2;
				meshPoints_[1] = p3;
			}
		}

		if ( wallsAtCorner.size() == 1 )
		{
			return;
		}

		WallSPtr otherTopWall,otherBottomWall;

		if ( wallsAtCorner.size() == 2 )
		{
			auto otherWall = wallsAtCorner[0] == wall ? wallsAtCorner[1] : wallsAtCorner[0];
			otherTopWall = otherWall;
			otherBottomWall = otherWall;
		}
		else
		{
			WallSorter ws;
			ws.RefCorner_ = beginCorner;
			std::sort(wallsAtCorner.begin(), wallsAtCorner.end(), ws);

			auto curItor = std::find(wallsAtCorner.begin(), wallsAtCorner.end(), wall);
			if ( curItor == wallsAtCorner.begin() )
			{
				otherTopWall = *(curItor + 1);
				otherBottomWall = wallsAtCorner.back();
			}
			else if ( curItor + 1 == wallsAtCorner.end() )
			{
				otherTopWall = wallsAtCorner.front();
				otherBottomWall = *(curItor - 1);
			}
			else
			{
				otherTopWall = *(curItor + 1);
				otherBottomWall = *(curItor - 1);
			}
		}

		gp_Pnt p1,p3,p0,p4;
		p1 = meshPoints_[1];
		p3 = meshPoints_[3];
		p0 = meshPoints_[0];
		p4 = meshPoints_[4];

		{//Top
			auto otherBeginCorner = otherTopWall->GetFirstCorner().lock();
			auto otherEndCorner = otherTopWall->GetSecondCorner().lock();

			if ( otherBeginCorner != corner )
			{
				otherBeginCorner.swap(otherEndCorner);
			}

			auto otherBeginPos = otherBeginCorner->GetPosition();
			auto otherEndPos = otherEndCorner->GetPosition();
			gp_Dir otherDir = gp_Vec(otherBeginPos, otherEndPos);
			auto otherThickDir = otherDir.Crossed(gp::DY().Reversed());
			gp_Vec otherThickVec = gp_Vec(otherThickDir) * otherTopWall->GetThickness()/2;

			auto curEdge = BRepBuilderAPI_MakeEdge(curBeginPos.XYZ()+curThickVec.XYZ(), curEndPos.XYZ()+curThickVec.XYZ()).Edge();
			auto otherEdge = BRepBuilderAPI_MakeEdge(otherBeginPos.XYZ()+otherThickVec.Reversed().XYZ(), otherEndPos.XYZ()+otherThickVec.Reversed().XYZ()).Edge();

			BRepAdaptor_Curve curBC(curEdge);
			BRepAdaptor_Curve otherBC(otherEdge);

			if ( Standard_False == curBC.Line().Direction().IsParallel(otherBC.Line().Direction(), Precision::Confusion()) )
			{
				gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

				auto firstline2D = GeomAPI::To2d(curBC.Curve().Curve(), pln);
				auto secondline2D = GeomAPI::To2d(otherBC.Curve().Curve(), pln);

				Geom2dAPI_InterCurveCurve icc(firstline2D, secondline2D);
				assert(icc.NbPoints() == 1);

				auto pnt2D = icc.Point(1);
				gp_Pnt pnt(pnt2D.X(), 0, pnt2D.Y());

				if ( corner == wall->GetFirstCorner().lock() )
				{
					p0 = pnt;
				}
				else
				{
					p3 = pnt;
				}
			}
		}

		{//Bottom
			auto otherBeginCorner = otherBottomWall->GetFirstCorner().lock();
			auto otherEndCorner = otherBottomWall->GetSecondCorner().lock();

			if ( otherBeginCorner != corner )
			{
				otherBeginCorner.swap(otherEndCorner);
			}

			auto otherBeginPos = otherBeginCorner->GetPosition();
			auto otherEndPos = otherEndCorner->GetPosition();
			gp_Dir otherDir = gp_Vec(otherBeginPos, otherEndPos);
			auto otherThickDir = otherDir.Crossed(gp::DY().Reversed());
			gp_Vec otherThickVec = gp_Vec(otherThickDir) * otherTopWall->GetThickness()/2;

			auto curEdge = BRepBuilderAPI_MakeEdge(curBeginPos.XYZ()+curThickVec.Reversed().XYZ(), curEndPos.XYZ()+curThickVec.Reversed().XYZ()).Edge();
			auto otherEdge = BRepBuilderAPI_MakeEdge(otherBeginPos.XYZ()+otherThickVec.XYZ(), otherEndPos.XYZ()+otherThickVec.XYZ()).Edge();

			BRepAdaptor_Curve curBC(curEdge);
			BRepAdaptor_Curve otherBC(otherEdge);

			if ( Standard_False == curBC.Line().Direction().IsParallel(otherBC.Line().Direction(), Precision::Confusion()) )
			{
				gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

				auto firstline2D = GeomAPI::To2d(curBC.Curve().Curve(), pln);
				auto secondline2D = GeomAPI::To2d(otherBC.Curve().Curve(), pln);

				Geom2dAPI_InterCurveCurve icc(firstline2D, secondline2D);
				assert(icc.NbPoints() == 1);

				auto pnt2D = icc.Point(1);
				gp_Pnt pnt(pnt2D.X(), 0, pnt2D.Y());

				if ( corner == wall->GetFirstCorner().lock() )
				{
					p4 = pnt;
				}
				else
				{
					p1 = pnt;
				}
			}
		}

		meshPoints_[0] = p0;
		meshPoints_[1] = p1;
		meshPoints_[3] = p3;
		meshPoints_[4] = p4;
	}
};

Wall::Wall( const CornerSPtr& firstCorner, const CornerSPtr& secondCorner, float wallThickness )
{
	FirstCorner_ = firstCorner;
	SecondCorner_ = secondCorner;
	Thickness_ = wallThickness;
}


bool Wall::IsBezierCurve() const
{
	return false;
}

TopoDS_Edge Wall::GetEdge() const
{
	return BRepBuilderAPI_MakeEdge(FirstCorner_.lock()->GetPosition(), SecondCorner_.lock()->GetPosition()).Edge();
}

void Wall::UpdateMesh()
{
	if ( IsBezierCurve() )
	{
		//TODO
	}
	else
	{
		Imp::CalculateSideMesh(shared_from_this(), FirstCorner_.lock());
		Imp::CalculateSideMesh(shared_from_this(), SecondCorner_.lock());
	}
}
