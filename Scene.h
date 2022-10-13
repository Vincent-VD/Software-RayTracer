#pragma once
#include "GeometryObject.h"
#include "Camera.h"
#include "Light.h"
#include "ETimer.h"
#include <vector>

enum class LightEquationTerm {
	both = 0,
	IrrOnly = 1,
	BRDFOnly = 2
};

class Scene
{
public:
	Scene();
	~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;

	void Update();

	bool Hit(const Ray& ray, HitRecord& hirRecord, const bool shading = false) const;

	//bool Hit(const Ray& ray) const;

	Camera* GetCamera() const;
	std::vector<Light*> GetLights() const;

	void InitWeek1();
	void InitReference();
	void InitTriangles();
	void InitPolyBunny();

	void ToggleShadows();
	void ToggleLET();

	bool GetShading();
	int GetIrradiance();
	int GetBRDF();

	void EmptyGeometryObjects();
	void EmptyLights();
	void EmptyMaterials();

private:
	std::vector<GeometryObject*> m_pGeometryObjects{};
	Camera* m_pCamera{};
	std::vector<Light*> m_pLights{};
	std::vector<BaseMaterial*> m_pMaterials{};
	Timer* m_pTimer;
	bool m_Shadows;
	bool m_IsBunnyActive{ false };
	LightEquationTerm m_LET;

};

