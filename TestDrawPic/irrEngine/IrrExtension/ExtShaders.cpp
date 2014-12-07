#include "stdafx.h"

#include "ExtShaders.h"
#include "irrlicht.h"

#include "irrEngine/irrEngine.h"
#include "irrEngine/SRenderContext.h"

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



const char*	ADSLight[2] = 
{
	"uniform	mat4	mvMatrix;"
	"uniform	mat3	normalMatrix;"
	"uniform	vec3	vLightPos;"

	"varying	vec3	vVaryingLightDir;"
	"varying	vec3	vVaryingNormal;"

	"void main()"
	"{	"
	"	vec4 pos4 = mvMatrix * gl_Vertex;"
	"	vec3 pos3 = pos4.xyz/pos4.w;"
	"	"
	"	gl_Position 	= ftransform();"
	"	"
	"	vVaryingNormal	= normalMatrix * gl_Normal;"
	"	"
	"	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
	"	"
	"	vVaryingLightDir = normalize(vLightPos - pos3);"
	"}"
	,

	"uniform vec4		diffuseColor;"
	"uniform vec4		ambientColor;"
	"uniform vec4		FragColor;"
	"uniform float		FragAlpha = 1;"
	"uniform int		HasTexture;"
	"uniform sampler2D	colorMap;"

	"varying vec3 vVaryingNormal;"
	"varying vec3 vVaryingLightDir;"


	"void main(void)"
	"{ "
	"	float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));"

	"	gl_FragColor = diff * diffuseColor;"

	"	if ( 1 == HasTexture )"
	"	{"
	"		gl_FragColor *= texture2D(colorMap, gl_TexCoord[0].xy);"
	"	}"
	"	else"
	"	{"
	"		gl_FragColor *= FragColor;"
	"	}"

	"	gl_FragColor += ambientColor;"
	"	gl_FragColor.a = FragAlpha;"
	"}	"
};

void ADSLightCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	auto driver = services->getVideoDriver();

	irr::core::matrix4 mvMatrix;
	mvMatrix = driver->getTransform(irr::video::ETS_VIEW);
	mvMatrix = driver->getTransform(irr::video::ETS_WORLD);

	auto norMat = mvMatrix;
	norMat.makeInverse();
	norMat = norMat.getTransposed();

	irr::f32 normalMatrix[9] = {0};
	normalMatrix[0] = norMat.pointer()[0];
	normalMatrix[1] = norMat.pointer()[1];
	normalMatrix[2] = norMat.pointer()[2];
	normalMatrix[3] = norMat.pointer()[4];
	normalMatrix[4] = norMat.pointer()[5];
	normalMatrix[5] = norMat.pointer()[6];
	normalMatrix[6] = norMat.pointer()[8];
	normalMatrix[7] = norMat.pointer()[9];
	normalMatrix[8] = norMat.pointer()[10];

	irr::core::vector3df Camera = Smgr_->getActiveCamera()->getAbsolutePosition();

	static irr::video::SColorf Whitecol(1.0f,1.0f,1.0f,0.0f);
	static irr::video::SColorf Blackcol(0.1f,0.1f,0.1f,0.1f);
	irr::video::SColorf fragColor(CurrentMaterial_.DiffuseColor);

	services->setVertexShaderConstant("mvMatrix", mvMatrix.pointer(), 16);
	services->setVertexShaderConstant("normalMatrix", normalMatrix, 9);		
	services->setVertexShaderConstant("vLightPos", &Camera.X, 3);

	services->setPixelShaderConstant("ambientColor",&Blackcol.r, 4);
	services->setPixelShaderConstant("diffuseColor",&Whitecol.r, 4);
	services->setPixelShaderConstant("FragAlpha",&CurrentMaterial_.Shininess, 1);
	services->setPixelShaderConstant("FragColor",&fragColor.r, 4);

	irr::s32 pos = 0;
	irr::s32 hasTexture = CurrentMaterial_.getTexture(0) ? 1 : 0;

	services->setPixelShaderConstant("colorMap", &pos, 1);
	services->setPixelShaderConstant("HasTexture", &hasTexture, 1);
}

void ADSLightCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* ADSLightCB::GetVertexShader()
{
	return ADSLight[0];
}

const char* ADSLightCB::GetPixelShader()
{
	return ADSLight[1];
}



const char*	PickingColorShader[2] = 
{
	//"varying	vec4		vertexColor;"

	"void main(void)"
	"{"
	"	gl_Position = ftransform();"
	//"	vertexColor = gl_Color;"
	"	gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;"
	"}"
	,
	"uniform	vec4		pickingColor;"
	"uniform	sampler2D	colorMap;"

	//"varying	vec4 vertexColor;"

	"void main(void)"
	"{"
	"	vec4 color;"
	""
	"	color = texture2D(colorMap, vec2(gl_TexCoord[0]));"
	"	float lum = color.r*0.3 + color.g*0.59 + color.b*0.11;"
	"	if ( lum > 0.8 )"
	"	{"
	"		gl_FragColor = pickingColor;"
	"	}"
	"	else"
	"	{"
	"		gl_FragColor = color;"
	"	}"
	"}"
};


void PickingColorCB::OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 userData )
{
	irr::s32 tex = 0;
	services->setPixelShaderConstant("colorMap", &tex, 1);

	irr::video::SColorf color;
	auto& curColor = CurrentMaterial_.DiffuseColor;
	color.set(1, static_cast<float>(curColor.getRed())/255.f, static_cast<float>(curColor.getGreen())/255.f, static_cast<float>(curColor.getBlue())/255.f);

	services->setPixelShaderConstant("pickingColor", &color.r, 4);
}

void PickingColorCB::OnSetMaterial( const irr::video::SMaterial& material )
{
	CurrentMaterial_ = material;
}

const char* PickingColorCB::GetVertexShader()
{
	return PickingColorShader[0];
}

const char* PickingColorCB::GetPixelShader()
{
	return PickingColorShader[1];
}

void ExtShaders::Init( const IrrEngineUPtr& engine )
{
	auto device = engine->GetDevice();

	{
		auto selectionCB = new LuminanceCB;
		auto material = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(LuminanceCB::GetVertexShader(), LuminanceCB::GetPixelShader(), selectionCB);
		selectionCB->drop();
		engine->AddShaderType(EST_LUMINANCE, static_cast<irr::video::E_MATERIAL_TYPE>(material));
	}

	{
		auto blueLine = new LineColorCB;
		auto material = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(LineColorCB::GetVertexShader(), LineColorCB::GetPixelShader(), blueLine);
		blueLine->drop();
		engine->AddShaderType(EST_LINE, static_cast<irr::video::E_MATERIAL_TYPE>(material));
	}

	{
		auto vertexAlpha = new VertexAlphaCB;
		auto material = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(VertexAlphaCB::GetVertexShader(), VertexAlphaCB::GetPixelShader(), vertexAlpha, irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
		vertexAlpha->drop();
		engine->AddShaderType(EST_VERTEX_ALPHA, static_cast<irr::video::E_MATERIAL_TYPE>(material));
	}

	{
		auto font = new FontColorCB;
		auto material = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(FontColorCB::GetVertexShader(), FontColorCB::GetPixelShader(), font, irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
		font->drop();
		engine->AddShaderType(EST_FONT, static_cast<irr::video::E_MATERIAL_TYPE>(material));
	}

	{
		auto ads = new ADSLightCB;
		auto material = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(ADSLightCB::GetVertexShader(), ADSLightCB::GetPixelShader(), ads);
		ads->drop();
		engine->AddShaderType(EST_ADS_LIGHT, static_cast<irr::video::E_MATERIAL_TYPE>(material));

		auto ftr = [ads](const SRenderContextSPtr& rc)
		{
			ads->SetSmgr(rc->Smgr_.get());
		};

		engine->Register(ftr);
	}

	{
		auto picking = new PickingColorCB;
		auto material = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(PickingColorCB::GetVertexShader(), PickingColorCB::GetPixelShader(), picking);
		picking->drop();
		engine->AddShaderType(EST_PICKING, static_cast<irr::video::E_MATERIAL_TYPE>(material));
	}
}
