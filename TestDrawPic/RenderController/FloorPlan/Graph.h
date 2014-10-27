#ifndef Graph_h__
#define Graph_h__

#pragma once

#include "CornerFwd.h"
#include "WallFwd.h"
#include "RoomFwd.h"

#include <boost/graph/adjacency_list.hpp>

class	CornerTag
{
public:
	typedef boost::vertex_property_tag	kind;
};

class	CornerProperty
{
public:
	CornerSPtr	Corner_;
};

typedef	boost::property<boost::vertex_index_t, std::size_t>	VertexIndexProp;
typedef	boost::property<CornerTag, CornerProperty, VertexIndexProp>	VertexProp;




class	WallTag
{
public:
	typedef	boost::edge_property_tag kind;
};

class	WallProperty
{
public:
	WallSPtr	Wall_;
};

typedef	boost::property<boost::edge_index_t, std::size_t>	EdgeIndexProp;
typedef	boost::property<WallTag, WallProperty, EdgeIndexProp>	EdgeProp;


typedef	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProp, EdgeProp>	Graph;



#endif // Graph_h__