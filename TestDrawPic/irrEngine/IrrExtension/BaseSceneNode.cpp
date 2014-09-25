#include "stdafx.h"

#include "BaseSceneNode.h"
#include "irrEngine/SRenderContext.h"

using namespace irr;
using namespace scene;

CBaseSceneNode::CBaseSceneNode( SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr, std::string typeName/*=""*/ )
	:IMeshSceneNode(0, rcWPtr.lock()->Smgr_.get(), -1),
	TypeName_(typeName),
	RenderContextWPtr_(rcWPtr),
	BaseODLWPtr_(odlWPtr)
{
	DbgColor_.setAlpha(0xff);
	DbgColor_.setRed(0xff&rand());
	DbgColor_.setGreen(0xff&rand());
	DbgColor_.setBlue(0xff&rand());
}

CBaseSceneNode::~CBaseSceneNode()
{

}
