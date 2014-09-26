#include "stdafx.h"

#include "DrawingLineWallCtrller.h"
#include "IrrEngine/SRenderContext.h"

#include "ODL/ODLTools.h"

#include "SMeshBuffer.h"

#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "gp_Pnt.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "TopExp_Explorer.hxx"
#include "TopExp.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"
#include "STLAPI.hxx"

static	irr::core::vector3df s_PntNormal(0,1,0);
static	irr::core::vector2df s_PntCoord(0,0);
static	irr::core::plane3df	 s_PntPlane(0,0,0,0,1,0);
static	irr::video::SColor	 s_PntColor(~0);
static	irr::video::SColor	 s_LineColor(0xFF000000);

DrawingLineWallCtrller::DrawingLineWallCtrller()
{
	MeshBuf_ = nullptr;
	LineMeshBuf_ = nullptr;

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

	Material_.Lighting = false;
	Material_.ZWriteEnable = false;
	Material_.BackfaceCulling = false;
	Material_.Thickness = 3;

	NeedUpdateMesh_ = false;

	WallThickness_ = 200;

	State_ = EDWLS_BEGIN;

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
}

bool DrawingLineWallCtrller::OnPostEvent( const irr::SEvent& event )
{
	if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
	{
		if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
		{
			CurrentPos_.X = event.MouseInput.X;
			CurrentPos_.Y = event.MouseInput.Y;

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

					auto smgr = GetRenderContextSPtr()->Smgr_.get();

					auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CurrentPos_);
					irr::core::vector3df position;
					s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), position);
					
					Pnts_.push_back(position);

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
					auto smgr = GetRenderContextSPtr()->Smgr_.get();

					auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CurrentPos_);

					irr::core::vector3df position;
					s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), position);

					Pnts_.push_back(position);

					NeedUpdateMesh_ = true;

					return true;
				}
			}

			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN )
				{
					State_ = EDWLS_FINISH;

					irr::SEvent evt;
					evt.EventType = irr::EET_USER_EVENT;
					evt.UserEvent.UserData1 = EUT_FINISH_DRAW_LINE_WALL;

					GetParentSPtr()->OnPostEvent(evt);
					return true;
				}
			}

			if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
			{
				if ( event.KeyInput.Key == irr::KEY_ESCAPE )
				{
					if ( !Pnts_.empty() )
					{
						Pnts_.pop_back();

						NeedUpdateMesh_ = true;
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

bool DrawingLineWallCtrller::PreRender3D( const SRenderContext& rc )
{
	using namespace irr;
	using namespace core;
	using namespace scene;
	using namespace video;

	switch (State_)
	{
	case DrawingLineWallCtrller::EDWLS_BEGIN:
		break;
	case DrawingLineWallCtrller::EDWLS_DRAWING:
		{
			auto smgr = GetRenderContextSPtr()->Smgr_.get();

			if ( Pnts_.empty() )
			{
				break;
			}

			auto& lastPnt = Pnts_.back();

			auto smeshBuf = static_cast<SMeshBuffer*>(TmpRect_);
				
			auto StoWline = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CurrentPos_);
			vector3df cursorPos;
			s_PntPlane.getIntersectionWithLine(StoWline.start, StoWline.getVector(), cursorPos);

			vector3df dirVec = cursorPos - lastPnt;
			auto thickDir = dirVec.normalize().crossProduct(s_PntNormal);
			
			{//TmpRect_
				TmpRect_->getPosition(0) = lastPnt;
				TmpRect_->getPosition(1) = cursorPos;
				TmpRect_->getPosition(2) = cursorPos + thickDir * WallThickness_;
				TmpRect_->getPosition(3) = lastPnt + thickDir * WallThickness_;
			}

			if ( NeedUpdateMesh_ && Pnts_.size() > 1 )
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
					pipeMaker.SetTransitionMode(BRepBuilderAPI_RightCorner);	//延切线方向缝合
					pipeMaker.Add(dirWire);
					pipeMaker.Build();

					assert(pipeMaker.IsDone());

					FaceShape_ = pipeMaker.Shape();
					//StlAPI::Write(FaceShape_, "wallShell.stl");
					auto mesh = ODLTools::CreateMesh(FaceShape_);
					assert(mesh);

					if ( MeshBuf_ )
					{
						MeshBuf_->drop();
					}

					MeshBuf_ = mesh->getMeshBuffer(0);
					MeshBuf_->grab();
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
				}

				NeedUpdateMesh_ = false;
			}
		}
		break;
	case DrawingLineWallCtrller::EDWLS_FINISH:
		break;
	case DrawingLineWallCtrller::EDWLS_COUNT:
		break;
	default:
		break;
	}

	return true;
}

void DrawingLineWallCtrller::PostRender3D( const SRenderContext& rc )
{
	using namespace irr;
	using namespace core;
	using namespace scene;
	using namespace video;

	auto tmpRectSmesh = static_cast<irr::scene::SMeshBuffer*>(TmpRect_);

	switch (State_)
	{
	case DrawingLineWallCtrller::EDWLS_BEGIN:
		{

		}
		break;
	case DrawingLineWallCtrller::EDWLS_DRAWING:
		{
			auto driver = rc.Smgr_->getVideoDriver();

			driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
			driver->setMaterial(Material_);

			if ( MeshBuf_ )
			{
				driver->drawMeshBuffer(MeshBuf_);
			}

			if ( LineMeshBuf_ )
			{
				driver->drawVertexPrimitiveList(LineMeshBuf_->getVertices(), LineMeshBuf_->getVertexCount(), LineMeshBuf_->getIndices(), LineMeshBuf_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			for ( unsigned index=0; index<TmpRect_->getVertexCount(); ++index )
			{
				tmpRectSmesh->Vertices[index].Color.set(~0);
			}
			driver->drawVertexPrimitiveList(TmpRect_->getVertices(), TmpRect_->getVertexCount(), TmpRect_->getIndices(), TmpRect_->getIndexCount()-2, EVT_STANDARD, EPT_TRIANGLE_FAN);

			for ( unsigned index=0; index<TmpRect_->getVertexCount(); ++index )
			{
				tmpRectSmesh->Vertices[index].Color.set(0xFF000000);
			}
			driver->drawVertexPrimitiveList(TmpRect_->getVertices(), TmpRect_->getVertexCount(), TmpRect_->getIndices(), TmpRect_->getIndexCount(), EVT_STANDARD, EPT_LINE_LOOP);
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
	
	Pnts_.clear();

	State_ = EDWLS_BEGIN;
}

irr::scene::IMeshBuffer* DrawingLineWallCtrller::GetMeshBuffer() const
{
	return MeshBuf_;
}
