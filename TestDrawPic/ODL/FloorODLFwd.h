#pragma once

#include <memory>
#include <vector>

class	FloorODL;

typedef	std::shared_ptr<FloorODL>	FloorODLSPtr;
typedef	std::weak_ptr<FloorODL>		FloorODLWPtr;

typedef	std::vector<FloorODLSPtr>	FloorODLList;
typedef	std::vector<FloorODLWPtr>	FloorODLWList;