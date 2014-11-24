#pragma once

#include "RoomODLFwd.h"
#include "WallODLFwd.h"
#include "CornerODLFwd.h"

#include "BaseODL.h"

#include <string>

class	RoomODL : public BaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;
public:

	RoomODL(const SRenderContextWPtr& rc);
	~RoomODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_ROOM; }

	virtual	void	Init() {}

protected:

	virtual	void	UpdateSweeping();

	virtual	void	UpdatePicking();

public:

	void	SetWallList(const WallODLList& list);

	void	SetCornerList(const CornerODLList& list);

	WallODLList	GetWallList() const;

	CornerODLList	GetCornerList() const;

	bool			Build();

	double			GetArea() const;

	const gp_Pnt&	GetCenter() const;

	void			SetName(const std::wstring& name);

	const std::wstring&	GetName() const;

private:

	WallODLWList	WallList_;
	CornerODLWList	CornerList_;
};