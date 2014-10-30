#pragma once

#include <memory>
#include <vector>

class	RoomODL;

typedef	std::shared_ptr<RoomODL>	RoomODLSPtr;
typedef	std::weak_ptr<RoomODL>		RoomODLWPtr;

typedef	std::vector<RoomODLSPtr>	RoomODLList;
typedef	std::vector<RoomODLWPtr>	RoomODLWList;