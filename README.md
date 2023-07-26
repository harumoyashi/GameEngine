# NekoEngine
このゲームエンジンはDirectX12を使用し、3Dゲームを作る基盤となるものを実装したエンジンです。  
「今作るアプリに必要なものを実装する」「アプリを作るうえで不便だと感じ始めたらリファクタリングを行う」という2点を重視して設計しています。  
## 基本機能
- DirectX12による3D描画
- 当たり判定のシステム化
- XAudio2を用いたサウンド再生
- Media Foundationを用いたサウンドファイルの読み込み
- DInput,Xnputによるキーボード、マウス、Xboxゲームパッドの入力
- Assimpを用いたFBXの読み込み
- どこでも簡単に使えるイージング
- blenderで出力したレベルエディタの読み込み
- ジオメトリシェーダーを用いた1頂点パーティクルの3D描画
## 追加予定の機能
- GPUパーティクルをメッシュに貼り付け
- ポストエフェクトをクラスとシェーダーを作るだけで使えるように
## 使用している外部ライブラリ
- [DirectXTex](https://github.com/microsoft/DirectXTex)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Assimp](https://github.com/assimp/assimp)
