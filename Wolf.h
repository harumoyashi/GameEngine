#pragma once
#include "IEnemy.h"
class Wolf final :
    public IEnemy
{
public:
    std::string modelname = "boss";
};