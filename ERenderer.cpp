//External includes
#include "SDL.h"
#include "SDL_surface.h"

#include "omp.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"
//#include "Scene.h"
#include "Ray.h"
#include "Camera.h"
#include "Light.h"

Elite::Renderer::Renderer(SDL_Window * pWindow)
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = static_cast<uint32_t*>(m_pBackBuffer->pixels);
	//m_Scene.InitWeek1();//plane and 2 spheres
	//m_Scene.InitReference(); // Plane and 6 sphere
	//m_Scene.InitTriangles(); //triangles
	//m_Scene.InitPolyBunny();
}

void Elite::Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);
	Camera* camera{ m_Scene.GetCamera() };
	m_Scene.Update();
	const float aspectRatio{ static_cast<float>(m_Width) / static_cast<float>(m_Height) };
	const float fov{ tan(camera->GetFOV() / 2) };
	RGBColor largestColorValue{};

	//Loop over all the pixels
	#pragma omp parallel for
	for (int r = 0; r < m_Height; ++r)
	{
		for (int c = 0; c < m_Width; ++c)
		{
			m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(0),
				static_cast<uint8_t>(0),
				static_cast<uint8_t>(0));
			float xss{ (2 * (c + 0.5f) / m_Width - 1) * aspectRatio * fov};
			float yss{ (1 - 2 * (r + 0.5f) / m_Height) * fov };
			FPoint4 screenPos{ xss, yss, -1.f, 1.0f };
			FPoint4 rayOrigin{ camera->GetONB() * screenPos };
			FVector3 rayDirection{ rayOrigin - FPoint4{camera->GetPos(), 0} };
			Normalize(rayDirection);
			Ray ray{ rayOrigin.xyz, rayDirection };
			HitRecord hitRecord{};
			RGBColor finalColor{};
			if (m_Scene.Hit(ray, hitRecord)) {
				std::vector<Light*> lights{ m_Scene.GetLights() };
				for (Light* light : lights) {
					if (light->IsLightOn()) {
						FVector3 lightVector{ light->GetDirection(hitRecord) };
						FVector3 pointVector{ light->GetTowardPointDirection(hitRecord) };
						Normalize(lightVector);
						Normalize(pointVector);
						float dotProd{ Dot(hitRecord.normal, lightVector) };
						if (dotProd >= 0.00001f) {
							Ray shadowRay{ hitRecord.hitPoint, lightVector };
							shadowRay.tMax = Elite::Magnitude(light->GetDirection(hitRecord)); // don't check further than the light
							shadowRay.tMin = 0.1f;

							RGBColor rad{ light->Irradiance(hitRecord) };
							bool shading{ m_Scene.GetShading() };
							int Irr{ m_Scene.GetIrradiance() };
							int BRDF{ m_Scene.GetBRDF() };
							if (!m_Scene.Hit(shadowRay, hitRecord, true) && shading) {
								//convert RGBColor to neutral element or not
								finalColor += rad.Pow(Irr) * dotProd * (hitRecord.material->Shade(hitRecord, lightVector, -ray.direction).Pow(BRDF));
							}
							else if(!shading) {
								//convert RGBColor to neutral element or not
								finalColor += rad.Pow(Irr) * dotProd * (hitRecord.material->Shade(hitRecord, lightVector, -ray.direction).Pow(BRDF));
							}
						}
					}
				}
			}
			finalColor.MaxToOne();
			m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255.f),
				static_cast<uint8_t>(finalColor.g * 255.f),
				static_cast<uint8_t>(finalColor.b * 255.f));
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);

}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}
