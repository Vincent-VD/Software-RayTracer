#pragma once

#include "EMath.h"
#include "ERGBColor.h"
#include "BaseMaterial.h"

using namespace Elite;

struct Ray {

	FPoint3 origin{};
	FVector3 direction{};
	float tMin{ 0.0001f };
	float tMax{ FLT_MAX };
};

struct HitRecord {

	FPoint3 hitPoint{};
	float tValue{};
	FVector3 normal{};
	BaseMaterial* material{};
};

