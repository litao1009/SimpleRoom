#ifndef EShaderType_h__
#define EShaderType_h__
#pragma once

enum EShaderType
{
	//��ͼ������
	EST_TEXTURE_LIGHT,
	
	//��ͼ͸��������
	EST_TEXTURE_LIGHT_TRANSPARENT,
	
	//��ɫ������
	EST_COLOR_LIGHT,
	
	//��ɫ͸��������
	EST_COLOR_LIGHT_TRANSPARENT,
	
	//������ͼ������
	EST_NORMAL_LIGHT_TEXTURE,
	
	EST_COUNT
};

#endif // EShaderType_h__
