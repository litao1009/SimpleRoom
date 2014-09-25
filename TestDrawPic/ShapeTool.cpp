#include "stdafx.h"
#include "ShapeTool.h"

#include "TopoDS_Wire.hxx"
#include "TopoDS_Face.hxx"
#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepBuilderAPI_Copy.hxx"
#include "BRepPrimAPI_MakePrism.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "gp_Dir.hxx"
#include "gp.hxx"
#include "gp_Lin.hxx"
#include "TopExp_Explorer.hxx"
#include "BrepTools.hxx"
#include "BRep_Tool.hxx"
#include "TopoDS.hxx"
#include "TopExp.hxx"
#include "BRepFeat.hxx"
#include "BRepExtrema_DistShapeShape.hxx"
#include "Precision.hxx"
#include "SceneNode/CDataSceneNode.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif


CShapeTool::CShapeTool(void)
{
}


CShapeTool::~CShapeTool(void)
{
}

bool CShapeTool::IsSamePoint( gp_Pnt stStart, gp_Pnt stStop )
{
	return Standard_True == stStart.IsEqual(stStop, Precision::Confusion());
}

double CShapeTool::GetWallLength(gp_Pnt stStart, gp_Pnt stStop)
{
	return stStart.Distance(stStop);
}

void CShapeTool::CreateWall(const std::vector<gp_Pnt>& arrPoint, gp_Dir wallDir, float wallHeight, float wallThick, CBaseODLSPtr pObjGroup)
{
	//获取全部的墙的点
	if (arrPoint.size()<2)
	{
		return;
	}
	bool needCalcWire = 2 < arrPoint.size();

	TopoDS_Wire wallPathWire;	//路径线
	{
		BRepBuilderAPI_MakePolygon wallPath;
		for ( const auto& curPnt : arrPoint )
		{
			wallPath.Add(curPnt);
		}

		wallPathWire = wallPath.Wire();
		if ( needCalcWire )
		{
			wallPath.Close();
		}
	}
	
	TopoDS_Shape wallShell;
	float fWallLength =0.0f;
	{
		//求墙的法线
		auto& pnt1 = arrPoint[1];
		auto& pnt0 = arrPoint[0];
		fWallLength = static_cast<float>(pnt1.Distance(pnt0));
		gp_Dir lineDir(pnt1.X()-pnt0.X(), pnt1.Y()-pnt0.Y(), pnt1.Z()-pnt0.Z());
		gp_Dir dir = lineDir;
		dir.Cross(wallDir);
		auto dirEdge = BRepBuilderAPI_MakeEdge(gp_Lin(pnt0, dir), 0, wallThick).Edge();
		BRepBuilderAPI_MakeWire dirWireMaker(dirEdge);
		TopoDS_Wire dirWire = dirWireMaker.Wire();

		BRepOffsetAPI_MakePipeShell pipeMaker(wallPathWire);
		pipeMaker.SetTransitionMode(BRepBuilderAPI_RightCorner);	//延切线方向缝合
		pipeMaker.Add(dirWire);
		pipeMaker.Build();
		if (!pipeMaker.IsDone())
		{
			return;
		}
		wallShell = pipeMaker.Shape();
	}
	
	TopoDS_Wire buildWire;	//生成和路径平行的另一个线
	{
		if ( needCalcWire )
		{
			BRepBuilderAPI_MakeWire builder;
			TopTools_IndexedDataMapOfShapeListOfShape m;
			TopExp::MapShapesAndAncestors(wallShell, TopAbs_EDGE, TopAbs_FACE, m);

			for ( auto i=1; i<=m.Extent(); ++i )
			{
				auto& edge = m.FindKey(i);
				auto& list = m.FindFromKey(edge);

				if ( 1 != list.Extent() )
				{
					continue;
				}

				if ( BRepExtrema_DistShapeShape(edge, wallPathWire).Value() > 0.001 )
				{
					builder.Add(TopoDS::Edge(edge));
				}

			}

			if ( builder.IsDone() )
			{
				buildWire = builder.Wire();
			}
		}
	}

	TopoDS_Wire insideWire, ousideWire;
	{
		if ( needCalcWire )
		{
			if ( !buildWire.IsNull() )
			{
				BRepBuilderAPI_MakeFace faceBuilder1(wallPathWire);
				BRepBuilderAPI_MakeFace faceBuilder2(buildWire);

				if ( faceBuilder1.IsDone() && faceBuilder2.IsDone() )
				{
					insideWire = (Standard_True == BRepFeat::IsInside(BRepBuilderAPI_MakeFace(wallPathWire).Face(), BRepBuilderAPI_MakeFace(buildWire))) ? wallPathWire : buildWire;
					ousideWire = (Standard_True == BRepFeat::IsInside(BRepBuilderAPI_MakeFace(wallPathWire).Face(), BRepBuilderAPI_MakeFace(buildWire))) ? buildWire : wallPathWire;
				}
			}
		}
		else
		{
			insideWire = wallPathWire;
		}
	}

	TopoDS_Shape buildSolid = BRepPrimAPI_MakePrism(wallShell, gp_Vec(0, wallHeight, 0));


	auto iCount = 0;

	if (needCalcWire)
	{
		//需要生成天花板
		auto spFloor=std::make_shared<CFloorODL>(BRepBuilderAPI_MakeFace(insideWire));
		std::vector<gp_Pnt> pnts;
		for ( BRepTools_WireExplorer wireExp(insideWire); wireExp.More(); wireExp.Next() )
		{
			pnts.push_back(BRep_Tool::Pnt(wireExp.CurrentVertex()));
		}

		spFloor->SetTopPointList(pnts);
		spFloor->SetIndex(iCount);
		if ( pObjGroup )
		{
			spFloor->InitByParent(pObjGroup, "FloorODL");
		}
		spFloor->UpdateMesh();
		iCount++;
	}

	for (TopExp_Explorer exp(buildSolid, TopAbs_SOLID); exp.More(); exp.Next(),++iCount )
	{
		auto curSolid = TopoDS::Solid(exp.Current());

		auto spWall = std::make_shared<CWallODL>();
		//加入对象列表
		if ( pObjGroup )
		{
			spWall->InitByParent(pObjGroup, "WallODL");
		}
		spWall->SetSize(fWallLength, wallHeight, wallThick);
		spWall->SetBaseSolid(curSolid);
		spWall->UpdateMesh();
		spWall->CreateWallFace();
		spWall->SetIndex(iCount);
	}
}


CBaseODLSPtr CShapeTool::CreateBeamPillar(const std::vector<gp_Pnt>& arrPoint, float wallHeight, CBaseODLSPtr pObjGroup)
{
	//获取全部的墙的点
	if (arrPoint.size()<2)
	{
		return nullptr;
	}
	
	BRepBuilderAPI_MakePolygon ply;
	for ( auto& pnt : arrPoint )
	{
		ply.Add(pnt);
	}

	ply.Close();

	if ( !ply.IsDone() )
	{
		return nullptr;
	}

	auto buildSolid = TopoDS::Solid(BRepPrimAPI_MakePrism(BRepBuilderAPI_MakeFace(ply.Wire()), gp_Vec(0, wallHeight, 0)));

	auto sptr = std::make_shared<CWallODL>();
	if ( pObjGroup )
	{
		sptr->InitByParent(pObjGroup, "BeamPillar");
	}
	sptr->SetBaseSolid(buildSolid);
	sptr->UpdateMesh();
	sptr->CreateWallFace();

	return sptr;
}
