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
		RealWorld_ = false;
		CreateWallHeight_ = 2600;

		DrawingState_ = EDS_NONE;
		PuttingState_ = EPS_NONE;
	}

	~StatusMgr() {}

public:

	enum EDrawingState
	{
		EDS_NONE,
		EDS_LINE_WALL,
		EDS_RECT_WALL,
		EDS_BAY_WINDOW,
		EDS_COUNT
	};

	enum EPuttingState
	{
		EPS_NONE,
		EPS_DOOR,
		EPS_WINDOW,
		EPS_DECOR,
		EPS_COUNT
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

	float									CreateWallHeight_;

	bool									RealWorld_;

	//--for test
	EPuttingState							PuttingState_;
	//--for test
};
#endif // StatusMgr_h__
