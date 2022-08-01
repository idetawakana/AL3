#pragma once
#include "WorldTransform.h"

float radian(float a);

float pi();

float angle(float a);

void Determinant(WorldTransform worldtransform_, Vector3 scale, Vector3 rotation, Vector3 translation);

void translation_(WorldTransform worldTransform, Vector3 trans);

void Rotation(WorldTransform worldtransform_, Vector3 rotation);

Vector3 operator*(const Vector3& v, Matrix4& m);