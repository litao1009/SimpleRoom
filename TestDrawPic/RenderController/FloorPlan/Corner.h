#ifndef Corner_h__
#define Corner_h__

#pragma once

#include "CornerFwd.h"
#include "Graph.h"

#include "gp_Pnt.hxx"

class	Corner : public std::enable_shared_from_this<Corner>
{
public:

	Corner();

public:

	void	Init( Graph& theGraph);

	const gp_Pnt&	GetPosition() const { return Position_; }

private:

	std::size_t	GraphIndex_;
	gp_Pnt		Position_;
};

#endif // Corner_h__