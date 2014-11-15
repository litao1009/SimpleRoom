#ifndef WallBLL_h__
#define WallBLL_h__

#pragma once

#include "Basebll.h"

class WallBLL : public BaseBLL
{
public:
	WallBLL(void)
	{
		Thickness_ = 200.f;
		Height_ = 2400.f;
	}
	~WallBLL(void);

public:

	float	Thickness_;
	float	Height_;
};

#endif // WallBLL_h__