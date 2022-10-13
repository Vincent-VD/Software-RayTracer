#pragma once
#include "Triangle.h"
#include "Ray.h"
#include "BaseMaterial.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class TriangleMesh : public GeometryObject{
public:

	TriangleMesh(const FPoint3 position, const std::string filename, const Cullmode cullmode, BaseMaterial* material, bool isRotating = true);
	virtual ~TriangleMesh() override = default;

	TriangleMesh(const TriangleMesh& other) = delete;
	TriangleMesh& operator=(const TriangleMesh& other) = delete;
	TriangleMesh(TriangleMesh&& other) = delete;
	TriangleMesh& operator=(TriangleMesh&& other) = delete;

	void SetTranslation(const FPoint3& position);
	void SetRotationY(float radians);

	void Update(float elapsedSec) override;

	bool Hit(const Ray& ray, HitRecord& hitRecord, const bool shading = false) const override;

private:
	BaseMaterial* m_pMaterial;
	const Cullmode m_Cullmode;

	std::vector<FPoint3> m_Vertices{};
	std::vector<FPoint3> m_TransformedVertices{};
	std::vector<IPoint3> m_Indices{};

	float m_TotalRotationY;

	FMatrix4 m_RotationTransform;
	FMatrix4 m_TranslationTransform;

	bool m_IsRotating;

	void ParseObjFile(const std::string& filename);

};

