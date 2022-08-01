#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>


///<summary>
///���L����
///</summary>
class Player {
public:
	///<summary>
	///������
	///</summary>
	///<param name = "model">���f��</param>
	///<param name = "textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///�X�V
	///</summary>
	void Update();

	///<summary>
	///�`��
	///</summary>
	///<param name = "viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection viewProjection_);

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	Vector3 move = { 0,0,0 };
	Vector3 rotate = { 0,0,0 };
	Vector3 scale = { 1,1,1 };

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};