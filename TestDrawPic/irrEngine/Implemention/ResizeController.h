#ifndef ResizeController_h__
#define ResizeController_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	ResizeController : public IRenderController
{

public:

	ResizeController(HWND hwnd);

public:

	virtual bool	OnEvent(const irr::SEvent& event);

private:

	HWND	Hwnd_;
};

#endif // ResizeController_h__
