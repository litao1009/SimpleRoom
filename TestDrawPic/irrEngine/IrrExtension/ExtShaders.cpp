#include "stdafx.h"

#include "ExtShaders.h"
#include "irrlicht.h"

const char*	Selection[2] = 
{
	"varying	vec4		vertexColor;"

	"void main(void)"
	"{"
	"	gl_Position = ftransform();"
	"	vertexColor = gl_Color;"
	"	gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;"
	"}"
	,
	"uniform	int			hasTexture;"
	"uniform	sampler2D	colorMap;"

	"varying	vec4 vertexColor;"

	"void main(void)"
	"{"
	"	vec4 color;"
	"	"
	"	if ( hasTexture == 1 )"
	"	{"
	"		color = texture2D(colorMap, vec2(gl_TexCoord[0]));"
	"		color.r = 1;"
	"		gl_FragColor = color;"
	"	}"
	"	else"
	"	{"
	"		color = vertexColor;"
	"		color.r = 1;"
	"		gl_FragColor = color;"
	"	}"
	"}"
};


void SelectionCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	irr::s32 tex = 0;
	services->setPixelShaderConstant("colorMap", &tex, 1);

	auto hasTexture = CurrentMaterial_.TextureLayer[0].Texture ? 1 : 0;
	services->setPixelShaderConstant("hasTexture", &hasTexture, 1);
}

void SelectionCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* SelectionCB::GetVertexShader()
{
	return Selection[0];
}

const char* SelectionCB::GetPixelShader()
{
	return Selection[1];
}
