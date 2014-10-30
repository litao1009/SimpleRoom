#include "stdafx.h"

#include "GraphODL.h"
#include "CornerODL.h"
#include "WallODL.h"
#include "RoomODL.h"

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
			Dirty_ = false;
		}

	public:

		void begin_face()
		{
			auto newRoom = std::make_shared<RoomODL>(GraphODL_->GetRenderContextWPtr());
			Rooms_.push_back(newRoom);
		}

		void end_face()
		{
			if ( !Dirty_ )
			{
				Rooms_.back()->SetWallList(CurWalls_);
				Rooms_.back()->SetCornerList(CurCorners_);
			}
			else
			{
				Rooms_.pop_back();
			}
			
			CurCorners_.clear();
			CurWalls_.clear();
			Dirty_ = false;
		}

		template <typename Vertex> 
		void next_vertex(Vertex v) 
		{ 
			if ( Dirty_ )
			{
				return;
			}

			auto corner = boost::get(CornerTag(), GraphODL_->Graph_, v);
			if ( std::find(CurCorners_.rbegin(), CurCorners_.rend(), corner) == CurCorners_.rend() )
			{
				CurCorners_.push_back(corner);
			}
			else
			{
				Dirty_ = true;
			}
		}

		template <typename Edge>
		void next_edge(Edge e)
		{
			if ( Dirty_ )
			{
				return;
			}

			CurWalls_.push_back(boost::get(WallTag(), GraphODL_->Graph_, e));
		}


		bool			Dirty_;
		WallODLList		CurWalls_;
		CornerODLList	CurCorners_;
		RoomODLList		Rooms_;
		GraphODLSPtr	GraphODL_;
	};

public:

	static void	SearchRoom(GraphODLSPtr graphODL)
	{
		auto edgeCount = 0;
		auto& indexMap = boost::get(boost::edge_index, graphODL->Graph_);
		for( auto itors=boost::edges(graphODL->Graph_); itors.first!=itors.second; ++itors.first,++edgeCount )
		{
			boost::put(indexMap, *itors.first, edgeCount);
		}

		std::vector<std::vector<WallODL::EdgeIndex>> embedding(boost::num_vertices(graphODL->Graph_));

		boost::boyer_myrvold_planarity_test(graphODL->Graph_, embedding.data());

		RoomVisitor vv(graphODL);
		boost::planar_face_traversal(graphODL->Graph_, embedding.data(), vv);
	}

public:


};

GraphODL::GraphODL(const SRenderContextWPtr& rc):CBaseODL(rc),ImpUPtr_(new Imp)
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

CornerODLSPtr GraphODL::CreateCornerBySplitWall( const WallODLSPtr& toSplit, const gp_Pnt& position )
{
	auto& edgeIndex = toSplit->GetIndex();

	auto src = boost::source(edgeIndex, Graph_);
	auto target = boost::target(edgeIndex, Graph_);

	boost::remove_edge(edgeIndex, Graph_);
	RemoveChild(toSplit);

	auto newCorner = CreateCorner(position);

	auto srcProp = boost::get(CornerTag(), Graph_, src);
	auto targetPorp = boost::get(CornerTag(), Graph_, target);

	AddWall(srcProp, newCorner);
	AddWall(newCorner, targetPorp);

	return newCorner;
}

bool GraphODL::RemoveCorner( const CornerODLSPtr& corner )
{
	boost::clear_vertex(corner->GetIndex(), Graph_);
	boost::remove_vertex(corner->GetIndex(), Graph_);
	RemoveChild(corner);

	return true;
}

WallODLSPtr GraphODL::AddWall( const CornerODLSPtr& corner1, const CornerODLSPtr& corner2 )
{
	auto thisSPtr = std::static_pointer_cast<GraphODL>(shared_from_this());
	auto newWall = std::make_shared<WallODL>(thisSPtr, corner1, corner2);
	newWall->CreateEmptyDataSceneNode();
	newWall->Init();
	AddChild(newWall);

	auto index = boost::add_edge(corner1->GetIndex(), corner2->GetIndex(), newWall, Graph_);

	assert(index.second);

	newWall->SetIndex(index.first);

	newWall->UpdateMesh();

	Imp::SearchRoom(std::static_pointer_cast<GraphODL>(shared_from_this()));

	return newWall;
}

bool GraphODL::RemoveWall( const WallODLSPtr& wall )
{
	auto& edgeIndex = wall->GetIndex();

	auto src = boost::source(edgeIndex, Graph_);
	auto target = boost::target(edgeIndex, Graph_);

	boost::remove_edge(wall->GetIndex(), Graph_);

	auto srcOutEdges = boost::out_edges(src, Graph_);
	if ( srcOutEdges.first == srcOutEdges.second )
	{
		boost::remove_vertex(src, Graph_);
	}

	auto targetOutEdges = boost::out_edges(target, Graph_);
	if ( targetOutEdges.first == targetOutEdges.second )
	{
		boost::remove_vertex(target, Graph_);
	}

	RemoveChild(wall);

	Imp::SearchRoom(std::static_pointer_cast<GraphODL>(shared_from_this()));

	return true;
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
