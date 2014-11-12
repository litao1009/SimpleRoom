#include "stdafx.h"

#include "ExtShaders.h"
#include "irrlicht.h"

const char*	LuminanceSahder[2] = 
{
	//"varying	vec4		vertexColor;"

	"void main(void)"
	"{"
	"	gl_Position = ftransform();"
	//"	vertexColor = gl_Color;"
	"	gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;"
	"}"
	,
	"uniform	int			hasTexture;"
	"uniform	sampler2D	colorMap;"

	//"varying	vec4 vertexColor;"

	"void main(void)"
	"{"
	"	vec4 color;"
	"	"
	"	if ( hasTexture == 1 )"
	"	{"
	"		color = texture2D(colorMap, vec2(gl_TexCoord[0]));"
	"		float lum = color.r*0.3 + color.g*0.59 + color.b*0.11;"
	"		color = vec4(lum,lum,lum,1);"
	"		gl_FragColor = color;"
	"	}"
	"	else"
	"	{"
	"		color = gl_Color;"
	"		color.r = 1;"
	"		gl_FragColor = color;"
	"	}"
	"}"
};


void LuminanceCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	irr::s32 tex = 0;
	services->setPixelShaderConstant("colorMap", &tex, 1);

	auto hasTexture = CurrentMaterial_.TextureLayer[0].Texture ? 1 : 0;
	services->setPixelShaderConstant("hasTexture", &hasTexture, 1);
}

void LuminanceCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* LuminanceCB::GetVertexShader()
{
	return LuminanceSahder[0];
}

const char* LuminanceCB::GetPixelShader()
{
	return LuminanceSahder[1];
}



const char*	LineColorShader[2] = 
{
	"void main(void)"
	"{"
	"	gl_Position = ftransform();"
	//"	gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;"
	"}"

	,

	"uniform	vec4			lineColor;"
	"void main(void)"
	"{"
	"	gl_FragColor = lineColor;"
	"}"
};

void LineColorCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	static irr::video::SColorf color;
	auto& curColor = CurrentMaterial_.DiffuseColor;
	color.set(1, static_cast<float>(curColor.getRed())/255.f, static_cast<float>(curColor.getGreen())/255.f, static_cast<float>(curColor.getBlue())/255.f);
	services->setPixelShaderConstant("lineColor", &color.r, 4);
}

void LineColorCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* LineColorCB::GetVertexShader()
{
	return LineColorShader[0];
}

const char* LineColorCB::GetPixelShader()
{
	return LineColorShader[1];
}


const char*	VertexAlhpa[2] = 
{
	"void main(void)"
	"{"
	"	gl_Position = ftransform();"
	"	gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;"
	"}"

	,

	"uniform	float			factor;"
	"uniform	sampler2D		colorMap;"

	"void main(void)"
	"{"
	"	vec4 color = texture2D(colorMap, vec2(gl_TexCoord[0]));"
	"	color.a *= factor;"
	"	gl_FragColor = color;"
	"}"
};

void VertexAlphaCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	irr::s32 pos = 0;

	services->setPixelShaderConstant("colorMap", &pos, 1);
	services->setPixelShaderConstant("factor", &CurrentMaterial_.MaterialTypeParam, 1);
}

void VertexAlphaCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* VertexAlphaCB::GetVertexShader()
{
	return VertexAlhpa[0];
}

const char* VertexAlphaCB::GetPixelShader()
{
	return VertexAlhpa[1];
}

const char*	FontColor[2] = 
{
	"void main(void)"
	"{"
	"	gl_Position = ftransform();"
	"	gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;"
	"}"

	,

	"uniform	vec4			fontColor;"
	"uniform	sampler2D		colorMap;"

	"void main(void)"
	"{"
	"	vec4 color = texture2D(colorMap, vec2(gl_TexCoord[0]));"
	"	color.rgb = fontColor.rgb;"
	"	gl_FragColor = color;"
	"}"
};

void FontColorCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	irr::s32 pos = 0;
	static irr::video::SColorf color;
	auto& curColor = CurrentMaterial_.DiffuseColor;
	color.set(1, static_cast<float>(curColor.getRed())/255.f, static_cast<float>(curColor.getGreen())/255.f, static_cast<float>(curColor.getBlue())/255.f);

	services->setPixelShaderConstant("colorMap", &pos, 1);
	services->setPixelShaderConstant("fontColor", &color.r, 4);
}

void FontColorCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* FontColorCB::GetVertexShader()
{
	return FontColor[0];
}

const char* FontColorCB::GetPixelShader()
{
	return FontColor[1];
}
