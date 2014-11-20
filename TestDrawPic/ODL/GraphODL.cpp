#include "stdafx.h"

#include "GraphODL.h"
#include "CornerODL.h"
#include "WallODL.h"
#include "RoomODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepFeat.hxx"
#include "BRepAdaptor_Curve.hxx"

#include <boost/graph/property_iter_range.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

class	GraphODL::Imp
{
public:

	class RoomVisitor : public boost::planar_face_traversal_visitor
	{
	public:

		RoomVisitor(const GraphODLSPtr& graphODL)
		{
			GraphODL_ = graphODL;
		}

	public:

		void begin_face()
		{
			auto newRoom = std::make_shared<RoomODL>(GraphODL_->GetRenderContextWPtr());
			newRoom->CreateEmptyDataSceneNode();
			FoundRooms_.push_back(newRoom);
		}

		void end_face()
		{
			if ( CurWalls_.empty() || CurCorners_.empty() )
			{
				FoundRooms_.pop_back();
				return;
			}

			if ( CurCorners_.size() < 3 )
			{
				FoundRooms_.pop_back();
				return;
			}

			FoundRooms_.back()->SetWallList(CurWalls_);
			FoundRooms_.back()->SetCornerList(CurCorners_);

			CurCorners_.clear();
			CurWalls_.clear();
		}

		template <typename Vertex> 
		void next_vertex(Vertex v) 
		{ 
			auto corner = boost::get(CornerTag(), GraphODL_->Graph_, v);
			auto wallsOnCorner = GraphODL_->GetWallsOnCorner(corner);
			if ( 1 == wallsOnCorner.size() )
			{
				return;
			}

			if ( !CurCorners_.empty() && CurCorners_.back() == corner )
			{
				CurCorners_.pop_back();
			}
			else
			{

				CurCorners_.push_back(corner);
			}
		}

		template <typename Edge>
		void next_edge(Edge e)
		{
			auto curWall = boost::get(WallTag(), GraphODL_->Graph_, e);

			if ( !CurWalls_.empty() && CurWalls_.back() == curWall )
			{
				CurWalls_.pop_back();
			}
			else
			{
				CurWalls_.push_back(curWall);
			}
		}

		WallODLList		CurWalls_;
		CornerODLList	CurCorners_;
		RoomODLList		FoundRooms_;
		GraphODLSPtr	GraphODL_;
	};

	class	SortedRoom
	{
	public:

		SortedRoom()
		{
			ToRemove_ = false;
		}

	public:

		bool	operator < ( const SortedRoom& rhs) const
		{
			if ( SortedCorners_.size() < rhs.SortedCorners_.size() )
			{
				return true;
			}

			if ( SortedCorners_.size() > rhs.SortedCorners_.size() )
			{
				return false;
			}

			for ( auto itor1=SortedCorners_.begin(),itor2=rhs.SortedCorners_.begin(); itor1!=SortedCorners_.end(); ++itor1,++itor2 )
			{
				if ( *itor1 < *itor2 )
				{
					return true;
				}

				if ( *itor1 > *itor2 )
				{
					return false;
				}
			}

			return false;
		}

		CornerODLList	SortedCorners_;
		RoomODLSPtr		Room_;
		TopoDS_Face		Face_;
		bool			ToRemove_;
	};

public:

	static void	SearchRoom(GraphODLSPtr graphODL)
	{
// 		auto edgeCount = 0;
// 		auto& indexMap = boost::get(boost::edge_index, graphODL->Graph_);
// 		for( auto itors=boost::edges(graphODL->Graph_); itors.first!=itors.second; ++itors.first,++edgeCount )
// 		{
// 			boost::put(indexMap, *itors.first, edgeCount);
// 		}

		std::vector<std::vector<WallODL::EdgeIndex>> embedding(boost::num_vertices(graphODL->Graph_));

		boost::boyer_myrvold_planarity_test(graphODL->Graph_, embedding.data());

		RoomVisitor vv(graphODL);
		boost::planar_face_traversal(graphODL->Graph_, embedding.data(), vv);

		std::vector<SortedRoom>	originalRooms,newRooms;
		std::vector<SortedRoom> intersection,toRemove,toAdd;

		for ( auto& curRoom : graphODL->ImpUPtr_->Rooms_ )
		{
			SortedRoom item;
			item.SortedCorners_ = curRoom->GetCornerList();
			item.Room_ = curRoom;
			std::sort(item.SortedCorners_.begin(), item.SortedCorners_.end());
			originalRooms.push_back(item);
		}
		std::sort(originalRooms.begin(), originalRooms.end());

		if ( !vv.FoundRooms_.empty() )
		{
			for ( auto& curRoom : vv.FoundRooms_ )
			{
				SortedRoom item;
				item.SortedCorners_ = curRoom->GetCornerList();
				item.Room_ = curRoom;

				std::sort(item.SortedCorners_.begin(), item.SortedCorners_.end());

				//去掉有交叉的
				auto itor = std::adjacent_find(item.SortedCorners_.begin(), item.SortedCorners_.end());
				if ( itor == item.SortedCorners_.end() )
				{
					newRooms.push_back(item);
				}
			}

			std::sort(newRooms.begin(), newRooms.end());

			{//去掉重复的
				auto itor = std::unique(newRooms.begin(), newRooms.end(), [](const SortedRoom& lhs, const SortedRoom& rhs)
				{
					return !(lhs < rhs) && !(rhs < lhs);
				});
				newRooms.erase(itor, newRooms.end());
			}

			{//去掉最大的面
				for ( auto& curRoom : newRooms )
				{
					BRepBuilderAPI_MakePolygon mp;
					for ( auto& curCorner : curRoom.Room_->GetCornerList() )
					{
						mp.Add(curCorner->GetPosition());
					}
					mp.Close();

					curRoom.Face_ = BRepBuilderAPI_MakeFace(mp.Wire()).Face();
				}

				for ( auto itorBase=newRooms.begin(); itorBase!=newRooms.end(); ++itorBase )
				{
					for ( auto itorCompare=newRooms.begin(); itorCompare!=newRooms.end(); ++itorCompare )
					{
						if ( itorCompare == itorBase )
						{
							continue;
						}

						if ( itorCompare->ToRemove_ )
						{
							continue;
						}

						auto ret = BRepFeat::IsInside(itorCompare->Face_, itorBase->Face_);
						if ( Standard_True == ret )
						{
							itorBase->ToRemove_ = true;
						}
						break;
					}
				}
			}

			auto itor = std::remove_if(newRooms.begin(), newRooms.end(), [](const SortedRoom& val)
			{
				return val.ToRemove_;
			});
			newRooms.erase(itor, newRooms.end());
		}

		std::set_intersection(originalRooms.begin(), originalRooms.end(), newRooms.begin(), newRooms.end(), std::back_inserter(intersection));
		std::set_difference(originalRooms.begin(), originalRooms.end(), intersection.begin(), intersection.end(), std::back_inserter(toRemove));
		std::set_difference(newRooms.begin(), newRooms.end(), intersection.begin(), intersection.end(), std::back_inserter(toAdd));

#ifdef _DEBUG
		TRACE("\nRoomTestBegin:\n");
		if ( !intersection.empty() )
		{
			TRACE("-------------Intersection:\n");
			for ( auto& curRoom : intersection )
			{
				TRACE("\tRoom:\t");
				for ( auto& curCorner : curRoom.Room_->GetCornerList() )
				{
					TRACE((std::to_string(curCorner->GetIndex())+" ").c_str());
				}
				TRACE("\n");
			}
			TRACE("\n--------------------------:\n");
		}
		
		if ( !toAdd.empty() )
		{
			TRACE("-------------Add:\n");
			for ( auto& curRoom : toAdd )
			{
				TRACE("\tRoom:\t");
				for ( auto& curCorner : curRoom.Room_->GetCornerList() )
				{
					TRACE((std::to_string(curCorner->GetIndex())+" ").c_str());
				}
				TRACE("\n");
			}
			TRACE("\n--------------------------:\n");
		}

		if ( !toRemove.empty() )
		{
			TRACE("-------------toRemove:\n");
			for ( auto& curRoom : toRemove )
			{
				TRACE("\tRoom:\t");
				for ( auto& curCorner : curRoom.Room_->GetCornerList() )
				{
					TRACE((std::to_string(curCorner->GetIndex())+" ").c_str());
				}
				TRACE("\n");
			}
			TRACE("\n--------------------------:\n");
		}
		TRACE("RoomTestEnd:\n\n");
#endif

		for ( auto& curRoom : toRemove )
		{
			curRoom.Room_->RemoveFromParent();
			auto itor = std::find(graphODL->ImpUPtr_->Rooms_.begin(), graphODL->ImpUPtr_->Rooms_.end(), curRoom.Room_);
			graphODL->ImpUPtr_->Rooms_.erase(itor);
		}

		for ( auto& curRoom : toAdd )
		{
			if ( !curRoom.Room_->Build() )
			{
				continue;
			}

			graphODL->AddChild(curRoom.Room_);
			graphODL->ImpUPtr_->Rooms_.push_back(curRoom.Room_);
		}
	}

	static void	UpdateVertexIndex(Graph& graph)
	{
		for ( auto itors=boost::vertices(graph); itors.first!=itors.second; ++itors.first )
		{
			auto curCorner = boost::get(CornerTag(), graph, *itors.first);
			curCorner->SetIndex(*itors.first);
		}
	}

	static void	UpdateEdgeIndex(Graph& graph)
	{
		auto edgeIndex = 0;
		for ( auto itors=boost::edges(graph); itors.first!=itors.second; ++itors.first, ++edgeIndex )
		{
			boost::put(boost::edge_index, graph, *itors.first, edgeIndex);

			auto curWall = boost::get(WallTag(), graph, *itors.first);

			auto src = boost::source(*itors.first, graph);
			auto target = boost::target(*itors.first, graph);

			curWall->SetIndex(*itors.first);
		}
	}

	static WallODLSPtr AddWall( const GraphODLSPtr& graph, const CornerODLSPtr& corner1, const CornerODLSPtr& corner2, bool researchRomm, bool updateMesh )
	{
		auto newWall = std::make_shared<WallODL>(graph, corner1, corner2);
		newWall->CreateEmptyDataSceneNode();
		newWall->Init();
		graph->AddChild(newWall);

		auto index = boost::add_edge(corner1->GetIndex(), corner2->GetIndex(), newWall, graph->Graph_);

		assert(index.second);

		//boost::put(boost::edge_index, Graph_, index.first, index);
		//newWall->SetIndex(index.first);
		UpdateEdgeIndex(graph->Graph_);

		if ( updateMesh )
		{
			newWall->UpdateBaseMesh();
		}
		else
		{
			newWall->SetDirty(true);
		}
		

		if ( researchRomm )
		{
			Imp::SearchRoom(graph);
		}

		return newWall;
	}

	static bool RemoveWall( const GraphODLSPtr& graphODL, const WallODLSPtr& wall, bool needMerge, bool researchRomm, bool updateMesh )
	{
		auto& edgeIndex = wall->GetIndex();
		auto& graph_ = graphODL->Graph_;

		auto src = boost::source(edgeIndex, graph_);
		auto target = boost::target(edgeIndex, graph_);
		auto srcCorner = boost::get(CornerTag(), graph_, src);
		auto targetCorner = boost::get(CornerTag(), graph_, target);

		graphODL->RemoveChild(wall);
		boost::remove_edge(wall->GetIndex(), graph_);

		auto needMergeSrc = false, needMergeTarget = false;
		auto needDelSrc = false, needDelTarget = false;

		auto srcOutEdges = boost::out_edges(src, graph_);
		if ( srcOutEdges.first == srcOutEdges.second )
		{
			needDelSrc = true;
		}
		else
		{
			auto corner = boost::get(CornerTag(), graph_, src);
			needMergeSrc = true;
			for ( auto& curWall : graphODL->GetWallsOnCorner(corner) )
			{
				if ( updateMesh )
				{
					curWall->UpdateBaseMesh();
				}
				else
				{
					curWall->SetDirty(true);
				}
			}
		}

		auto targetOutEdges = boost::out_edges(target, graph_);
		if ( targetOutEdges.first == targetOutEdges.second )
		{
			needDelTarget = true;
		}
		else
		{
			auto corner = boost::get(CornerTag(), graph_, target);
			needMergeTarget = true;
			for ( auto& curWall : graphODL->GetWallsOnCorner(corner) )
			{
				if ( updateMesh )
				{
					curWall->UpdateBaseMesh();
				}
				else
				{
					curWall->SetDirty(true);
				}
			}
		}

		Imp::UpdateEdgeIndex(graph_);

		if ( needDelSrc )
		{
			graphODL->RemoveCorner(srcCorner);
		}
		else if ( needMergeSrc && needMerge )
		{
			graphODL->MergeWallIfNeeded(srcCorner, researchRomm, updateMesh);
		}

		if ( needDelTarget )
		{
			graphODL->RemoveCorner(targetCorner);
		}
		else if ( needMergeTarget && needMerge )
		{
			graphODL->MergeWallIfNeeded(targetCorner, researchRomm, updateMesh);
		}

		if ( researchRomm )
		{
			Imp::SearchRoom(graphODL);
		}

		return true;
	}

public:

	RoomODLList		Rooms_;
};

GraphODL::GraphODL(const SRenderContextWPtr& rc):BaseODL(rc),ImpUPtr_(new Imp)
{

}

GraphODL::~GraphODL()
{

}

CornerODLList GraphODL::GetAllCorners()
{
	CornerODLList ret;

	auto itors = boost::get_property_iter_range(Graph_, CornerTag());
	
	for ( auto& curProp : boost::make_iterator_range(itors.first, itors.second) )
	{
		ret.push_back(curProp);
	}

	return ret;
}

WallODLList GraphODL::GetAllWalls()
{
	WallODLList ret;

	auto itors = boost::get_property_iter_range(Graph_, WallTag());

	for ( auto& curProp : boost::make_iterator_range(itors.first, itors.second) )
	{
		ret.push_back(curProp);
	}

	return ret;
}

WallODLList GraphODL::GetWallsOnCorner( const CornerODLSPtr& corner )
{
	WallODLList ret;

	auto curV = boost::vertex(corner->GetIndex(), Graph_);

	auto itors = boost::out_edges(curV, Graph_);
	for ( auto& curE : boost::make_iterator_range(itors.first, itors.second) )
	{
		auto wall = boost::get(WallTag(), Graph_, curE);
		ret.push_back(wall);
	}

	return ret;
}

CornerODLSPtr GraphODL::CreateCorner( const gp_Pnt& position )
{
	auto newCorner = CreateChild<CornerODL>(GetRenderContextWPtr());

	auto index = boost::add_vertex(newCorner, Graph_);

	newCorner->SetIndex(index);
	newCorner->SetPosition(position);

	return newCorner;
}

CornerODLSPtr GraphODL::CreateCornerBySplitWall( const WallODLSPtr& toSplit, const gp_Pnt& position, bool researchRoom, bool updateMesh )
{
	auto& edgeIndex = toSplit->GetIndex();

	auto src = boost::source(edgeIndex, Graph_);
	auto target = boost::target(edgeIndex, Graph_);

	boost::remove_edge(edgeIndex, Graph_);
	RemoveChild(toSplit);

	auto newCorner = CreateCorner(position);

	auto srcProp = boost::get(CornerTag(), Graph_, src);
	auto targetPorp = boost::get(CornerTag(), Graph_, target);

	auto thisSPtr = std::static_pointer_cast<GraphODL>(shared_from_this());

	Imp::AddWall(thisSPtr, srcProp, newCorner, false, updateMesh);
	Imp::AddWall(thisSPtr, newCorner, targetPorp, researchRoom, updateMesh);

	return newCorner;
}

bool GraphODL::RemoveCorner( const CornerODLSPtr& corner )
{
	boost::clear_vertex(corner->GetIndex(), Graph_);
	boost::remove_vertex(corner->GetIndex(), Graph_);
	RemoveChild(corner);

	Imp::UpdateVertexIndex(Graph_);
	Imp::UpdateEdgeIndex(Graph_);

	return true;
}

WallODLSPtr GraphODL::AddWall( const CornerODLSPtr& corner1, const CornerODLSPtr& corner2, bool researchRoom, bool updateMesh )
{
	auto thisSPtr = std::static_pointer_cast<GraphODL>(shared_from_this());
	return Imp::AddWall(thisSPtr, corner1, corner2, researchRoom, updateMesh);
}

bool GraphODL::RemoveWall( const WallODLSPtr& wall, bool needMerge, bool researchRoom, bool updateMesh )
{
	auto thisSPtr = std::static_pointer_cast<GraphODL>(shared_from_this());

	return Imp::RemoveWall(thisSPtr, wall, needMerge, researchRoom, updateMesh);
}

RoomODLList GraphODL::GetAllRooms()
{
	RoomODLList ret;

	for ( auto& curChild : GetChildrenList() )
	{
		if ( EODLT_ROOM != curChild->GetType() )
		{
			continue;
		}

		ret.push_back(std::static_pointer_cast<RoomODL>(curChild));
	}

	return ret;
}

void GraphODL::MergeWallIfNeeded( const CornerODLSPtr& corner, bool researchRoom, bool updateMesh )
{
	auto walls = GetWallsOnCorner(corner);

	if ( 2 !=walls.size() )
	{
		return;
	}

	auto& wall1 = walls[0];
	auto& wall2 = walls[1];

	gp_Dir edge1Dir = gp_Vec(corner->GetPosition(), wall1->GetOtherCorner(corner).lock()->GetPosition());
	gp_Dir edge2Dir = gp_Vec(corner->GetPosition(), wall2->GetOtherCorner(corner).lock()->GetPosition());

	if ( Standard_False == edge1Dir.IsParallel(edge2Dir, Precision::Angular()) )
	{
		return;
	}

	auto corner1 = wall1->GetOtherCorner(corner);
	auto corner2 = wall2->GetOtherCorner(corner);

	auto thisSPtr = std::static_pointer_cast<GraphODL>(shared_from_this());

	Imp::AddWall(thisSPtr, corner1.lock(),corner2.lock(), false, updateMesh);

	Imp::RemoveWall(thisSPtr, wall1, false, false, updateMesh);
	Imp::RemoveWall(thisSPtr, wall2, false, researchRoom, updateMesh);
}

void GraphODL::SearchRooms()
{
	auto thisSPtr = std::static_pointer_cast<GraphODL>(shared_from_this());

	Imp::SearchRoom(thisSPtr);
}

void GraphODL::UpdateWallMeshIfNeeded()
{
	for ( auto& curWall : GetAllWalls() )
	{
		if ( curWall->IsDirty() )
		{
			curWall->Update2DMesh();
			curWall->SetDirty(false);
		}
	}
}
