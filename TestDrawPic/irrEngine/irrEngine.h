#pragma once

#include "irrlicht.h"
#include "EShaderType.h"
#include "SRenderContextFwd.h"
#include "irrEngineFwd.h"

#include <boost/filesystem/path.hpp>
#include <windows.h>


/*
	渲染引擎实例，应用程序生存周期内只保存一个实例
*/
class	IrrEngine
{
	class Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	//************************************
	// Method:  IrrEngine::CreateDeviceInstance
	// Access:    public static 
	// Returns:   void
	// Parameter: HWND hWnd				程序主窗口
	// Parameter: bool StencilBuffer	模板缓冲
	// Parameter: int Antialias			抗锯齿
	// Parameter: bool Vsync			垂直同步
	// Parameter: int Bits				深度缓冲
	/*
		创建实例
	*/
	//************************************
	static	void				CreateDeviceInstance(HWND hWnd,
													bool StencilBuffer=false,
													int Antialias=4,
													bool Vsync=false,
													int Bits=16);

	//************************************
	// Method:  IrrEngine::GetInstance
	// Access:    public static 
	// Returns:   IrrEngineUPtr&
	/*
		确保首先调用IrrEngine::CreateDeviceInstance
	*/
	//************************************
	static	IrrEngineUPtr&		GetInstance();

	//************************************
	// Method:  IrrEngine::ReleseRenderInstance
	// Access:    public static 
	// Returns:   void
	/*
		释放引擎实例
	*/
	//************************************
	static	void				ReleseRenderInstance();

	//************************************
	// Method:  IrrEngine::CreateRenderContext
	// Access:    public static 
	// Returns:   SRenderContextSPtr
	// Parameter: HWND hWnd					渲染子窗口
	// Parameter: irr::video::SColor bgClr	背景颜色
	/*
		创建场景上下文
	*/
	//************************************
	static	SRenderContextSPtr	CreateRenderContext(HWND hWnd, irr::video::SColor bgClr=irr::video::SColor(255,100,100,100));

	static	bool				WriteTextureToDisk(irr::video::ITexture* tex, const char* filename);

public:

	IrrEngine(const irr::SIrrlichtCreationParameters& params);

	~IrrEngine();

public:

	irr::IrrlichtDevice*		GetDevice();

	irr::video::E_MATERIAL_TYPE	GetShaderType(EShaderType enm);

	//************************************
	// Method:  IrrEngine::SetResPath
	// Access:    public 
	// Returns:   bool
	// Parameter: boost::filesystem::path dirPath
	/*
		设置资源的默认路径
	*/
	//************************************
	bool						SetResPath(boost::filesystem::path dirPath);

	const boost::filesystem::path&	GetResPath() const;
};