#ifndef StatusMgr_h__
#define StatusMgr_h__

#pragma once

#include "irrEngine/IrrExtension/CombineSceneNode.h"

#include <vector>
#include <boost/optional.hpp>

class	StatusMgr
{
	StatusMgr()
	{
		DrawingState_ = EDS_NONE;
	}

	~StatusMgr() {}

public:

	enum EDrawingState
	{
		EDS_NONE,
		EDS_LINE_WALL,
		EDS_RECT_WALL,
		EDS_COUNT
	};

public:

	static	StatusMgr&	GetInstance()
	{
		static StatusMgr s_Instance;
		return s_Instance;
	}

public:

	boost::optional<irr::core::vector3df>	GridAlign_;

	EDrawingState							DrawingState_;
};
#endif // StatusMgr_h__
