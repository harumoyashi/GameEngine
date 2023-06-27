#pragma once
#include "NSprite.h"

class NPostEffect :
    public NSprite
{
private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff_;
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff_;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

    //画面クリアカラー
    static const float kClearColor[4];

public:
    NPostEffect();

    void Init();
    void Draw();

    //テクスチャ生成
    void CreateTexture();
    //レンダーターゲットビュー生成
    void CreateRTV();
    //深度バッファ生成
    void CreateDepthBuff();
    //デプスステンシルビュー生成
    void CreateDSV();

    void PreDrawScene();
    void PostDrawScene();
};

