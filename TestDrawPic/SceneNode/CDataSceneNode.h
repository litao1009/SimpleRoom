#ifndef CDataSceneNode_h__
#define CDataSceneNode_h__
#pragma once

#include "IMeshSceneNode.h"
#include "CDataSceneNodeFwd.h"
#include "ODL/BaseODLFwd.h"
#include "irrEngine/SRenderContextFwd.h"

#include <string>
#include <memory>
#include <functional>

class	CDataSceneNode : public irr::scene::IMeshSceneNode
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	template<typename T>
	static std::shared_ptr<T>	TranslateNodeToSPtr(T* pointer)
	{
		return std::shared_ptr<T>(pointer, std::bind([](T*){}, std::placeholders::_1));
	}

public:

	CDataSceneNode(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr, std::string debugName="");
	~CDataSceneNode();

public://ISceneNode

	virtual void OnRegisterSceneNode();

	virtual bool removeChild(irr::scene::ISceneNode* child);

	virtual void render();

	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	virtual irr::u32 getMaterialCount() const;

	virtual const irr::core::aabbox3df& getBoundingBox() const;

	virtual irr::scene::ISceneNode* clone(irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager);

	virtual void setVisible(bool isVisible);

public://IMeshSceneNode

	virtual void setMesh(irr::scene::IMesh* mesh);

	virtual irr::scene::IMesh* getMesh();

	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh*, irr::s32, bool, irr::f32);

	virtual void setReadOnlyMaterials(bool readonly) {}

	virtual bool isReadOnlyMaterials() const { return false; }

public:

	CBaseODLWPtr GetBaseODL() const { return m_wpBaseODL; }
	void SetBaseODL(CBaseODLWPtr val) { m_wpBaseODL = val; }

	const std::string& GetTypeName() const { return m_TypeName; }
	void SetTypeName(std::string val) { m_TypeName = val; }

	SRenderContextWPtr	GetRenderContext() const { return m_SRenderContextWPtr; }

	void	AddToShadowPass();
	bool	IsInShadowPass() const { return m_InShadowPass; }
	void	RemoveFromShadowPass();

	void	AddToDepthPass();
	bool	IsInDepthPass() const { return m_InDepthPass; }
	void	RemoveFromDepthPass();

	void	SaveID() { m_SaveID = getID(); }

	void	ResetID() { setID(m_SaveID); }

	void	CreateTriSelector();

	void	SetSavePosition(irr::core::vector3df& pos)
	{
		m_SavePosition = pos;
	}

	const irr::core::vector3df& GetSavePosition() const
	{
		return m_SavePosition;
	}


private:

	bool					m_InShadowPass;
	bool					m_InDepthPass;

	irr::scene::IMesh*		m_pMesh;
	irr::scene::IShadowVolumeSceneNode* m_pShadow;

	irr::s32				m_PassCount;
	irr::s32				m_SaveID;

	irr::video::SColor		m_DbgColor;
	irr::core::vector3df	m_SavePosition;

	SRenderContextWPtr		m_SRenderContextWPtr;

	CBaseODLWPtr			m_wpBaseODL;

	irr::core::aabbox3df	m_Box;

	std::string				m_TypeName;
};
#endif // CDataSceneNode_h__
