#pragma once
#include "IEnemy.h"

class Mouse final :
    public IEnemy
{
public:
    std::string modelname = "mouse";     //ƒ‚ƒfƒ‹–¼

    Mouse();
};