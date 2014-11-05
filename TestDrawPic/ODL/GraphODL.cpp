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

		bool	operator < ( const SortedRoom& rhs) const
		{
			if ( Corners_.size() < rhs.Corners_.size() )
			{
				return true;
			}

			if ( Corners_.size() > rhs.Corners_.size() )
			{
				return false;
			}

			for ( auto itor1=Corners_.begin(),itor2=rhs.Corners_.begin(); itor1!=Corners_.end(); ++itor1,++itor2 )
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

		void	CalculateOrder()
		{
			auto count = 0;

			for ( auto itor=Corners_.begin(); itor!=Corners_.end(); ++itor )
			{
				gp_Pnt cur,pre,next;

				cur = (*itor)->GetPosition();

				if ( itor == Corners_.begin() )
				{
					pre = Corners_.back()->GetPosition();
					next = (*(itor+1))->GetPosition();
				}
				else if ( itor+1 == Corners_.end() )
				{
					pre = (*(itor-1))->GetPosition();
					next = Corners_.front()->GetPosition();
				}
				else
				{
					pre = (*(itor-1))->GetPosition();
					next = (*(itor+1))->GetPosition();
				}

				gp_Vec v1(pre,cur);
				gp_Vec v2(cur,next);

				auto ret = v1.X() * v2.Z() - v1.Z() * v2.X();

				if ( ret > 0 )
				{
					++count;
				}
				else if ( ret < 0 )
				{
					--count;
				}
			}

			Order_ = count < 0;
		}

		CornerODLList	Corners_;
		RoomODLSPtr		Room_;
		bool			Order_;
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

		std::vector<SortedRoom>	originalRooms,newRooms;
		std::vector<SortedRoom> intersection,toRemove,toAdd;

		for ( auto& curRoom : graphODL->ImpUPtr_->Rooms_ )
		{
			SortedRoom item;
			item.Corners_ = curRoom->GetCornerList();
			item.Room_ = curRoom;
			std::sort(item.Corners_.begin(), item.Corners_.end());
			originalRooms.push_back(item);
		}
		std::sort(originalRooms.begin(), originalRooms.end());

		if ( !vv.FoundRooms_.empty() )
		{
			auto posCount = 0, negCount = 0;

			for ( auto& curRoom : vv.FoundRooms_ )
			{
				SortedRoom item;
				item.Corners_ = curRoom->GetCornerList();
				item.Room_ = curRoom;
				item.CalculateOrder();
				item.Order_ ? ++posCount : ++negCount;

				std::sort(item.Corners_.begin(), item.Corners_.end());

				auto itor = std::adjacent_find(item.Corners_.begin(), item.Corners_.end());
				if ( itor == item.Corners_.end() )
				{
					newRooms.push_back(item);
				}
			}

			assert( !(posCount != 1 && negCount != 1) );

			if ( posCount == 1 && negCount != 1 )
			{
				auto itor = std::remove_if(newRooms.begin(), newRooms.end(), [](const SortedRoom& item)
				{
					return true == item.Order_;
				});

				newRooms.erase(itor, newRooms.end());
			}
			else if ( posCount != 1 && negCount == 1 )
			{
				auto itor = std::remove_if(newRooms.begin(), newRooms.end(), [](const SortedRoom& item)
				{
					return false == item.Order_;
				});

				newRooms.erase(itor, newRooms.end());
			}
			else
			{
				auto itorPos = std::find_if(newRooms.begin(), newRooms.end(), [](const SortedRoom& item)
				{
					return true == item.Order_;
				});

				auto itorNeg = std::find_if(newRooms.begin(), newRooms.end(), [](const SortedRoom& item)
				{
					return false == item.Order_;
				});

				if ( itorPos != newRooms.end() && itorNeg != newRooms.end() )
				{
					newRooms.erase(itorPos);
				}

			}

			std::sort(newRooms.begin(), newRooms.end());
		}

		std::set_intersection(originalRooms.begin(), originalRooms.end(), newRooms.begin(), newRooms.end(), std::back_inserter(intersection));
		std::set_difference(originalRooms.begin(), originalRooms.end(), intersection.begin(), intersection.end(), std::back_inserter(toRemove));
		std::set_difference(newRooms.begin(), newRooms.end(), intersection.begin(), intersection.end(), std::back_inserter(toAdd));

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

public:

	RoomODLList		Rooms_;
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
