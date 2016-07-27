#include "SCADMeshTool.h"

#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepPrim_Wedge.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepMesh.hxx>
#include <BRepBndLib.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepFeat_SplitShape.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <gp_Pln.hxx>

#include <GeomAPI_ProjectPointOnSurf.hxx>

#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>

#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Connect.hxx>
#include <Poly_Triangulation.hxx>

#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <CSLib_DerivativeStatus.hxx>
#include <CSLib_NormalStatus.hxx>
#include <CSLib.hxx>

#include <Bnd_Box.hxx>
#include <Bnd_Box2d.hxx>

#include <GeomLProp_SLProps.hxx>
#include <TopTools_MapOfShape.hxx>
#include <NCollection_DataMap.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopExp.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>

#include <utility>
#include <cassert>
#include <functional>

SimpleSegment Tslt(SPntInfo& p1, SPntInfo& p2)
{
	SimpleSegment seg;
	seg[0] = p1;
	seg[1] = p2;

	return seg;
}

inline Standard_Boolean IsEqual(const TopoDS_Shape& s1, const TopoDS_Shape& s2)
{
	return s1 == s2;
}

class	SCADMeshTool::SImp
{
	class	SFaceTopoInfo
	{
	public:
		gp_Dir										theDir_;
		gp_Dir										theTangentU_;
		gp_Dir										theTangentV_;
		Standard_Real								theURange_,theVRange_;
		Handle(Geom_Surface)						theSurfaceofFace_;
		int											theIndex_;
	};

	typedef	NCollection_DataMap<TopoDS_Shape, SFaceTopoInfo>	FaceInfoMap;

	TopTools_ListOfShape						EdgeOfFrontFace_;
	FaceInfoMap									FaceInfo_;
	TopTools_IndexedDataMapOfShapeListOfShape	IndexDataMap_;

	static	const gp_Dir	s_FrontDir;

public:

	SImp(SCADMeshTool::ECoordSys sys, double zoff)
	{
		MeshSPtr_	= std::make_shared<SCADMesh>();
		CoordSys_	= sys;
		Zoffset_	= zoff;
	}

public:

	SCADMeshPtr		CreateCADMesh(const TopoDS_Shape& theShape, Standard_Real thePrecision )
	{
		TopExp::MapShapesAndAncestors(theShape, TopAbs_EDGE, TopAbs_FACE, IndexDataMap_);

		BRepMesh::Mesh(theShape, thePrecision);

		_CheckPrecision(theShape);

		_PrepareFace(theShape);

		if ( !_BuildSolidMesh() )
		{
			return SCADMeshPtr();
		}

		_BuildWrieframeMesh(theShape);

		_Build2DIndexMesh();

		return MeshSPtr_;
	}

	SCADMeshPtr		CreateBooleanMesh(const SVectorInfo& baseSize, GapsPtr gapsPtr, const SVectorInfo& subdivide)
	{
		BaseSize_ = BaseSize_;

		TopoDS_Shape tpBaseBox	= BRepPrimAPI_MakeBox(std::get<0>(baseSize), std::get<1>(baseSize), std::get<2>(baseSize)).Shape();

		if ( !gapsPtr )
		{
			//_Subdivide(tpBaseBox, subdivide);
			return CreateCADMesh(tpBaseBox, 0.1);
		}

		for ( auto& curGaps : *gapsPtr )
		{
			if ( curGaps.GapType >= SGapInfo::EGT_COUNT )
			{
				continue;
			}

			switch ( curGaps.GapType )
			{
			case SGapInfo::EGT_BOX:
				{
					TopoDS_Shape subBox = 
						BRepPrimAPI_MakeBox(gp_Pnt(curGaps.PosX, curGaps.PosY, curGaps.PosZ), 
						curGaps.Box.XLength, curGaps.Box.YLength, curGaps.Box.ZLength).Shape();

					if ( SGapInfo::EOT_CUT == curGaps.OptType )
					{
						BRepAlgoAPI_Cut cut(tpBaseBox, subBox);
						tpBaseBox = cut.Shape();
					}
				}
				break;
			case  SGapInfo::EGT_TRIANGLEPRIM:
				{
					gp_Pnt pO(curGaps.PosX, curGaps.PosY, curGaps.PosZ);
					gp_Pnt pX(curGaps.PosX + curGaps.Prim.XLength, curGaps.PosY, curGaps.PosZ);
					gp_Pnt pY(curGaps.PosX, curGaps.PosY + curGaps.Prim.YLength, curGaps.PosZ);

					TopoDS_Edge e1 =  BRepBuilderAPI_MakeEdge(pO, pX);
					TopoDS_Edge e2 =  BRepBuilderAPI_MakeEdge(pO, pY);
					TopoDS_Edge e3 =  BRepBuilderAPI_MakeEdge(pY, pX);

					TopoDS_Wire	w1 = BRepBuilderAPI_MakeWire(e1, e2, e3);

					TopoDS_Shape subPrim;

					switch (curGaps.Prim.Dir)
					{
					case STrianglePrimInfo::ETD_LT:
						{
							gp_Dir xDir(gp_Vec(pO, pX));
							gp_Pnt xPnt(curGaps.PosX, curGaps.PosY + curGaps.Prim.YLength/2, curGaps.PosZ);
							gp_Ax1 xAx(xPnt, xDir);

							gp_Trsf tf;
							tf.SetMirror(xAx);

							BRepBuilderAPI_Transform	miTrsf(w1, tf);
							TopoDS_Shape	miShape		= miTrsf.Shape();
							TopoDS_Wire		miWire		= TopoDS::Wire(miShape);
							TopoDS_Face		subFace		= BRepBuilderAPI_MakeFace(miWire);

							subPrim =  BRepPrimAPI_MakePrism(subFace, gp_Vec(0, 0, curGaps.Prim.ZLength));
						}
						break;
					case STrianglePrimInfo::ETD_RT:
						{
							gp_Dir xDir1(gp_Vec(pO, pX));
							gp_Pnt xPnt1(curGaps.PosX, curGaps.PosY + curGaps.Prim.YLength/2, curGaps.PosZ);
							gp_Ax1 xAx1(xPnt1, xDir1);
							gp_Trsf tf1;
							tf1.SetMirror(xAx1);
							
							gp_Dir xDir2(gp_Vec(pY, pO));
							gp_Pnt xPnt2(curGaps.PosX + curGaps.Prim.XLength/2, curGaps.PosY, curGaps.PosZ);
							gp_Ax1 xAx2(xPnt2, xDir2);
							gp_Trsf tf2;
							tf2.SetMirror(xAx2);

							gp_Trsf tf = tf1 * tf2;

							BRepBuilderAPI_Transform	miTrsf(w1, tf);
							TopoDS_Shape	miShape		= miTrsf.Shape();
							TopoDS_Wire		miWire		= TopoDS::Wire(miShape);
							TopoDS_Face		subFace		= BRepBuilderAPI_MakeFace(miWire);

							subPrim =  BRepPrimAPI_MakePrism(subFace, gp_Vec(0, 0, curGaps.Prim.ZLength));
						}
						break;
					case STrianglePrimInfo::ETD_LB:
						{
							TopoDS_Face subFace = BRepBuilderAPI_MakeFace(w1);
							subPrim = BRepPrimAPI_MakePrism(subFace, gp_Vec(0, 0, curGaps.Prim.ZLength));
						}
						break;
					case STrianglePrimInfo::ETD_RB:
						{
							gp_Dir xDir(gp_Vec(pY, pO));
							gp_Pnt xPnt(curGaps.PosX + curGaps.Prim.XLength/2, curGaps.PosY, curGaps.PosZ);
							gp_Ax1 xAx(xPnt, xDir);

							gp_Trsf tf;
							tf.SetMirror(xAx);

							BRepBuilderAPI_Transform	miTrsf(w1, tf);
							TopoDS_Shape	miShape		= miTrsf.Shape();
							TopoDS_Wire		miWire		= TopoDS::Wire(miShape);
							TopoDS_Face		subFace		= BRepBuilderAPI_MakeFace(miWire);

							subPrim =  BRepPrimAPI_MakePrism(subFace, gp_Vec(0, 0, curGaps.Prim.ZLength));
						}
						break;
					default: break;
					}

					if ( !subPrim.IsNull() )
					{
						BRepAlgoAPI_Cut cut(tpBaseBox, subPrim);
						tpBaseBox = cut.Shape();
					}
				}
				break;
			default:break;
			}
		}

		//_Subdivide(tpBaseBox, subdivide);
		return CreateCADMesh(tpBaseBox, 0.1);
	}

	SCADMeshPtr		CreateWireMesh(const PntsVector& pnts, const SVectorInfo& vDir, GapsPtr gapsPtr )
	{
		BRepBuilderAPI_MakePolygon buildWire;
		for (auto& perPoint : pnts )
		{
			buildWire.Add(gp_Pnt(perPoint.X, perPoint.Y, perPoint.Z));
		}
		buildWire.Close();

		TopoDS_Wire theWire = buildWire.Wire();

		gp_Vec			heiVec(std::get<0>(vDir), std::get<1>(vDir), std::get<2>(vDir));
		TopoDS_Face		bottomFace	= BRepBuilderAPI_MakeFace(theWire);
		TopoDS_Shape	theShape	= BRepPrimAPI_MakePrism(bottomFace, heiVec);

		if ( gapsPtr )
		{
			for ( auto& curGaps : *gapsPtr )
			{
				if ( curGaps.GapType >= SGapInfo::EGT_COUNT )
				{
					continue;
				}

				switch ( curGaps.GapType )
				{
				case SGapInfo::EGT_BOX:
					{
						TopoDS_Shape subBox = 
							BRepPrimAPI_MakeBox(gp_Pnt(curGaps.PosX, curGaps.PosY, curGaps.PosZ), 
							curGaps.Box.XLength, curGaps.Box.YLength, curGaps.Box.ZLength).Shape();

						if ( SGapInfo::EOT_CUT == curGaps.OptType )
						{
							BRepAlgoAPI_Cut cut(theShape, subBox);
							theShape = cut.Shape();
						}
					}
					break;
				case SGapInfo::EGT_SEGMENTS:
					{
						SWireInfo& wireRef = curGaps.Wires;

						BRepBuilderAPI_MakePolygon buildGapWire;
						for ( auto& curPnt : curGaps.Wires.Pnts )
						{
							buildGapWire.Add(gp_Pnt(curPnt.X, curPnt.Y, curPnt.Z));
						}
						buildGapWire.Close();

						TopoDS_Wire theGapWire = buildGapWire.Wire();

						auto& theDir = wireRef.Dir;

						gp_Vec			GapHeiVec(std::get<0>(theDir), std::get<1>(theDir), std::get<2>(theDir));
						TopoDS_Face		GapbottomFace	= BRepBuilderAPI_MakeFace(theGapWire);
						TopoDS_Shape	theGapShape	= BRepPrimAPI_MakePrism(GapbottomFace, GapHeiVec);

						if ( SGapInfo::EOT_CUT == curGaps.OptType )
						{
							BRepAlgoAPI_Cut cut(theShape, theGapShape);
							theShape = cut.Shape();
						}

					}break;
				default:break;
				}
			}
		}
		

		return CreateCADMesh(theShape, 0.01);
	}
private:

	bool			_CheckPrecision(const TopoDS_Shape& theShape)
	{
		Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
		Bnd_Box Total;
		BRepBndLib::Add(theShape, Total);
		if ( Total.IsVoid())
		{
			return false;
		}

		Total.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);

		double theDeflection = std::max(std::max(aXmax-aXmin , aYmax-aYmin), aZmax-aZmin)*0.001;

		if ( theDeflection <= Precision::Confusion () )
		{
			assert(0);
			return false;
		}

		return true;
	}

	bool			_PrepareFace(const TopoDS_Shape& theShape)
	{
		int index = 0;
		for (TopExp_Explorer itf(theShape,TopAbs_FACE); itf.More(); itf.Next())
		{
			//Face
			TopoDS_Face face = TopoDS::Face(itf.Current());			
			
			SFaceTopoInfo newInfo;
			_GetTopoFaceInfo(face, newInfo);
			newInfo.theIndex_ = index++;
			
			if ( newInfo.theDir_.IsEqual(s_FrontDir, 0.01) )
			{
				for (TopExp_Explorer ite(face,TopAbs_EDGE); ite.More(); ite.Next())
				{
					EdgeOfFrontFace_.Append(ite.Current());
				}
			}

			FaceInfo_.Bind(itf.Current(), newInfo);
		}

		return true;
	}

	bool			_BuildSolidMesh()
	{
		for ( FaceInfoMap::Iterator it(FaceInfo_); it.More(); it.Next() )
		{
			SFaceInfo fi;

			TopoDS_Face face				= TopoDS::Face(it.Key());
			const SFaceTopoInfo& theInfo	= it.Value();

			fi.theIndex = theInfo.theIndex_;

			TopLoc_Location faceLoc;
			Handle(Poly_Triangulation)	faceTriangulation = BRep_Tool::Triangulation(face, faceLoc);
			
			if (faceTriangulation.IsNull())
			{
				return false; //Meshing was not done for this face!
			}

			// points
			for ( int i=0; i<faceTriangulation->NbNodes(); ++i )
			{
				gp_Pnt	rawPnt = faceTriangulation->Nodes().Value(i+1);

				SPntInfo pnt;
				
				//UV
				GeomAPI_ProjectPointOnSurf projpnta(rawPnt, theInfo.theSurfaceofFace_);
				projpnta.LowerDistanceParameters(pnt.U, pnt.V);

				//pos
				rawPnt.Transform(faceLoc.Transformation());
				rawPnt.Coord(pnt.X, pnt.Y, pnt.Z);

				//nomals
				theInfo.theDir_.Coord(pnt.nX, pnt.nY, pnt.nZ);

				fi.thePnts.push_back(pnt);
			}

			//index
			for ( int i=1; i<=faceTriangulation->Triangles().Length(); ++i)
			{
				Standard_Integer IndexPnt1, IndexPnt2, IndexPnt3;
				Poly_Triangle triangle = faceTriangulation->Triangles().Value(i);
				triangle.Get(IndexPnt1, IndexPnt2, IndexPnt3);
				gp_Pnt P1, P2, P3;
				P1 = faceTriangulation->Nodes().Value(IndexPnt1);
				P2 = faceTriangulation->Nodes().Value(IndexPnt2);
				P3 = faceTriangulation->Nodes().Value(IndexPnt3);
				gp_Vec average = theInfo.theDir_;   

				//check angle between vectors
				gp_Vec VecP1P2(P1, P2), VecP2P3(P2, P3);
				Standard_Integer trueIndexP1,trueIndexP2,trueIndexP3;
				gp_Vec vec = VecP1P2^VecP2P3;
				Standard_Real modul1, modul2;
				modul1 = average.XYZ().Modulus();
				modul2 = vec.XYZ().Modulus();
				if (modul2>Precision::Confusion ())
				{
					vec.Divide(modul2);
				}

				trueIndexP1	=	IndexPnt1;
				trueIndexP2	=	IndexPnt2;
				trueIndexP3	=	IndexPnt3;

				if (modul1>Precision::Confusion () && modul2>Precision::Confusion ())
				{
					Standard_Real an = vec.Angle(average);
					if ( an > M_PI/2) 
					{
						trueIndexP1	=	IndexPnt3;
						trueIndexP2	=	IndexPnt2;
						trueIndexP3	=	IndexPnt1;
					}
					else
					{
						trueIndexP1	=	IndexPnt1;
						trueIndexP2	=	IndexPnt2;
						trueIndexP3	=	IndexPnt3;
					}
				}

				if ( CoordSys_ == ECS_LEFTHAND )
				{
					fi.theTriangles.emplace_back(trueIndexP1-1, trueIndexP2-1, trueIndexP3-1);
				}
				else
				{
					fi.theTriangles.emplace_back(trueIndexP3-1, trueIndexP2-1, trueIndexP1-1);
				}
			}

			MeshSPtr_->PushFace(fi);
		}

		return true;
	}

	bool			_BuildWrieframeMesh(const TopoDS_Shape& theShape)
	{
		for ( TopExp_Explorer ite(theShape, TopAbs_EDGE); ite.More(); ite.Next() )
		{
			TopoDS_Edge edge = TopoDS::Edge(ite.Current());

			if ( TopAbs_REVERSED == edge.Orientation() )
			{
				continue;
			}
			
			Segments				dest;
			_TransEdgeToSegs(edge, dest);

			MeshSPtr_->PushSegments(dest);
		}

		return true;
	}

	void			_Build2DIndexMesh()
	{
		for( TopTools_ListIteratorOfListOfShape itEdge(EdgeOfFrontFace_); itEdge.More(); itEdge.Next() )
		{
			SIndexSegmentInfo indexSeg;

			const TopTools_ListOfShape& lst =  IndexDataMap_.FindFromKey(itEdge.Value());
			
			for ( TopTools_ListIteratorOfListOfShape itFace(lst); itFace.More(); itFace.Next() )
			{
				const SFaceTopoInfo& faceTopoInfo = FaceInfo_.Find(itFace.Value());
				if ( faceTopoInfo.theDir_.IsEqual(s_FrontDir, 0.01) )
				{
					continue;
				}

				indexSeg.theFaceIndex = faceTopoInfo.theIndex_;
				break;
			}

			TopoDS_Edge edge = TopoDS::Edge(itEdge.Value());

			_TransEdgeToSegs(edge, indexSeg.theSegments);

			MeshSPtr_->PushIndexSegmenst(indexSeg);
		}
	}

	void			_GetTopoFaceInfo(const TopoDS_Face& theFace, SFaceTopoInfo& theFaceInfo)
	{
		Standard_Real umin, umax, vmin, vmax;
		BRepTools::UVBounds(theFace, umin, umax, vmin, vmax);	// create surface

		Handle(Geom_Surface) surf=BRep_Tool::Surface(theFace);	// get surface properties
		GeomLProp_SLProps props(surf, umin, vmin, 1, 0.01);		// get surface normal
		props.TangentU(theFaceInfo.theTangentU_);
		props.TangentV(theFaceInfo.theTangentV_);
		gp_Dir norm=props.Normal();								// check orientation
		if( TopAbs_REVERSED == theFace.Orientation() )
		{
			norm.Reverse();
		}

		theFaceInfo.theDir_				= norm;
		theFaceInfo.theSurfaceofFace_	= surf;
		theFaceInfo.theURange_			= umax-umin;
		theFaceInfo.theVRange_			= vmax-vmin;
	}

	void			_TransEdgeToSegs(const TopoDS_Edge& theEdge, Segments& segContainer)
	{
		PntsVector	pOfeVec;
		for ( TopExp_Explorer itv(theEdge, TopAbs_VERTEX); itv.More(); itv.Next() )
		{
			TopoDS_Vertex vertice = TopoDS::Vertex(itv.Current());
			gp_Pnt curPnt = BRep_Tool::Pnt(vertice);
			SPntInfo pnt;
			curPnt.Coord(pnt.X, pnt.Y, pnt.Z);
			pOfeVec.push_back(pnt);
		}
		assert ( pOfeVec.size() >= 2 );
		PntsVector::iterator itor1 = pOfeVec.begin();
		PntsVector::iterator itor2 = itor1+1;
		std::transform(itor2, pOfeVec.end(), itor1, std::back_inserter(segContainer), std::bind(Tslt, std::placeholders::_1, std::placeholders::_2));
	}

	void			_Subdivide(TopoDS_Shape& theShape, const SVectorInfo& subdivide )
	{
		TopoDS_Shape tmpShape = theShape;

		if ( std::abs(std::get<0>(subdivide)) >0.00001 )
		{
			for ( double offset=std::get<0>(subdivide); offset<std::get<0>(BaseSize_); offset+=std::get<0>(subdivide) )
			{
				gp_Pln thePlan( 1, 0, 0, -offset);
				tmpShape = __SubdivideByPlan(tmpShape, thePlan);
			}
		}

		if ( std::abs(std::get<1>(subdivide)) >0.00001 )
		{
			for ( double offset=std::get<1>(subdivide); offset<std::get<1>(BaseSize_); offset+=std::get<1>(subdivide) )
			{
				gp_Pln thePlan( 0, 1, 0, -offset);
				tmpShape = __SubdivideByPlan(tmpShape, thePlan);
			}
		}

		if ( std::abs(std::get<2>(subdivide)) >0.00001 )
		{
			for ( double offset=std::get<2>(subdivide); offset<std::get<2>(BaseSize_); offset+=std::get<2>(subdivide) )
			{
				gp_Pln thePlan( 0, 0, 1, -offset);
				tmpShape = __SubdivideByPlan(tmpShape, thePlan);
			}
		}

		theShape = tmpShape;
	}

	TopoDS_Shape	__SubdivideByPlan(TopoDS_Shape& theShape, gp_Pln& gpln)
	{
		BRepFeat_SplitShape splitShapeBuilder(theShape);

		BRepAlgoAPI_Section Section(theShape, gpln,Standard_False);	
		Section.ComputePCurveOn1(Standard_True);	
		Section.Approximation(Standard_True);	
		Section.Build();
		TopoDS_Shape R = Section.Shape();
		for (TopExp_Explorer Ex(R,TopAbs_EDGE); Ex.More(); Ex.Next())
		{	
			TopoDS_Shape anEdge = Ex.Current();	
			TopoDS_Shape aFace;	
			if (Section.HasAncestorFaceOn1(anEdge,aFace))
			{	
				TopoDS_Face F = TopoDS::Face(aFace);	
				TopoDS_Edge E = TopoDS::Edge(anEdge);	
				splitShapeBuilder.Add(E,F);	
			}	
		}
		splitShapeBuilder.Build();

		return splitShapeBuilder.Shape();
	}

public:

	SCADMeshPtr					MeshSPtr_;
	Handle(Geom_Surface)		mFaceUVPlan;
	TopLoc_Location				mFaceLoc;

	SVectorInfo					BaseSize_;

	SCADMeshTool::ECoordSys		CoordSys_;
	double						Zoffset_;
};

const gp_Dir	SCADMeshTool::SImp::s_FrontDir(0, 0, 1);


SCADMeshTool::SCADMeshTool( ECoordSys sys, double zoff )
{
	SImpPtr = std::make_shared<SCADMeshTool::SImp>(sys, zoff);
}


GapsPtr		SCADMeshTool::CreateGapsPtr()
{
	return std::make_shared<Gaps>();
}


DLL_EXPORT SCADMeshPtr SCADMeshTool::CreateBooleanMesh( const SVectorInfo& baseSize, GapsPtr gapsPtr, ECoordSys sys )
{
	SCADMeshTool creator(sys, std::get<2>(baseSize));
	return creator.SImpPtr->CreateBooleanMesh(baseSize, gapsPtr, std::make_tuple(0,0,0));
}

SCADMeshPtr	SCADMeshTool::CreateWireMesh(const PntsVector& pnts, const SVectorInfo& vDir, GapsPtr gapsPtr, ECoordSys sys )
{
	SCADMeshTool creator(sys, 0);
	return creator.SImpPtr->CreateWireMesh(pnts, vDir, gapsPtr);
}