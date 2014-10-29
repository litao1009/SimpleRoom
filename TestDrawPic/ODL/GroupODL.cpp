#include "stdafx.h"

#include "GroupODL.h"

#include "WallODL.h"
#include "FloorODL.h"

#include "BRepBndLib.hxx"
#include "TopExp_Explorer.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepOffsetAPI_MakePipeShell.hxx"

#include "StatusMgr.h"

CGroupODL::CGroupODL(void)
{
	
}


CGroupODL::~CGroupODL(void)
{
}

CBaseODLSPtr CGroupODL::CreateByWallPath( SRenderContextSPtr& rc, const TopoDS_Shape& bottomFace, const PointList& pntList, float wallHeight )
{
	Bnd_Box groupBox;
	gp_Pnt groupCenter;
	gp_Trsf transToGroupCenter;;
	{//求BBox和center
		BRepBndLib::Add(bottomFace, groupBox);
		const auto& samplePnt = pntList.back();
		groupBox.Add(gp_Pnt(samplePnt.X, StatusMgr::GetInstance().CreateWallHeight_, samplePnt.Y));

		Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
		groupBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
		groupCenter.SetX((xMin+xMax)/2);
		groupCenter.SetY((yMin+yMax)/2);
		groupCenter.SetZ((zMin+zMax)/2);
		
		transToGroupCenter.SetTranslationPart(groupCenter.XYZ().Reversed());
		groupBox = groupBox.Transformed(transToGroupCenter);
	}

	auto newGroup = Create<CGroupODL>(rc);
	{//创建Group
		newGroup->SetTranslation(groupCenter.XYZ());
		newGroup->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(groupCenter.X()), static_cast<float>(groupCenter.Y()), static_cast<float>(groupCenter.Z())));
		newGroup->SetBaseBndBox(groupBox);
	}

	auto transformedBottomFace = bottomFace.Moved(transToGroupCenter);

	{//创建墙
		static irr::core::vector3df s_YDir(0,1,0);

		int intIndex=0;
		for ( TopExp_Explorer exp(transformedBottomFace, TopAbs_FACE); exp.More(); exp.Next(),++intIndex )
		{
			auto& pnt0 = pntList[intIndex];
			auto& pnt1 = pntList[intIndex+1];

			auto drawDir = (pnt1-pnt0).normalize();
			auto wallDir = drawDir.crossProduct(s_YDir);

			auto newWall = WallODL::CreateByBottomFace(rc, exp.Current(), gp_Dir(wallDir.X,wallDir.Y,wallDir.Z), wallHeight);
			newGroup->AddChild(newWall);
		}
	}

	{//创建地板
		if ( pntList.size() > 2 )
		{
			BRepBuilderAPI_MakePolygon mp;
			for ( auto& curPnt : pntList )
			{
				mp.Add(gp_Pnt(curPnt.X, curPnt.Y, curPnt.Z));
			}
			mp.Close();
			assert(mp.IsDone());

			auto floorFace = BRepBuilderAPI_MakeFace(mp.Wire()).Shape();
			floorFace.Move(transToGroupCenter);
			auto newFloor = CFloorODL::CreateByFace(rc, transformedBottomFace, floorFace);

			newGroup->AddChild(newFloor);
		}
	}

	return newGroup;
}

CBaseODLSPtr CGroupODL::CreateByRect( SRenderContextSPtr& rc, const irr::core::vector3df& firstPnt, const irr::core::vector3df& lastPnt, float wallThick, float wallHeight )
{
	BRepBuilderAPI_MakePolygon mp;

	irr::core::vector3df v1,v2,v3,v4;
	v1 = firstPnt;
	v2.set(lastPnt.X, firstPnt.Y, firstPnt.Z);
	v3 = lastPnt;
	v4.set(firstPnt.X, firstPnt.Y, lastPnt.Z);

	gp_Pnt p1(v1.X, v1.Y, v1.Z);
	gp_Pnt p2(v2.X, v2.Y, v2.Z);
	gp_Pnt p3(v3.X, v3.Y, v3.Z);
	gp_Pnt p4(v4.X, v4.Y, v4.Z);
	mp.Add(p1);
	mp.Add(p2);
	mp.Add(p3);
	mp.Add(p4);
	mp.Close();
	assert(mp.IsDone());

	auto thickDir = gp_Dir(gp_Vec(p1, p2)).Crossed(gp::DY().Reversed());
	if ( lastPnt.X - firstPnt.X < 0 )
	{
		thickDir.Reverse();
	}

	gp_Pnt thickPnt(thickDir.XYZ()*wallThick + p1.XYZ());

	auto thickEdge = BRepBuilderAPI_MakeEdge(p1, thickPnt).Edge();

	TopoDS_Wire dirWire = BRepBuilderAPI_MakeWire(thickEdge).Wire();

	BRepOffsetAPI_MakePipeShell pipeMaker(mp.Wire());
	pipeMaker.SetTransitionMode(BRepBuilderAPI_RightCorner);//延切线方向缝合
	pipeMaker.Add(dirWire);
	pipeMaker.Build();

	PointList plist;
	plist.push_back(v1);
	plist.push_back(v2);
	plist.push_back(v3);
	plist.push_back(v4);
	plist.push_back(v1);

	return CreateByWallPath(rc, pipeMaker.Shape(), plist, wallHeight);
}
