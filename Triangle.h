#pragma once
#include <vector>
#include "GeometryObject.h"
#include "TriangleHelpers.h"


class Triangle : public GeometryObject {
public:
	Triangle(const FPoint3 position, const FPoint3& v0, const FPoint3& v1, const FPoint3& v2, const Cullmode cm, BaseMaterial* material, bool isRotating = true);
	virtual ~Triangle() override = default;

	Triangle(const Triangle& other) = delete;
	Triangle(Triangle&& other) = delete;
	Triangle& operator=(Triangle&& other) = delete;
	Triangle& operator=(const Triangle& other) = delete;

	void SetTranslation(const FPoint3& position);
	void SetRotationY(float radians);
	void Update(float elapsedSec) override;

	bool Hit(const Ray& ray, HitRecord& hitRecord, const bool shading = false) const override;

private:
	const Cullmode m_Cullmode;
	std::vector<FPoint3> m_Vertices{ 3 };
	std::vector<FPoint3> m_TransformedVertices{ 3 };

	bool m_IsRotating;
	float m_TotalRotationY;

	FMatrix4 m_RotationTransform;
	FMatrix4 m_TranslationTransform;

	FVector3 m_Normal;

	const float m_RotSpeed{ 1.f };

};

