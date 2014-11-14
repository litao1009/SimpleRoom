#ifndef TestDecorGUIBoard_h__
#define TestDecorGUIBoard_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	TestDecorGUIBoard : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	TestDecorGUIBoard(const SRenderContextWPtr& rc);
	~TestDecorGUIBoard();

public:

	virtual	void	Init();

	virtual	bool	OnGUIEvent(const irr::SEvent& evt);

	virtual bool	PreRender2D();

	virtual void	PostRender2D();

};

#endif // TestDecorGUIBoard_h__
