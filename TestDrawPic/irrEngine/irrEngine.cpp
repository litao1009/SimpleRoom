#include "stdafx.h"

#include "irrEngine.h"
#include "Irrlicht/CSceneNodeAnimatorCameraMayaRH.h"
#include "Irrlicht/CIrrDeviceWin32.h"
#include "Irrlicht/os.h"
#include "SRenderContext.h"

#include "irrEngine/IrrExtension/ExtShaders.h"

#include <map>
#include <vector>
#include <string>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

class	IrrEngine::Imp
{
public:

	boost::filesystem::path					ResPath_;
	irr::SIrrlichtCreationParameters		Params_;
	std::shared_ptr<irr::CIrrDeviceWin32>	DeviceSPtr_;
	std::map<EShaderType,int>				ShaderMap_;
	std::vector<UpdateFunctor>				UpdateList_;
};


static	IrrEngineUPtr&	GetStaticInstance()
{
	static	IrrEngineUPtr	sInstance;

	return sInstance;
}

void	IrrEngine::CreateDeviceInstance(
	HWND hWnd,
	bool StencilBuffer,
	int Antialias,
	bool Vsync,
	int Bits)
{
	auto& sptr = GetInstance();

	if ( sptr )
	{
		return;
	}

	irr::SIrrlichtCreationParameters paras;
	paras.WindowId = hWnd;
	paras.Stencilbuffer = StencilBuffer;
	paras.AntiAlias = Antialias;
	paras.Vsync = Vsync;
	paras.Bits = Bits;
	paras.ZBufferBits = Bits;
	paras.DriverType = irr::video::EDT_OPENGL;
	paras.Doublebuffer = true;
	
	sptr.reset(new IrrEngine(paras));
}

IrrEngineUPtr& IrrEngine::GetInstance()
{
	return GetStaticInstance();
}

void IrrEngine::ReleseRenderInstance()
{
	GetStaticInstance().reset();
}

SRenderContextSPtr IrrEngine::CreateRenderContext( HWND hWnd, irr::video::SColor bgClr )
{
	auto ret = std::make_shared<SRenderContext>(hWnd, bgClr);
	ret->Timer_.reset(GetInstance()->GetDevice()->getTimer(), [](irr::ITimer*){});

	return ret;
}

IrrEngine::IrrEngine(const irr::SIrrlichtCreationParameters& params):ImpUPtr_(new Imp)
{
	auto& imp_ = *ImpUPtr_;
	imp_.Params_ = params;

	auto rawDevice = irr::createDeviceEx(imp_.Params_);
	imp_.DeviceSPtr_.reset(static_cast<irr::CIrrDeviceWin32*>(rawDevice), std::bind(&irr::IrrlichtDevice::drop, std::placeholders::_1));

	{
		auto selectionCB = new LuminanceCB;
		auto material = rawDevice->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(LuminanceCB::GetVertexShader(), LuminanceCB::GetPixelShader(), selectionCB);
		selectionCB->drop();
		imp_.ShaderMap_[EST_LUMINANCE] = material;
	}

	{
		auto blueLine = new LineColorCB;
		auto material = rawDevice->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(LineColorCB::GetVertexShader(), LineColorCB::GetPixelShader(), blueLine);
		blueLine->drop();
		imp_.ShaderMap_[EST_LINE] = material;
	}

	{
		auto vertexAlpha = new VertexAlphaCB;
		auto material = rawDevice->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(VertexAlphaCB::GetVertexShader(), VertexAlphaCB::GetPixelShader(), vertexAlpha, irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
		vertexAlpha->drop();
		imp_.ShaderMap_[EST_VERTEX_ALPHA] = material;
	}

	{
		auto font = new FontColorCB;
		auto material = rawDevice->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(FontColorCB::GetVertexShader(), FontColorCB::GetPixelShader(), font, irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
		font->drop();
		imp_.ShaderMap_[EST_FONT] = material;
	}

	{
		auto ads = new ADSLightCB;
		auto material = rawDevice->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(ADSLightCB::GetVertexShader(), ADSLightCB::GetPixelShader(), ads);
		ads->drop();
		imp_.ShaderMap_[EST_ADS_LIGHT] = material;

		auto ftr = [ads](const SRenderContextSPtr& rc)
		{
			ads->SetSmgr(rc->Smgr_.get());
		};

		Register(ftr);
	}

	{
		auto picking = new PickingColorCB;
		auto material = rawDevice->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(PickingColorCB::GetVertexShader(), PickingColorCB::GetPixelShader(), picking);
		picking->drop();
		imp_.ShaderMap_[EST_PICKING] = material;
	}
}

IrrEngine::~IrrEngine()
{

}

irr::IrrlichtDevice* IrrEngine::GetDevice()
{
	return ImpUPtr_->DeviceSPtr_.get();
}

irr::video::E_MATERIAL_TYPE IrrEngine::GetShaderType( EShaderType enm )
{
	return static_cast<irr::video::E_MATERIAL_TYPE>(ImpUPtr_->ShaderMap_.at(enm));
}

bool IrrEngine::SetResPath( boost::filesystem::path dirPath )
{
	if ( !boost::filesystem::is_directory(dirPath) )
	{
		return false;
	}

	if ( !boost::filesystem::exists(dirPath) )
	{
		boost::filesystem::create_directories(dirPath);
	}

	ImpUPtr_->ResPath_ = dirPath;

	return true;
}

const boost::filesystem::path& IrrEngine::GetResPath() const
{
	return ImpUPtr_->ResPath_;
}

bool IrrEngine::WriteTextureToDisk( irr::video::ITexture* tex, const char* filename )
{
	auto driver = GetInstance()->GetDevice()->getVideoDriver();
	auto img = GetInstance()->GetDevice()->getVideoDriver()->createImageFromData(tex->getColorFormat(), tex->getSize(), tex->lock(), false, false);
	auto ret = driver->writeImageToFile(img, filename);
	tex->unlock();
	img->drop();
	return ret;
}

void IrrEngine::Dump( const irr::core::vector3df& vec, const char* name )
{
	boost::format fmt("%s (X: %f, Y: %f, Z: %f)");
	fmt % name % vec.X % vec.Y % vec.Z;

	irr::os::Printer::print(fmt.str().c_str());
}

void IrrEngine::Dump( const irr::core::vector2df& vec, const char* name )
{
	boost::format fmt("%s (X: %f, Y: %f)");
	fmt % name % vec.X % vec.Y;

	irr::os::Printer::print(fmt.str().c_str());
}

void IrrEngine::Dump( const irr::core::vector2di& vec, const char* name )
{
	boost::format fmt("%s (X: %d, Y: %d)");
	fmt % name % vec.X % vec.Y;

	irr::os::Printer::print(fmt.str().c_str());
}

void IrrEngine::Dump( const char* simpleString )
{
	irr::os::Printer::print(simpleString);
}

void IrrEngine::Update( const SRenderContextSPtr& rc )
{
	for ( auto& curFtr : ImpUPtr_->UpdateList_ )
	{
		curFtr(rc);
	}
}

void IrrEngine::Register( const UpdateFunctor& ftr )
{
	ImpUPtr_->UpdateList_.push_back(ftr);
}

