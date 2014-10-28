#include "stdafx.h"

#include "GraphController.h"

#include "Graph.h"
#include "Corner.h"
#include "Wall.h"

#include <boost/graph/property_iter_range.hpp>

class	GraphController::Imp
{
public:
	
	Graph	Graph_;
};

GraphController::GraphController():ImpUPtr_(new Imp)
{

}

GraphController::~GraphController()
{

}

GraphController& GraphController::GetInstance()
{
	static GraphController sInstance;
	return sInstance;
}

CornerList GraphController::GetAllCorners()
{
	auto& imp_ = *ImpUPtr_;

	CornerList ret;

	auto itors = boost::get_property_iter_range(imp_.Graph_, CornerTag());

	for ( auto& curProp : boost::make_iterator_range(itors.first, itors.second) )
	{
		ret.push_back(curProp.Corner_);
	}

	return ret;
}

WallList GraphController::GetAllWalls()
{
	auto& imp_ = *ImpUPtr_;

	WallList ret;

	auto itors = boost::get_property_iter_range(imp_.Graph_, WallTag());

	for ( auto& curProp : boost::make_iterator_range(itors.first, itors.second) )
	{
		ret.push_back(curProp.Wall_);
	}

	return ret;
}

WallList GraphController::GetWallsOnCorner( const CornerSPtr& corner )
{
	auto& imp_ = *ImpUPtr_;

	WallList ret;

	auto curV = boost::vertex(corner->GetIndex(), imp_.Graph_);
	
	auto itors = boost::out_edges(curV, imp_.Graph_);
	for ( auto& curE : boost::make_iterator_range(itors.first, itors.second) )
	{
		auto wall = boost::get(WallTag(), imp_.Graph_, curE);
		ret.push_back(wall.Wall_);
	}

	return ret;
}

CornerSPtr GraphController::CreateCorner( const gp_Pnt& position )
{
	auto& imp_ = *ImpUPtr_;

	auto newCorner = std::make_shared<Corner>();

	CornerProperty prop;
	prop.Corner_ = newCorner;
	auto index = boost::add_vertex(prop, imp_.Graph_);
	
	newCorner->SetIndex(index);
	newCorner->SetPosition(position);

	return newCorner;
}

bool GraphController::RemoveCorner( const CornerSPtr& corner )
{
	auto& imp_ = *ImpUPtr_;

	boost::clear_vertex(corner->GetIndex(), imp_.Graph_);
	boost::remove_vertex(corner->GetIndex(), imp_.Graph_);

	return true;
}

CornerSPtr GraphController::CreateCornerBySplitWall( const WallSPtr& toSplit, const gp_Pnt& position )
{
	auto& imp_ = *ImpUPtr_;

	auto& edgeIndex = toSplit->GetIndex();

	auto src = boost::source(edgeIndex, imp_.Graph_);
	auto target = boost::target(edgeIndex, imp_.Graph_);

	boost::remove_edge(edgeIndex, imp_.Graph_);

	auto newCorner = CreateCorner(position);

	auto srcProp = boost::get(CornerTag(), imp_.Graph_, src);
	auto targetPorp = boost::get(CornerTag(), imp_.Graph_, target);

	AddWall(srcProp.Corner_, newCorner);
	AddWall(newCorner, targetPorp.Corner_);

	return newCorner;
}

WallSPtr GraphController::AddWall( const CornerSPtr& corner1, const CornerSPtr& corner2 )
{
	auto& imp_ = *ImpUPtr_;

	auto newWall = std::make_shared<Wall>(corner1, corner2);

	WallProperty prop;
	prop.Wall_ = newWall;

	auto index = boost::add_edge(corner1->GetIndex(), corner2->GetIndex(), prop, imp_.Graph_);

	assert(index.second);

	newWall->SetIndex(index.first);

	newWall->UpdateMesh();

	return newWall;
}

bool GraphController::RemoveWall( const WallSPtr& wall )
{
	auto& imp_ = *ImpUPtr_;

	auto& edgeIndex = wall->GetIndex();

	auto src = boost::source(edgeIndex, imp_.Graph_);
	auto target = boost::target(edgeIndex, imp_.Graph_);

	boost::remove_edge(wall->GetIndex(), imp_.Graph_);

	auto srcOutEdges = boost::out_edges(src, imp_.Graph_);
	if ( srcOutEdges.first == srcOutEdges.second )
	{
		boost::remove_vertex(src, imp_.Graph_);
	}

	auto targetOutEdges = boost::out_edges(target, imp_.Graph_);
	if ( targetOutEdges.first == targetOutEdges.second )
	{
		boost::remove_vertex(target, imp_.Graph_);
	}

	return true;
}
