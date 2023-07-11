#include "NCollisionManager.h"

NCollisionManager* NCollisionManager::GetInstance()
{
    static NCollisionManager instance;
    return &instance;
}
