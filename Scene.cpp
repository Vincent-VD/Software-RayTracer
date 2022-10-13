#include "Scene.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include <iostream>
#include "ERGBColor.h"
#include "BaseMaterial.h"
#include "Material_Lambert.h"
#include "Material_LambertPhong.h"
#include "Material_PBR.h"

Scene::Scene()
	: m_pGeometryObjects{}
	, m_pCamera{ new Camera{ FPoint3{0, 3.f, 9.f}, 45.f } }
	, m_pLights{}
	, m_pTimer{ new Timer{}  }
	, m_LET{ LightEquationTerm::both }
	, m_Shadows{ true }
{
	m_pTimer->Start();
	InitReference();
	//InitPolyBunny(); //When activating this line, remove line 65 to 74
}

Scene::~Scene() {

	m_pTimer->Stop();
	delete m_pTimer;
	delete m_pCamera;
	for (GeometryObject* object : m_pGeometryObjects) {
		delete object;
	}
	for (Light* object : m_pLights) {
		delete object;
	}
	for (BaseMaterial* material : m_pMaterials) {
		delete material;
	}
}

void Scene::Update() {

	//Update timer
	m_pTimer->Update();
	const float elapsedSec{ m_pTimer->GetElapsed() };

	//Update camera rotation
	m_pCamera->Update(elapsedSec);
	for (GeometryObject* object : m_pGeometryObjects) {
		object->Update(elapsedSec);
	}

	//Kayboard handling
	const Uint8* keys{ SDL_GetKeyboardState(0) };
	if (keys[SDL_SCANCODE_Q]) {
		ToggleShadows(); //default off
	}
	if (keys[SDL_SCANCODE_E]) {
		ToggleLET(); //default both
	}
	if (keys[SDL_SCANCODE_B]) {
		if (m_IsBunnyActive) {
			m_IsBunnyActive = false;
			InitReference();
		}
		else {
			m_IsBunnyActive = true;
			InitPolyBunny();
		}
	}
}

Camera* Scene::GetCamera() const {

	return m_pCamera;
}

std::vector<Light*> Scene::GetLights() const {

	return m_pLights;
}

//Loop over all objects, check if Hit, and return smallest HitRecord
bool Scene::Hit(const Ray& ray, HitRecord& hitRecord, const bool shading) const {

	bool res{ false };
	HitRecord temp{};
	temp.tValue = FLT_MAX;
	for (GeometryObject* object : m_pGeometryObjects) {
		if (object->Hit(ray, hitRecord, shading)) {
			if (hitRecord.tValue < temp.tValue && !shading) {
				temp = hitRecord;
			}
			res = true;
		}
	}
	if (!shading) {
		hitRecord = temp;
	}
	return res;
}

void Scene::InitWeek1() {

	EmptyGeometryObjects();
	EmptyLights();
	EmptyMaterials();

	//MATERIALS
	BaseMaterial* lambertPlane{ new Material_Lambert{1.f, RGBColor{0.1f, 0.1f, 0.1f}} };
	BaseMaterial* lambertSphere1{ new Material_Lambert{1.f, RGBColor{1.f, 0.f, 0.f}} };
	BaseMaterial* lambertSphere2{ new Material_Lambert{1.f, RGBColor{0.f, 1.f, 0.f}} };

	BaseMaterial* phongSphere1{ new Material_LambertPhong{1.f, RGBColor{0.68f, 0.85f, 0.9f}, 1.f, 60} };

	//LIGHTS
	PointLight* pLight1{ new PointLight{ FPoint3{0.f, 5.f, -5.f}, RGBColor{1.f, 1.f, 1.f}, 5.f } };
	PointLight* pLight2{ new PointLight{ FPoint3{0.f, 2.5f, 5.f}, RGBColor{1.f, 1.f, 1.f}, 5.f } };
	DirectionalLight* dirLight1{new DirectionalLight{FVector3{.577f, .577f, .577f}, RGBColor{1.f, 1.f, 1.f}, 2.f}};

	//GEOMETRY OBJECTS
	Plane* plane1{ new Plane(FPoint3{0.f, 0.f, 0.f}, FVector3{0.f, 1.f, 0.f}, lambertPlane) };
	Plane* plane2{ new Plane(FPoint3{0.f, 0.f, -10.f}, FVector3{0.f, 0.f, 1.f}, lambertPlane) };
	Plane* plane3{ new Plane(FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.f}, lambertPlane) };
	Plane* plane4{ new Plane(FPoint3{5.f, 0.f, 0.f}, FVector3{-1.f, 0.f, 0.f}, lambertPlane) };
	Plane* plane5{ new Plane(FPoint3{-5.f, 0.f, 0.f}, FVector3{1.f, 0.f, 0.f}, lambertPlane) };
	Sphere* sphere1{ new Sphere(FPoint3{-0.75f, 1.f, 0.f}, 1.f, phongSphere1) };
	Sphere* sphere2{ new Sphere(FPoint3{1.75f, 1.f, 0.f}, 1.f, lambertSphere2) };
	

	//Push everything onto the vectors
	m_pLights.push_back(pLight1);
	m_pLights.push_back(pLight2);
	m_pLights.push_back(dirLight1);

	m_pGeometryObjects.push_back(plane1);
	m_pGeometryObjects.push_back(plane2);
	m_pGeometryObjects.push_back(plane3);
	m_pGeometryObjects.push_back(plane4);
	m_pGeometryObjects.push_back(plane5);
	m_pGeometryObjects.push_back(sphere1);
	m_pGeometryObjects.push_back(sphere2);

	m_pMaterials.push_back(lambertPlane);
	m_pMaterials.push_back(lambertSphere1);
	m_pMaterials.push_back(lambertSphere2);
	m_pMaterials.push_back(phongSphere1);
}


void Scene::InitReference() {

	EmptyGeometryObjects();
	EmptyLights();
	EmptyMaterials();

	//MATERIALS
	BaseMaterial* Lambert_Greyblue{ new Material_Lambert{1.f, RGBColor{0.49f, 0.57f, 0.57f}} };
	BaseMaterial* Lambert_White{ new Material_Lambert{1.f, RGBColor{1.f, 1.f, 1.f}} };

	BaseMaterial* PBR_RoughMetal{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{ 0.972f, 0.96f, 0.915f }, true, 1.0f, true } };
	BaseMaterial* PBR_MediumMetal{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{ 0.972f, 0.96f, 0.915f }, true, 0.6f, true } };
	BaseMaterial* PBR_SmoothMetal{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{ 0.972f, 0.96f, 0.915f }, true, 0.1f, true } };
	BaseMaterial* PBR_RoughPlastic{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{ 0.75f, 0.75f, 0.75f }, false, 1.0f, true } };
	BaseMaterial* PBR_MediumPlastic{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{ 0.75f, 0.75f, 0.75f }, false, 0.4f, true } };
	BaseMaterial* PBR_SmoothPlastic{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{ 0.75f, 0.75f, 0.75f }, false, 0.1f, true } };

	//LIGHTS
	PointLight* pLight1{ new PointLight{ FPoint3{0.f, 5.f, -5.f}, RGBColor{1.f, 0.61f, 0.45f}, 50.f } };
	PointLight* pLight2{ new PointLight{ FPoint3{-2.5f, 5.f, 5.f}, RGBColor{1.f, 0.8f, 0.45f}, 70.f } };
	PointLight* pLight3{ new PointLight{ FPoint3{2.5f, 2.5f, 5.f}, RGBColor{0.34f, 0.47f, .68f}, 50.f } };

	//GEOMETRY OBJECTS
	Plane* plane1{ new Plane(FPoint3{0.f, 0.f, 0.f}, FVector3{0.f, 1.f, 0.f}, Lambert_Greyblue) };
	Plane* plane2{ new Plane(FPoint3{0.f, 0.f, -10.f}, FVector3{0.f, 0.f, 1.f}, Lambert_Greyblue) };
	Plane* plane3{ new Plane(FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.f}, Lambert_Greyblue) };
	Plane* plane4{ new Plane(FPoint3{5.f, 0.f, 0.f}, FVector3{-1.f, 0.f, 0.f}, Lambert_Greyblue) };
	Plane* plane5{ new Plane(FPoint3{-5.f, 0.f, 0.f}, FVector3{1.f, 0.f, 0.f}, Lambert_Greyblue) };
	Sphere* sphere1{ new Sphere(FPoint3{-1.75f, 1.f, 0.f}, 0.75f, PBR_RoughMetal) };
	Sphere* sphere2{ new Sphere(FPoint3{0.f, 1.f, 0.f}, 0.75f, PBR_MediumMetal) };
	Sphere* sphere3{ new Sphere(FPoint3{1.75f, 1.f, 0.f}, 0.75f, PBR_SmoothMetal) };
	Sphere* sphere4{ new Sphere(FPoint3{-1.75f, 3.f, 0.f}, 0.75f, PBR_RoughPlastic) };
	Sphere* sphere5{ new Sphere(FPoint3{0.f, 3.f, 0.f}, 0.75f, PBR_MediumPlastic) };
	Sphere* sphere6{ new Sphere(FPoint3{1.75f, 3.f, 0.f}, 0.75f, PBR_SmoothPlastic) };
	Triangle* triangle1{ new Triangle{ FPoint3{-1.75f, 4.5f, 0.f}, FPoint3{-0.75f, 1.5f, 0}, FPoint3{-0.75f, 0.f, 0.f}, FPoint3{0.75f, 0.f, 0.f}, Cullmode::backSide, Lambert_White} };
	Triangle* triangle2{ new Triangle{ FPoint3{0.f, 4.5f, 0.f}, FPoint3{-0.75f, 1.5f, 0}, FPoint3{-0.75f, 0.f, 0.f}, FPoint3{0.75f, 0.f, 0.f}, Cullmode::frontSide, Lambert_White} };
	Triangle* triangle3{ new Triangle{ FPoint3{1.75f, 4.5f, 0.f}, FPoint3{-0.75f, 1.5f, 0}, FPoint3{-0.75f, 0.f, 0.f}, FPoint3{0.75f, 0.f, 0.f}, Cullmode::doublePrimitive, Lambert_White} };


	//Push everything onto the vectors
	m_pLights.push_back(pLight1);
	m_pLights.push_back(pLight2);
	m_pLights.push_back(pLight3);

	m_pGeometryObjects.push_back(plane1);
	m_pGeometryObjects.push_back(plane2);
	m_pGeometryObjects.push_back(plane3);
	m_pGeometryObjects.push_back(plane4);
	m_pGeometryObjects.push_back(plane5);
	m_pGeometryObjects.push_back(sphere1);
	m_pGeometryObjects.push_back(sphere2);
	m_pGeometryObjects.push_back(sphere3);
	m_pGeometryObjects.push_back(sphere4);
	m_pGeometryObjects.push_back(sphere5);
	m_pGeometryObjects.push_back(sphere6);
	m_pGeometryObjects.push_back(triangle1);
	m_pGeometryObjects.push_back(triangle2);
	m_pGeometryObjects.push_back(triangle3);

	m_pMaterials.push_back(Lambert_Greyblue);
	m_pMaterials.push_back(Lambert_White);
	m_pMaterials.push_back(PBR_RoughMetal);
	m_pMaterials.push_back(PBR_MediumMetal);
	m_pMaterials.push_back(PBR_SmoothMetal);
	m_pMaterials.push_back(PBR_RoughPlastic);
	m_pMaterials.push_back(PBR_MediumPlastic);
	m_pMaterials.push_back(PBR_SmoothPlastic);
}

void Scene::InitTriangles() {

	EmptyGeometryObjects();
	EmptyLights();
	EmptyMaterials();

	//MATERIALS
	BaseMaterial* Lambert_Greyblue{ new Material_Lambert{1.f, RGBColor{0.49f, 0.57f, 0.57f}} };

	//LIGHTS
	PointLight* pLight2{ new PointLight{ FPoint3{0.f, 5.f, 5.f}, RGBColor{1.f, 1.f, 1.f}, 50.f } };

	//GEOMETRY OBJECTS
	Plane* plane1{ new Plane(FPoint3{0.f, 0.f, 0.f}, FVector3{0.f, 1.f, 0.f}, Lambert_Greyblue) };
	Plane* plane2{ new Plane(FPoint3{0.f, 0.f, -10.f}, FVector3{0.f, 0.f, 1.f}, Lambert_Greyblue) };
	Plane* plane3{ new Plane(FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.f}, Lambert_Greyblue) };
	Plane* plane4{ new Plane(FPoint3{5.f, 0.f, 0.f}, FVector3{-1.f, 0.f, 0.f}, Lambert_Greyblue) };
	Plane* plane5{ new Plane(FPoint3{-5.f, 0.f, 0.f}, FVector3{1.f, 0.f, 0.f}, Lambert_Greyblue) };
	Triangle* triangle{ new Triangle{ FPoint3{}, FPoint3{-0.75f, 1.5f, 0}, FPoint3{-0.75f, 0.f, 0.f}, FPoint3{0.75f, 0.f, 0.f}, Cullmode::doublePrimitive, Lambert_Greyblue} };

	//Push everything onto the vectors
	m_pLights.push_back(pLight2);
	m_pGeometryObjects.push_back(plane1);
	m_pGeometryObjects.push_back(triangle);
	m_pMaterials.push_back(Lambert_Greyblue);

}

void Scene::InitPolyBunny() {

	EmptyGeometryObjects();
	EmptyLights();
	EmptyMaterials();

	//MATERIALS
	BaseMaterial* Lambert_Greyblue{ new Material_Lambert{1.f, RGBColor{0.49f, 0.57f, 0.57f}} };
	BaseMaterial* pbr1{ new Material_PBR{1.f, RGBColor{ 1.f, 1.f, 1.f }, 1.f , RGBColor{}, false, 1.f, true } };

	//LIGHTS
	PointLight* pLight1{ new PointLight{ FPoint3{0.f, 5.f, -5.f}, RGBColor{1.f, 0.61f, 0.45f}, 50.f } };
	PointLight* pLight2{ new PointLight{ FPoint3{-2.5f, 5.f, 5.f}, RGBColor{1.f, 0.8f, 0.45f}, 70.f } };
	PointLight* pLight3{ new PointLight{ FPoint3{2.5f, 2.5f, 5.f}, RGBColor{0.34f, 0.47f, .68f}, 50.f } };

	//GEOMETRY OBJECTS
	Plane* plane1{ new Plane(FPoint3{0.f, 0.f, 0.f}, FVector3{0.f, 1.f, 0.f}, Lambert_Greyblue) };
	Plane* plane2{ new Plane(FPoint3{0.f, 0.f, -10.f}, FVector3{0.f, 0.f, 1.f}, Lambert_Greyblue) };
	Plane* plane3{ new Plane(FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.f}, Lambert_Greyblue) };
	Plane* plane4{ new Plane(FPoint3{5.f, 0.f, 0.f}, FVector3{-1.f, 0.f, 0.f}, Lambert_Greyblue) };
	Plane* plane5{ new Plane(FPoint3{-5.f, 0.f, 0.f}, FVector3{1.f, 0.f, 0.f}, Lambert_Greyblue) };
	TriangleMesh* bunny{ new TriangleMesh{ FPoint3{}, "Bunny/lowpoly_bunny.obj", Cullmode::doublePrimitive, pbr1, true} };

	//Push everything onto the vectors
	m_pLights.push_back(pLight1);
	m_pLights.push_back(pLight2);
	m_pLights.push_back(pLight3);

	m_pGeometryObjects.push_back(plane1);
	m_pGeometryObjects.push_back(plane2);
	m_pGeometryObjects.push_back(plane3);
	m_pGeometryObjects.push_back(plane4);
	m_pGeometryObjects.push_back(plane5);
	m_pGeometryObjects.push_back(bunny);

	m_pMaterials.push_back(Lambert_Greyblue);
	m_pMaterials.push_back(pbr1);
}

void Scene::ToggleShadows() {
	if (m_Shadows) {
		m_Shadows = false;
	}
	else {
		m_Shadows = true;
	}
}

void Scene::ToggleLET() {
	switch (m_LET)
	{
	case LightEquationTerm::both:
		m_LET = LightEquationTerm::IrrOnly;
		break;
	case LightEquationTerm::IrrOnly:
		m_LET = LightEquationTerm::BRDFOnly;
		break;
	case LightEquationTerm::BRDFOnly:
		m_LET = LightEquationTerm::both;
		break;
	}

}

bool Scene::GetShading() {
	return m_Shadows;
}

int Scene::GetIrradiance() {
	if (m_LET == LightEquationTerm::both || m_LET == LightEquationTerm::IrrOnly) {
		return 1;
	}
	else {
		return 0;
	}
}

int Scene::GetBRDF() {
	if (m_LET == LightEquationTerm::both || m_LET == LightEquationTerm::BRDFOnly) {
		return 1;
	}
	else {
		return 0;
	}
}

void Scene::EmptyGeometryObjects() {
	m_pGeometryObjects.clear();
}

void Scene::EmptyLights() {
	m_pLights.clear();
}

void Scene::EmptyMaterials() {
	m_pMaterials.clear();
}