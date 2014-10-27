#ifndef WallFwd_h__
#define WallFwd_h__

#pragma once

#include <memory>
#include <vector>
#include <set>

class	Wall;

typedef	std::shared_ptr<Wall>	WallSPtr;
typedef	std::weak_ptr<Wall>		WallWPtr;

typedef	std::vector<WallSPtr>	WallList;
typedef	std::vector<WallWPtr>	WallWList;

#endif // WallFwd_h__