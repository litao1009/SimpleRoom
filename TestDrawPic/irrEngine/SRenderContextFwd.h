#ifndef SRenderContextFwd_h__
#define SRenderContextFwd_h__

#pragma once

#include <memory>

class	SRenderContext;

typedef	std::shared_ptr<SRenderContext>	SRenderContextSPtr;
typedef	std::weak_ptr<SRenderContext>	SRenderContextWPtr;


#endif // SRenderContextFwd_h__