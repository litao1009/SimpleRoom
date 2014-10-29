#pragma once

#include "CornerODLFwd.h"
#include "WallODLFwd.h"

#include <memory>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

class	GraphODL;

typedef	std::shared_ptr<GraphODL>	GraphODLSPtr;
typedef	std::weak_ptr<GraphODL>		GraphODLWPtr;

typedef	std::vector<GraphODLSPtr>	GraphODLList;
typedef	std::vector<GraphODLWPtr>	GraphODLWList;



class	CornerTag
{
public:
	typedef boost::vertex_property_tag	kind;
};
typedef	boost::property<boost::vertex_index_t, std::size_t>			VertexIndexProp;
typedef	boost::property<CornerTag, CornerODLSPtr, VertexIndexProp>	VertexProp;


class	WallTag
{
public:
	typedef	boost::edge_property_tag kind;
};
typedef	boost::property<boost::edge_index_t, std::size_t>		EdgeIndexProp;
typedef	boost::property<WallTag, WallODLSPtr, EdgeIndexProp>	EdgeProp;


typedef	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProp, EdgeProp>	Graph;