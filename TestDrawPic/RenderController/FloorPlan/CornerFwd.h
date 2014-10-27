#ifndef CornerFwd_h__
#define CornerFwd_h__

#pragma once

#include <memory>
#include <vector>

class	Corner;

typedef	std::shared_ptr<Corner>	CornerSPtr;
typedef	std::weak_ptr<Corner>	CornerWPtr;

typedef	std::vector<CornerSPtr>	CornerList;
typedef	std::vector<CornerWPtr>	CornerWList;

#endif // CornerFwd_h__