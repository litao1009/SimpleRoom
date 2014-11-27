#pragma once

#include "BaseODL.h"

class	BeamODL : public BaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	BeamODL(const SRenderContextWPtr& rc);
	~BeamODL();

public://BaseODL

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_BEAM; }

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

	void	SetDefaultTexture();

	BaseODLList	GetAlignList() const;

	void	SetAlignList(const BaseODLList& list);

	void		AddAlign(const BaseODLSPtr& align);

	void	SetVaildPosition(bool val);
};