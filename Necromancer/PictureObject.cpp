#include "stdafx.h"
#include "PictureObject.h"


#include "PIL.h"
#include "PILLLGI.h"
#include "RenderingResource.h"

namespace Necromancer{
	Tex2DPtr create_texture_from_file(const char* file_name){
		return nullptr;
	}

	Tex2DPtr create_texture_from_file(const wchar_t* file_name){
		return nullptr;
	}

	PictureObject::PictureObject(const char* file_name)
		:TwoDObject(create_texture_from_file(file_name))
	{
	}

	PictureObject::PictureObject(const wchar_t* file_name)
		:TwoDObject(create_texture_from_file(file_name))
	{
	}

	PictureObject::~PictureObject(){
	}
}