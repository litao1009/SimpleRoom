#include "stdafx.h"

#include "DrawingLineWallCtrller.h"
#include "IrrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"

#include "ODL/ODLTools.h"
#include "ODL/BaseODL.h"
#include "ODL/GroupODL.h"

#include "SMeshBuffer.h"

#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "gp_Pnt.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "TopExp_Explorer.hxx"
#include "TopExp.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"
#include "Geom2d_Circle.hxx"
#include "Geom2dAdaptor_Curve.hxx"
#include "GCPnts_UniformAbscissa.hxx"
#include "BRepGProp_EdgeTool.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "BRepAdaptor_Surface.hxx"

#include "StatusMgr.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

static	irr::core::vector3df s_PntNormal(0,-1,0);
static	irr::core::vector2df s_PntCoord(0,0);
static	irr::core::plane3df	 s_PntPlane(0,0,0,0,1,0);
static	irr::video::SColor	 s_PntColor(~0);
static	irr::video::SColor	 s_LineColor(0xFF000000);
static	irr::video::SColor	 s_PathColor(0xFF0000FF);
static	irr::video::SColor	 s_RedColor(0xFFFF0000);

DrawingLineWallCtrller::DrawingLineWallCtrller()
{
	MeshBuf_ = nullptr;
	LineMeshBuf_ = nullptr;
	
	{
		PathMeshBuf_ = new irr::scene::SMeshBuffer;
		PathMeshBuf_->getMaterial().Lighting = false;
		PathMeshBuf_->getMaterial().ZWriteEnable = false;
		PathMeshBuf_->getMaterial().BackfaceCulling = false;
		PathMeshBuf_->getMaterial().Thickness = 3;
		PathMeshBuf_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
		PathMeshBuf_->getMaterial().DiffuseColor = s_LineColor;
	}

	{
		TmpRect_ = new irr::scene::SMeshBuffer;
		irr::core::vector3df defaultPos;
		auto smeshBuf = static_cast<irr::scene::SMeshBuffer*>(TmpRect_);
		smeshBuf->Vertices.reallocate(4);
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(1,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(0,1)));
		smeshBuf->Indices.reallocate(4);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(3);

		TmpRect_->getMaterial().Lighting = false;
		TmpRect_->getMaterial().ZWriteEnable = false;
		TmpRect_->getMaterial().BackfaceCulling = false;
		TmpRect_->getMaterial().Thickness = 3;
		TmpRect_->getMaterial().DiffuseColor = irr::video::SColor(0xFF000000);
	}

	{
		PntMeshBuf_ = new irr::scene::SMeshBuffer;
		auto smeshBuf = static_cast<irr::scene::SMeshBuffer*>(PntMeshBuf_);
		smeshBuf->Vertices.reallocate(4);
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(irr::core::vector3df(-10, 0, -10), s_PntNormal, s_PathColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(irr::core::vector3df(10, 0, -10), s_PntNormal, s_PathColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(irr::core::vector3df(10, 0, 10), s_PntNormal, s_PathColor, irr::core::vector2df(1,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(irr::core::vector3df(10, 0, -10), s_PntNormal, s_PathColor, irr::core::vector2df(0,1)));
		smeshBuf->Indices.reallocate(4);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(3);

		PntMeshBuf_->getMaterial().Lighting = false;
		PntMeshBuf_->getMaterial().ZWriteEnable = false;
		PntMeshBuf_->getMaterial().BackfaceCulling = false;
		PntMeshBuf_->getMaterial().Thickness = 3;
	}

	CircleMeshBuf_ = new irr::scene::SMeshBuffer;
	{
		auto smeshBuf = static_cast<irr::scene::SMeshBuffer*>(CircleMeshBuf_);
		smeshBuf->Vertices.reallocate(360);
		smeshBuf->Indices.reallocate(360);

		Handle(Geom2d_Circle) C = new Geom2d_Circle(gp::OX2d(),1);
		Geom2dAdaptor_Curve GAC (C);
		GCPnts_UniformAbscissa UA (GAC,360);
		assert(UA.IsDone());

		gp_Pnt2d P;
		irr::video::SColor	 clr(0xFF000000);
		bool bClr = true;
		for( auto index=1, count=UA.NbPoints(); index<=count; ++index )
		{
			if ( index % 10 == 0 )
			{
				bClr= !bClr;
			}

			C->D0(UA.Parameter(index),P);
			
			irr::core::vector3df pos(static_cast<float>(P.X()), 0, static_cast<float>(-P.Y()));
			smeshBuf->Vertices.push_back(irr::video::S3DVertex(pos, s_PntNormal, bClr ? s_PntColor : clr, s_PntCoord));
			smeshBuf->Indices.push_back(index-1);
		}

		CircleMeshBuf_->getMaterial().Lighting = false;
		CircleMeshBuf_->getMaterial().ZWriteEnable = false;
		CircleMeshBuf_->getMaterial().BackfaceCulling = false;
		CircleMeshBuf_->getMaterial().Thickness = 3;
	}

	NeedUpdateMesh_ = false;
	Checker_ = true;

	WallThickness_ = 300;
	LastAngle_ = 0;

	State_ = EDWLS_BEGIN;

	PolarAngle_ = 30.f;
	PolarAngleRange_ = 10.f;

	SetName("DrawingLineWallCtrller");
}

DrawingLineWallCtrller::~DrawingLineWallCtrller()
{
	if ( MeshBuf_ )
	{
		MeshBuf_->drop();
	}

	if ( LineMeshBuf_ )
	{
		LineMeshBuf_->drop();
	}

	if ( TmpRect_ )
	{
		TmpRect_->drop();
	}

	if ( PathMeshBuf_ )
	{
		PathMeshBuf_->drop();
	}

	if ( CircleMeshBuf_ )
	{
		CircleMeshBuf_->drop();
	}

	if ( PntMeshBuf_ )
	{
		PntMeshBuf_->drop();
	}
}

bool DrawingLineWallCtrller::OnPostEvent( const irr::SEvent& event )
{
	if ( !IsEnable() )
	{
		return false;
	}

	if ( StatusMgr::GetInstance().DrawingState_ != StatusMgr::EDS_LINE_WALL )
	{
		return false;
	}

	if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
	{
		if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
		{
			CursorIPos_.X = event.MouseInput.X;
			CursorIPos_.Y = event.MouseInput.Y;

			return false;
		}
	}

	switch (State_)
	{
	case DrawingLineWallCtrller::EDWLS_BEGIN:
		{
			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
				{
					State_ = EDWLS_DRAWING;
					
					Pnts_.push_back(CurrentPos_);

					return true;
				}
			}
		}
		break;
	case DrawingLineWallCtrller::EDWLS_DRAWING:
		{
			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
				{
					if ( !Checker_ )
					{
						return true;
					}

					Pnts_.push_back(CurrentPos_);

					NeedUpdateMesh_ = true;
					Checker_ = true;

					return true;
				}
			}

			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN )
				{
					State_ = EDWLS_FINISH;
					return true;
				}
			}

			if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
			{
				if ( event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown )
				{
					assert(!Pnts_.empty());
					
					Pnts_.pop_back();

					NeedUpdateMesh_ = true;

					if ( Pnts_.size() == 1 )
					{
						MeshBuf_->drop();
						LineMeshBuf_->drop();
						MeshBuf_ = nullptr;
						LineMeshBuf_ = nullptr;
					}

					if ( Pnts_.empty() )
					{
						State_ = EDWLS_BEGIN;
					}

					return true;
				}
			}
		}
		break;
	default:
		break;
	}

	return false;
}

bool DrawingLineWallCtrller::PreRender3D()
{
	SetEnable( StatusMgr::GetInstance().DrawingState_ == StatusMgr::EDS_LINE_WALL );

	if ( !IsEnable() )
	{
		Reset();
		return false;
	}

	UpdateMesh();

	Checker_ = true;

	auto smgr = GetRenderContextSPtr()->Smgr_.get();
	auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
	s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), CurrentPos_);

	switch (State_)
	{
	case DrawingLineWallCtrller::EDWLS_BEGIN:
		//break;
	case DrawingLineWallCtrller::EDWLS_DRAWING:
		{
			auto smgr = GetRenderContextSPtr()->Smgr_.get();

			if ( !Pnts_.empty() && (Pnts_.back().getDistanceFromSQ(CurrentPos_) < WallThickness_*WallThickness_) )
			{
				Checker_ = false;
			}

			bool foundAlign = false;

			//吸附已生成的点
			for ( auto& existPnt : Pnts_ )
			{
				if ( existPnt.getDistanceFromSQ(CurrentPos_) <= 100 * 100 )
				{
					CurrentPos_ = existPnt;
					foundAlign = true;
					break;
				}
			}

			//吸附其他墙面
			if ( !foundAlign )
			{
				gp_Pnt curPnt(CurrentPos_.X, CurrentPos_.Y, CurrentPos_.Z);
				auto curVertex = BRepBuilderAPI_MakeVertex(curPnt).Vertex();
				Bnd_Box bb;
				bb.Add(curPnt);

				assert(!CurrentRoot_.expired());
				auto spRoot = CurrentRoot_.lock();
				for ( auto& curGroup : spRoot->GetChildrenList() )
				{
					if ( curGroup->GetType() != EODLT_GROUP )
					{
						continue;
					}

					if ( curGroup->GetBaseBndBox().Distance(bb.Transformed(curGroup->GetAbsoluteTransform().Inverted())) > WallThickness_/2 )
					{
						continue;
					}

					auto needBreak = false;

					for ( auto& curWall : curGroup->GetChildrenList() )
					{
						if ( curWall->GetType() != EODLT_WALL )
						{
							continue;
						}

						auto inverted = curWall->GetAbsoluteTransform().Inverted();
						
						if ( curWall->GetBaseBndBox().Distance(bb.Transformed(inverted)) > WallThickness_/2 )
						{
							continue;
						}

						auto& selector = curWall->GetBaseSelector();
						selector.LoadS2(curVertex.Moved(inverted));
						selector.Perform();

						if ( 0 == selector.NbSolution() )
						{
							continue;
						}

						auto distance = selector.Value();
						if ( distance > WallThickness_/2 )
						{
							continue;
						}

						if ( BRepExtrema_IsVertex == selector.SupportTypeShape1(1) )
						{
							curVertex = TopoDS::Vertex(selector.SupportOnShape1(1));
							curPnt = BRep_Tool::Pnt(curVertex);
						}
						else if ( BRepExtrema_IsOnEdge == selector.SupportTypeShape1(1) )
						{
							Standard_Real curPar;
							selector.ParOnEdgeS1(1, curPar);
							
							auto curEdge = TopoDS::Edge(selector.SupportOnShape1(1));
							BRepAdaptor_Curve edgeAdaptor(curEdge);
							auto firstL = BRepGProp_EdgeTool::FirstParameter(edgeAdaptor);
							auto lastL = BRepGProp_EdgeTool::LastParameter(edgeAdaptor);

							auto fVertex = BRep_Tool::Pnt(TopExp::FirstVertex(curEdge));
							auto lVertex = BRep_Tool::Pnt(TopExp::LastVertex(curEdge));

							auto adjCurPar = curPar - firstL;
							auto adjLastL = lastL - firstL;

							if ( adjCurPar < WallThickness_/2 )//起点
							{
								curVertex = TopExp::FirstVertex(curEdge);
								curPnt = BRep_Tool::Pnt(curVertex);
							}
							else if ( adjLastL - adjCurPar < WallThickness_/2 )//终点
							{
								curVertex = TopExp::LastVertex(curEdge);
								curPnt = BRep_Tool::Pnt(curVertex);
							}
							else if ( std::abs(WallThickness_ - adjCurPar) < WallThickness_/2 )
							{
								edgeAdaptor.D0(firstL+WallThickness_, curPnt);
							}
							else if ( std::abs(adjLastL-WallThickness_ - adjCurPar) < WallThickness_/2 )
							{
								edgeAdaptor.D0(lastL-WallThickness_, curPnt);
							}
							else
							{
								curPnt = selector.PointOnShape1(1);
							}
						}
						else if ( BRepExtrema_IsInFace == selector.SupportTypeShape1(1) )
						{
							Standard_Real u,v, uMin,uMax,vMin,vMax;
							selector.ParOnFaceS1(1, u, v);
							auto curFace = TopoDS::Face(selector.SupportOnShape1(1));
 							BRepAdaptor_Surface bs(curFace);
							uMin = bs.FirstUParameter();
							uMax = bs.LastUParameter();
							vMin = bs.FirstVParameter();
							vMax = bs.LastVParameter();

							auto adjU = u - uMin;
							auto adjV = v - vMin;
							auto adjUMax = uMax - uMin;
							auto adjVMax = vMax - vMin;

							auto uDis = adjU<uMax-adjU ? adjU : uMax-adjU;
							auto vDis = adjV<vMax-adjV ? adjV : vMax-adjV;

							if ( uDis < vDis )
							{
								adjU = adjU<uMax-adjU ? 0 : uMax;

								if ( std::abs(adjV-WallThickness_) < WallThickness_/2 )
								{
									adjV = WallThickness_;
								}
								else if ( std::abs((adjVMax-WallThickness_) - adjV) < WallThickness_/2 )
								{
									adjV = adjVMax - WallThickness_;
								}
							}
							else
							{
								adjV = adjV<vMax-adjV ? 0 : vMax;

								if ( std::abs(adjU-WallThickness_) < WallThickness_/2 )
								{
									adjU = WallThickness_;
								}
								else if ( std::abs((adjUMax-WallThickness_) - adjU) < WallThickness_/2 )
								{
									adjU = adjUMax - WallThickness_;
								}
							}

							bs.D0(adjU+uMin, adjV+vMin, curPnt);
						}

						curPnt.Transform(curWall->GetAbsoluteTransform());

						needBreak = true;
						break;
					}

					if ( needBreak )
					{
						CurrentPos_.X = static_cast<float>(curPnt.X());
						CurrentPos_.Y = static_cast<float>(curPnt.Y());
						CurrentPos_.Z = static_cast<float>(curPnt.Z());

						foundAlign = true;
						break;
					}
				}
			}

			if ( !foundAlign )
			{
				if ( StatusMgr::GetInstance().GridAlign_ )
				{
					CurrentPos_ = *StatusMgr::GetInstance().GridAlign_;
				}
			}

			//检查角度
			if ( Pnts_.size() >= 2 )
			{
				auto& back2 = Pnts_[Pnts_.size()-2];
				auto vec1 = (back2 - Pnts_.back()).normalize();
				auto vec2 = (CurrentPos_ - Pnts_.back()).normalize();
				if ( vec1.dotProduct(vec2) > std::cos(irr::core::degToRad(15.f)) )
				{
					Checker_ = false;
				}
			}

			//极轴追踪
			if ( !Pnts_.empty() && !foundAlign && !StatusMgr::GetInstance().GridAlign_ && Checker_ )
			{
				vector3df refDir;
				if ( Pnts_.size() > 1 )
				{
					refDir = (Pnts_.back()-Pnts_[Pnts_.size()-2]).normalize();
				}
				else
				{
					auto p1 = smgr->getVideoDriver()->getViewPort().UpperLeftCorner;
					auto p2 = smgr->getVideoDriver()->getViewPort().LowerRightCorner;
					auto line1 = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(p1);
					auto line2 = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(vector2di(p2.X, p1.Y));
					vector3df wp1,wp2;
					s_PntPlane.getIntersectionWithLine(line1.start, line1.getVector(), wp1);
					s_PntPlane.getIntersectionWithLine(line2.start, line2.getVector(), wp2);
					refDir = (wp2-wp1).normalize();
				}

				auto curVec = CurrentPos_ - Pnts_.back();
				auto curDir = curVec;
				auto curLength = curVec.getLength();
				curDir.normalize();

				gp_Dir dRef(refDir.X, refDir.Y, refDir.Z);
				gp_Dir dDir(curDir.X, curDir.Y, curDir.Z);

				auto rad = dRef.AngleWithRef(dDir, gp::DY());
				rad = rad < 0 ? M_PI * 2 + rad : rad;
				auto angle = irr::core::radToDeg(rad);

				auto rest = std::fmod(angle, PolarAngle_);
				if ( rest < PolarAngleRange_/2 )
				{
					angle -= rest;
				}
				if ( PolarAngle_-rest < PolarAngleRange_/2 )
				{
					angle += PolarAngle_ - rest;
				}

				auto newDir = dRef.Rotated(gp::OY(), irr::core::degToRad(angle));

				CurrentPos_ = Pnts_.back() + vector3df(static_cast<float>(newDir.X()), static_cast<float>(newDir.Y()), static_cast<float>(newDir.Z())) * curLength;
			}
			
			if ( !Pnts_.empty() )
			{//TmpRect_
				auto& lastPnt = Pnts_.back();
				vector3df dirVec = CurrentPos_ - lastPnt;
				auto thickDir = dirVec.normalize().crossProduct(s_PntNormal);

				TmpRect_->getPosition(0) = lastPnt;
				TmpRect_->getPosition(1) = CurrentPos_;
				TmpRect_->getPosition(2) = CurrentPos_ + thickDir * WallThickness_;
				TmpRect_->getPosition(3) = lastPnt + thickDir * WallThickness_;
			}
		}
		break;
	case DrawingLineWallCtrller::EDWLS_FINISH:
		{
			if ( Pnts_.size() <= 1 )
			{
				FaceShape_.Nullify();
			}

			if ( !FaceShape_.IsNull() )
			{				
				auto newGroup = CGroupODL::CreateByWallPath(GetRenderContextSPtr(), FaceShape_, Pnts_, StatusMgr::GetInstance().CreateWallHeight_);
				CurrentRoot_.lock()->AddChild(newGroup);
			}

			StatusMgr::GetInstance().DrawingState_ = StatusMgr::EDS_NONE;
			Reset();
		}
		break;
	case DrawingLineWallCtrller::EDWLS_COUNT:
		break;
	default:
		break;
	}

	return false;
}

void DrawingLineWallCtrller::PostRender3D()
{
	if ( !IsEnable() )
	{
		return;
	}

	auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();
	auto tmpRectSmesh = static_cast<irr::scene::SMeshBuffer*>(TmpRect_);

	switch (State_)
	{
	case DrawingLineWallCtrller::EDWLS_BEGIN:
		{
			driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());

			auto radius = 32.f;
			auto p1 = CurrentPos_ + vector3df(-radius, 0, -radius);
			auto p2 = CurrentPos_ + vector3df(radius, 0, -radius);
			auto p3 = CurrentPos_ + vector3df(radius, 0, radius);
			auto p4 = CurrentPos_ + vector3df(-radius, 0, radius);
			PntMeshBuf_->getPosition(0) = p1;
			PntMeshBuf_->getPosition(1) = p2;
			PntMeshBuf_->getPosition(2) = p3;
			PntMeshBuf_->getPosition(3) = p4;

			driver->setMaterial(PntMeshBuf_->getMaterial());
			driver->drawVertexPrimitiveList(PntMeshBuf_->getVertices(), PntMeshBuf_->getVertexCount(), PntMeshBuf_->getIndices(), PntMeshBuf_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);
		}
		break;
	case DrawingLineWallCtrller::EDWLS_DRAWING:
		{
			driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());

			if ( MeshBuf_ )
			{
				driver->setMaterial(MeshBuf_->getMaterial());
				driver->drawMeshBuffer(MeshBuf_);
			}

			if ( LineMeshBuf_ )
			{
				driver->setMaterial(LineMeshBuf_->getMaterial());
				driver->drawVertexPrimitiveList(LineMeshBuf_->getVertices(), LineMeshBuf_->getVertexCount(), LineMeshBuf_->getIndices(), LineMeshBuf_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			{
				TmpRect_->getMaterial().MaterialType = EMT_SOLID;
				driver->setMaterial(TmpRect_->getMaterial());
				driver->drawVertexPrimitiveList(TmpRect_->getVertices(), TmpRect_->getVertexCount(), TmpRect_->getIndices(), TmpRect_->getIndexCount()-2, EVT_STANDARD, EPT_TRIANGLE_FAN);
				
				TmpRect_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
				driver->setMaterial(TmpRect_->getMaterial());
				driver->drawVertexPrimitiveList(TmpRect_->getVertices(), TmpRect_->getVertexCount(), TmpRect_->getIndices(), TmpRect_->getIndexCount(), EVT_STANDARD, EPT_LINE_LOOP);
			}

			if ( PathMeshBuf_->getIndexCount() > 1 )
			{
				driver->setMaterial(PathMeshBuf_->getMaterial());
				driver->drawVertexPrimitiveList(PathMeshBuf_->getVertices(), PathMeshBuf_->getVertexCount(), PathMeshBuf_->getIndices(), PathMeshBuf_->getIndexCount()-1, EVT_STANDARD, EPT_LINE_STRIP);
			}

			irr::video::SMaterial mat;
			mat.Lighting = false;
			mat.ZWriteEnable = false;
			mat.BackfaceCulling = false;
			mat.Thickness = 3;
			driver->setMaterial(mat);
			if ( Checker_ )
			{
				driver->draw3DLine(Pnts_.back(), CurrentPos_, s_PathColor);
			}
			else
			{
				driver->draw3DLine(Pnts_.back(), CurrentPos_, s_RedColor);
			}

			if ( Pnts_.size() >= 2 && CurrentPos_.getDistanceFromSQ(Pnts_.back()) > 1 )//Circle Mesh
			{
				irr::core::matrix4 t,r,s;
				s.setScale(irr::core::vector3df(500, 1, 500));
				t.setTranslation(Pnts_.back());

				auto vec1 = (Pnts_[Pnts_.size()-2] - Pnts_.back()).normalize();
				auto vec2 = (CurrentPos_ - Pnts_.back()).normalize();

				gp_Dir d1(vec1.X, vec1.Y, vec1.Z),d2(vec2.X, vec2.Y, vec2.Z);
				auto angle = irr::core::radToDeg(d1.AngleWithRef(d2, gp::DY()));

				auto angleCount = static_cast<int>(angle + (angle < 0 ? -.5f : .5f));
				angleCount = std::abs(angleCount);

				auto xAngle = static_cast<float>(d1.AngleWithRef(gp::DX(), gp::DY()));
				auto xAngle2 = static_cast<float>(d2.AngleWithRef(gp::DX(), gp::DY()));

				r.setRotationRadians(irr::core::vector3df(0, angle > 0 ? -xAngle : -xAngle2, 0));
				driver->setMaterial(CircleMeshBuf_->getMaterial());
				driver->setTransform(irr::video::ETS_WORLD, t*r*s);
				driver->drawVertexPrimitiveList(CircleMeshBuf_->getVertices(), CircleMeshBuf_->getVertexCount(), CircleMeshBuf_->getIndices(), angleCount, EVT_STANDARD, EPT_LINE_STRIP);
			}
		}
		break;
	case DrawingLineWallCtrller::EDWLS_FINISH:
		{

		}
		break;
	case DrawingLineWallCtrller::EDWLS_COUNT:
		break;
	default:
		break;
	}
}

void DrawingLineWallCtrller::Reset()
{
	if ( MeshBuf_ )
	{
		MeshBuf_->drop();
		MeshBuf_ = nullptr;
	}

	if ( LineMeshBuf_ )
	{
		LineMeshBuf_->drop();
		LineMeshBuf_ = nullptr;
	}

	if ( PathMeshBuf_ )
	{
		static_cast<irr::scene::SMeshBuffer*>(PathMeshBuf_)->Vertices.clear();
		static_cast<irr::scene::SMeshBuffer*>(PathMeshBuf_)->Indices.clear();
	}
	
	Pnts_.clear();

	Checker_ = true;
	State_ = EDWLS_BEGIN;
}

bool DrawingLineWallCtrller::UpdateMesh()
{
	if ( !NeedUpdateMesh_ )
	{
		return true;
	}

	if ( Pnts_.size() > 1 )
	{
		{//Triangle Mesh
			BRepBuilderAPI_MakePolygon mp;
			for ( const auto& curPnt : Pnts_ )
			{
				mp.Add(gp_Pnt(curPnt.X, curPnt.Y, curPnt.Z));
			}
			assert(mp.IsDone());

			auto wallPath = mp.Wire();
			assert(!wallPath.IsNull());

			auto& wallPnt1 = Pnts_[0];
			auto& wallPnt2 = Pnts_[1];

			auto beginThickPnt = (wallPnt2 - wallPnt1).normalize().crossProduct(s_PntNormal) * WallThickness_;
			beginThickPnt = wallPnt1 + beginThickPnt;

			auto thickEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(wallPnt1.X,wallPnt1.Y, wallPnt1.Z), gp_Pnt(beginThickPnt.X,beginThickPnt.Y, beginThickPnt.Z) ).Edge();
			BRepBuilderAPI_MakeWire dirWireMaker(thickEdge);
			TopoDS_Wire dirWire = dirWireMaker.Wire();

			BRepOffsetAPI_MakePipeShell pipeMaker(wallPath);
			pipeMaker.SetTransitionMode(BRepBuilderAPI_RightCorner);//延切线方向缝合
			pipeMaker.Add(dirWire);
			pipeMaker.Build();

			FaceShape_ = pipeMaker.Shape();
			if ( wallPath.Closed() )
			{
				State_ = EDWLS_FINISH;
				return false;
			}

			if ( MeshBuf_ )
			{
				MeshBuf_->drop();
			}

			auto mesh = ODLTools::CreateMesh(FaceShape_);
			assert(mesh);
			MeshBuf_ = mesh->getMeshBuffer(0);
			MeshBuf_->grab();

			auto tex = GetRenderContextSPtr()->Smgr_->getVideoDriver()->getTexture("../Data/Resource/3D/wallLine.png");
			MeshBuf_->getMaterial().setTexture(0, tex);
			float uLen = 200;
			float vLen = 200;
			irr::core::matrix4 scaleMat,rotateMat;
			scaleMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
			rotateMat.setTextureRotationCenter(static_cast<float>(M_PI/4));
			MeshBuf_->getMaterial().setTextureMatrix(0, rotateMat*scaleMat);
			MeshBuf_->getMaterial().Lighting = false;
			MeshBuf_->getMaterial().ZWriteEnable = false;
			MeshBuf_->getMaterial().BackfaceCulling = false;
			mesh->drop();
		}

		{//Line Mesh
			if ( LineMeshBuf_ )
			{
				LineMeshBuf_->drop();
			}

			auto newSmesh = new SMeshBuffer;
			for ( TopExp_Explorer exp(FaceShape_, TopAbs_EDGE); exp.More(); exp.Next() )
			{
				auto& curEdge = TopoDS::Edge(exp.Current());
				auto& v1 = TopExp::FirstVertex(curEdge);
				auto& v2 = TopExp::LastVertex(curEdge);

				auto p1 = BRep_Tool::Pnt(v1);
				auto p2 = BRep_Tool::Pnt(v2);

				S3DVertex sv1(irr::core::vector3df(static_cast<float>(p1.X()), static_cast<float>(p1.Y()), static_cast<float>(p1.Z())), s_PntNormal, s_LineColor, s_PntCoord);
				S3DVertex sv2(irr::core::vector3df(static_cast<float>(p2.X()), static_cast<float>(p2.Y()), static_cast<float>(p2.Z())), s_PntNormal, s_LineColor, s_PntCoord);

				newSmesh->Vertices.push_back(sv1);
				newSmesh->Vertices.push_back(sv2);
				newSmesh->Indices.push_back(newSmesh->getIndexCount());
				newSmesh->Indices.push_back(newSmesh->getIndexCount());
			}

			LineMeshBuf_ = newSmesh;

			LineMeshBuf_->getMaterial().Lighting = false;
			LineMeshBuf_->getMaterial().ZWriteEnable = false;
			LineMeshBuf_->getMaterial().BackfaceCulling = false;
			LineMeshBuf_->getMaterial().Thickness = 3;
			LineMeshBuf_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
			LineMeshBuf_->getMaterial().DiffuseColor = s_LineColor;
		}
	}


	{//Path Mesh
		auto smeshBuf = static_cast<irr::scene::SMeshBuffer*>(PathMeshBuf_);
		smeshBuf->Vertices.clear();
		smeshBuf->Indices.clear();

		auto curCount = PathMeshBuf_->getIndexCount();
		for ( const auto& curPnt : Pnts_ )
		{
			smeshBuf->Vertices.push_back(irr::video::S3DVertex(curPnt, s_PntNormal, s_PathColor, s_PntCoord));
			smeshBuf->Indices.push_back(curCount++);
		}
	}

	NeedUpdateMesh_ = false;

	return true;
}

void DrawingLineWallCtrller::Init()
{
// 	auto tex = GetRenderContextSPtr()->Smgr_->getVideoDriver()->getTexture("../Data/Resource/3D/wallLine.png");
// 	TmpRect_->getMaterial().setTexture(0, tex);
// 	float uLen = 200;
// 	float vLen = 200;
// 	irr::core::matrix4 scaleMat,rotateMat;
// 	scaleMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
// 	rotateMat.setTextureRotationCenter(static_cast<float>(M_PI/4));
// 	TmpRect_->getMaterial().setTextureMatrix(0, rotateMat*scaleMat);
}
