#pragma once
#include "NSprite.h"
class NPostEffect :
    public NSprite
{
private:
    ComPtr<ID3D12Resource> texBuff_;
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
    NPostEffect();

    void Init();
    void Draw();
};

