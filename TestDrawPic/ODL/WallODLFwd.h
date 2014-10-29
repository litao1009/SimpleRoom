#pragma once

#include <memory>
#include <vector>

class	WallODL;

typedef	std::shared_ptr<WallODL>	WallODLSPtr;
typedef	std::weak_ptr<WallODL>		WallODLWPtr;

typedef	std::vector<WallODLSPtr>	WallODLList;
typedef	std::vector<WallODLWPtr>	WallODLWList;