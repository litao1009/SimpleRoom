#pragma once

#include "irrlicht.h"
#include "EShaderType.h"
#include "SRenderContextFwd.h"
#include "irrEngineFwd.h"

#include <boost/filesystem/path.hpp>
#include <windows.h>


/*
	��Ⱦ����ʵ����Ӧ�ó�������������ֻ����һ��ʵ��
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
	// Parameter: HWND hWnd				����������
	// Parameter: bool StencilBuffer	ģ�建��
	// Parameter: int Antialias			�����
	// Parameter: bool Vsync			��ֱͬ��
	// Parameter: int Bits				��Ȼ���
	/*
		����ʵ��
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
		ȷ�����ȵ���IrrEngine::CreateDeviceInstance
	*/
	//************************************
	static	IrrEngineUPtr&		GetInstance();

	//************************************
	// Method:  IrrEngine::ReleseRenderInstance
	// Access:    public static 
	// Returns:   void
	/*
		�ͷ�����ʵ��
	*/
	//************************************
	static	void				ReleseRenderInstance();

	//************************************
	// Method:  IrrEngine::CreateRenderContext
	// Access:    public static 
	// Returns:   SRenderContextSPtr
	// Parameter: HWND hWnd					��Ⱦ�Ӵ���
	// Parameter: irr::video::SColor bgClr	������ɫ
	/*
		��������������
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
		������Դ��Ĭ��·��
	*/
	//************************************
	bool						SetResPath(boost::filesystem::path dirPath);

	const boost::filesystem::path&	GetResPath() const;
};