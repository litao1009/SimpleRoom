#include "stdafx.h"

#include "GraphODL.h"
#include "CornerODL.h"
#include "WallODL.h"

#include <boost/graph/property_iter_range.hpp>

GraphODL::GraphODL(const SRenderContextWPtr& rc):CBaseODL(rc)
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

	return true;
}
