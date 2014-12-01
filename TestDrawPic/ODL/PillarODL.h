#pragma once

#include "BaseODL.h"

class	PillarODL : public BaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	PillarODL(const SRenderContextWPtr& rc);
	~PillarODL();

public://BaseODL

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_PILLAR; }

	virtual	void	Init();

protected:

	virtual	void	UpdatePicking();

	virtual	void	UpdateSweeping();

public:

	float	GetOffsetHeight() const;

	void	SetOffsetHeight(float val);

	void	SetSize(float xLength, float yLength, float zLength);

	void	SetSize(const gp_XYZ& size);

	const gp_XYZ&	GetSize() const;

	void	UpdateMesh();

	void	Update2DMesh();

	void	SetDefaultTexture();

	BaseODLList	GetAlignList() const;

	void	SetAlignList(const BaseODLList& list);

	void		AddAlign(const BaseODLSPtr& align);

	void	SetVaildPosition(bool val);
};