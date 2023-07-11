#pragma once
#include "IEnemy.h"
class Wolf :
    public IEnemy
{
public:
    std::string modelname = "boss";
};