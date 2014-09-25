#ifndef CDataSceneNodeFwd_h__
#define CDataSceneNodeFwd_h__
#pragma once

#include <memory>

class	CDataSceneNode;
typedef	std::shared_ptr<CDataSceneNode>	CDataSceneNodeSPtr;
typedef	std::weak_ptr<CDataSceneNode>	CDataSceneNodeWPtr;

#endif // CDataSceneNodeFwd_h__
