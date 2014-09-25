#ifndef EShaderType_h__
#define EShaderType_h__
#pragma once

enum EShaderType
{
	//贴图漫反射
	EST_TEXTURE_LIGHT,
	
	//贴图透明漫反射
	EST_TEXTURE_LIGHT_TRANSPARENT,
	
	//颜色漫反射
	EST_COLOR_LIGHT,
	
	//颜色透明漫反射
	EST_COLOR_LIGHT_TRANSPARENT,
	
	//法线贴图漫反射
	EST_NORMAL_LIGHT_TEXTURE,
	
	EST_COUNT
};

#endif // EShaderType_h__
