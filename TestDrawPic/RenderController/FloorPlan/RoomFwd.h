#ifndef RoomFwd_h__
#define RoomFwd_h__

#pragma once

#include <memory>
#include <vector>

class	Room;

typedef	std::shared_ptr<Room>	RoomSPtr;
typedef	std::weak_ptr<Room>		RoomWPtr;

typedef	std::vector<RoomSPtr>	RoomList;
typedef	std::vector<RoomWPtr>	RoomWList;

#endif // RoomFwd_h__