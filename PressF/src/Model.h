#pragma once
#include "types.h"
#include "OBJ_Loader.h"


class Model 
	: public objl::Loader
{
public:
	Model();
	~Model();
};

//void GLModelCreate(Model &model) {
//
//}
