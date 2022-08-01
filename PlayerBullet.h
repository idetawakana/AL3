#pragma once
#include "Model.h"
#include "WorldTransform.h"

///<summary>
///自キャラの弾
///</summary>
class PlayerBullet {
public:
	///<summary>
	///初期化
	///</summary>
	///<param name = "model">モデル</param>
	///<param name = "position">初期座標</param>
	///<param name = "velocity">速度</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	///<summary>
	///更新
	///</summary>
	void Update();

	///<summary>
	///初期化
	///</summary>
	///<param name = "viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	Vector3 move = { 0,0,0 };
	Vector3 rotate = { 0,0,0 };
	Vector3 scale = { 1,1,1 };

	//速度
	Vector3 velocity_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};