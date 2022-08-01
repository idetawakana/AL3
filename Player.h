#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>


///<summary>
///自キャラ
///</summary>
class Player {
public:
	///<summary>
	///初期化
	///</summary>
	///<param name = "model">モデル</param>
	///<param name = "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///更新
	///</summary>
	void Update();

	///<summary>
	///描画
	///</summary>
	///<param name = "viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection viewProjection_);

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;

	Vector3 move = { 0,0,0 };
	Vector3 rotate = { 0,0,0 };
	Vector3 scale = { 1,1,1 };

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};