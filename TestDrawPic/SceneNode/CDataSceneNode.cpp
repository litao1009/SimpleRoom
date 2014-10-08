#include "stdafx.h"

#include <assert.h>

#include "CDataSceneNode.h"

#include "irrlicht.h"
#include "Irrlicht/CShadowVolumeSceneNode.h"
#include "irrEngine/SRenderContext.h"

using namespace irr;
using namespace scene;
using namespace video;

class	CDataSceneNode::Imp
{
public:

	Imp() {}
	~Imp() {}

};

CDataSceneNode::CDataSceneNode(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr, std::string debugName):
	irr::scene::IMeshSceneNode(0, rcWPtr.lock()->Smgr_.get(), 0xFF),m_TypeName(debugName)
{
	SetBaseODL(odlWPtr);

	m_InShadowPass = false;
	m_InDepthPass = false;
	m_SRenderContextWPtr = rcWPtr;
	m_pShadow = nullptr;
	m_pMesh = nullptr;

	m_DbgColor.setAlpha(0xff);
	m_DbgColor.setBlue(rand()&0xff);
	m_DbgColor.setGreen(rand()&0xff);
	m_DbgColor.setRed(rand()&0xff);
}

CDataSceneNode::~CDataSceneNode()
{
	if (m_pShadow)
	{
		m_pShadow->drop();
	}

	if (m_pMesh)
	{
		m_pMesh->drop();
	}

	RemoveFromDepthPass();
	RemoveFromShadowPass();
}

void CDataSceneNode::render()
{
	auto driver = SceneManager->getVideoDriver();
	

	if (!m_pMesh || !driver)
	{
		return;
	}

	bool isTransparentPass = SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;

	++m_PassCount;

	m_Box = m_pMesh->getBoundingBox();

	if (m_pShadow && m_PassCount==1)
	{
		m_pShadow->updateShadowVolumes();
	}

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	for (irr::u32 i=0; i<m_pMesh->getMeshBufferCount(); ++i )
	{
		auto mb = m_pMesh->getMeshBuffer(i);

		auto& mat = mb->getMaterial();
		driver->setMaterial(mat);
		driver->drawMeshBuffer(mb);
	}

	// for debug purposes only:
	if (DebugDataVisible)
	{
		video::SMaterial m;
		m.Lighting = false;
		m.AntiAliasing=4;
		m.Thickness = 2;
		driver->setMaterial(m);

		if (DebugDataVisible & scene::EDS_BBOX)
		{
			driver->draw3DBox(m_Box, m_DbgColor);
		}
		if (DebugDataVisible & scene::EDS_NORMALS)
		{
			// draw normals
			const f32 debugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
			const video::SColor debugNormalColor = SceneManager->getParameters()->getAttributeAsColor(DEBUG_NORMAL_COLOR);
			const u32 count = m_pMesh->getMeshBufferCount();

			for (u32 i=0; i != count; ++i)
			{
				driver->drawMeshBufferNormals(m_pMesh->getMeshBuffer(i), debugNormalLength, debugNormalColor);
			}
		}
	}

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
}

const irr::core::aabbox3df& CDataSceneNode::getBoundingBox() const
{
	return m_pMesh ? m_pMesh->getBoundingBox() : m_Box;
}

void CDataSceneNode::OnRegisterSceneNode()
{
	if ( !IsVisible )
	{
		return;
	}

 	m_PassCount = 0;
// 	SceneManager->registerNodeForRendering(this);
// 
// 	video::IVideoDriver* driver = SceneManager->getVideoDriver();
// 
// 	auto transparentCount=0, solidCount=0;
// 
// 	// count copied materials
// 	for (irr::u32 i=0; i<m_pMesh->getMeshBufferCount(); ++i)
// 	{
// 		auto mb = m_pMesh->getMeshBuffer(i);
// 		auto& mat = mb->getMaterial();
// 
// 		auto rnd =	driver->getMaterialRenderer(mat.MaterialType);
// 
// 		if (rnd && rnd->isTransparent())
// 			++transparentCount;
// 		else
// 			++solidCount;
// 
// 		if (solidCount && transparentCount)
// 			break;
// 	}
// 
// 	// register according to material types counted
// 
// 	if (solidCount)
// 	{
// 		SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);
// 	}
// 
// 	if (transparentCount)
// 	{
// 		SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
// 	}

	SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

bool CDataSceneNode::removeChild(irr::scene::ISceneNode* child)
{
	if (child && m_pShadow == child)
	{
		m_pShadow->drop();
		m_pShadow = nullptr;
	}

	return ISceneNode::removeChild(child);
}

irr::video::SMaterial& CDataSceneNode::getMaterial( u32 i )
{
	if (m_pMesh && i<m_pMesh->getMeshBufferCount())
	{
		return m_pMesh->getMeshBuffer(i)->getMaterial();
	}

	return ISceneNode::getMaterial(i);
}

irr::u32 CDataSceneNode::getMaterialCount() const
{
	if (m_pMesh)
	{
		return m_pMesh->getMeshBufferCount();
	}

	return ISceneNode::getMaterialCount();
}

void CDataSceneNode::setMesh( irr::scene::IMesh* mesh )
{
	if (!mesh)
	{
		return;
	}

	mesh->grab();
	if (m_pMesh)
	{
		m_pMesh->drop();
	}

	m_pMesh = mesh;
}

irr::scene::IShadowVolumeSceneNode* CDataSceneNode::addShadowVolumeSceneNode( const IMesh* shadowMesh, s32 id, bool zfailmethod, f32 infinity )
{
	if (!SceneManager->getVideoDriver()->queryFeature(video::EVDF_STENCIL_BUFFER))
		return 0;

	if (!shadowMesh)
	{
		shadowMesh = m_pMesh; // if null is given, use the mesh of node
	}

	if (m_pShadow)
	{
		m_pShadow->drop();
	}

	m_pShadow = new CShadowVolumeSceneNode(shadowMesh, this, SceneManager, id,  zfailmethod, infinity);
	return m_pShadow;
}

irr::scene::ISceneNode* CDataSceneNode::clone( irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager )
{
	return nullptr;
}

irr::scene::IMesh* CDataSceneNode::getMesh()
{
	return m_pMesh;
}

void CDataSceneNode::AddToShadowPass()
{
	m_InShadowPass = true;
	if ( isVisible() )
	{
		m_SRenderContextWPtr.lock()->AddShadowToNode(this);
	}
}

void CDataSceneNode::RemoveFromShadowPass()
{
	auto sptr = m_SRenderContextWPtr.lock();
	if ( sptr )
	{
		sptr->RemoveShadowFromNode(this);
	}
	m_InShadowPass = false;
}

void CDataSceneNode::AddToDepthPass()
{
	m_InDepthPass = true;
	if ( isVisible() )
	{
		m_SRenderContextWPtr.lock()->AddNodeToDepthPass(this);
	}
}

void CDataSceneNode::RemoveFromDepthPass()
{
	auto sptr = m_SRenderContextWPtr.lock();
	if ( sptr )
	{
		sptr->RemoveNodeFromDepthPass(this);
	}
	m_InDepthPass = false;
}

void CDataSceneNode::CreateTriSelector()
{
	assert(m_pMesh);

	auto selector = SceneManager->createTriangleSelector(m_pMesh, this);
	setTriangleSelector(selector);
	selector->drop();
}

void CDataSceneNode::setVisible( bool isVisible )
{
	ISceneNode::setVisible(isVisible);

	if ( isVisible )
	{
		if ( m_InDepthPass )
		{
			m_SRenderContextWPtr.lock()->AddNodeToDepthPass(this);
		}

		if ( m_InShadowPass )
		{
			m_SRenderContextWPtr.lock()->AddShadowToNode(this);
		}
	}
	else
	{
		if ( m_InDepthPass )
		{
			m_SRenderContextWPtr.lock()->RemoveNodeFromDepthPass(this);
		}

		if ( m_InShadowPass )
		{
			m_SRenderContextWPtr.lock()->RemoveShadowFromNode(this);
		}
	}
}
