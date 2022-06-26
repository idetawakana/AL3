#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t textureHandle_ = 0;

	Model* model_ = nullptr;

	WorldTransform worldtransform_;
	WorldTransform worldtransform1_;

	ViewProjection viewProjection_;

	DebugCamera* debugCamera_ = nullptr;

	int32_t speed = 1;

	float sx = 1.0f;
	float sy = 1.0f;
	float sz = 1.0f;

	float rx = 0.0f;
	float ry = 0.0f;
	float rz = 0.0f;

	float tx = 0.0f;
	float ty = 0.0f;
	float tz = 0.0f;

	Vector3 translation = { 0,0,0 };

	int32_t mode = 2;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
