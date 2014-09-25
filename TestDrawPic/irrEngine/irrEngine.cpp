#include "stdafx.h"

#include "irrEngine.h"
#include "Irrlicht/CSceneNodeAnimatorCameraMayaRH.h"
#include "Irrlicht/CIrrDeviceWin32.h"
#include "Irrlicht/os.h"
#include "SRenderContext.h"

#include <boost/filesystem.hpp>

class	IrrEngine::Imp
{
public:

	boost::filesystem::path					ResPath_;
	irr::SIrrlichtCreationParameters		Params_;
	std::shared_ptr<irr::CIrrDeviceWin32>	DeviceSPtr_;
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
	return static_cast<irr::video::E_MATERIAL_TYPE>(0);
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