#ifndef __MANHATTAN_CORE_MATERIALS_HPP__
#define __MANHATTAN_CORE_MATERIALS_HPP__

#include "Ray.hpp"
#include "Color.hpp"

class Material 
{
public:
	virtual ~Material() = default;
	virtual bool Scatter(const Ray& inRay, const Payload& payload, Color& attenuation, Ray& scattered) const { return false; }
};

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo) : m_Albedo(albedo) {}

	bool Scatter(const Ray& inRay, const Payload& payload, Color& attenuation, Ray& scattered) const override {
		auto scatterDir = payload.m_Normal + RandomUnitVector3();
		if (scatterDir.IsNearZero())
			scatterDir = payload.m_Normal;

		scattered = Ray(payload.m_HitPoint, scatterDir);
		attenuation = m_Albedo;
		return true;
	}

private:
	Color m_Albedo;
};

class Metal : public Material {
public:
	Metal(const Color& albedo, float fuzz) : m_Albedo(albedo), m_Fuzz(fuzz) {}

	bool Scatter(const Ray& inRay, const Payload& payload, Color& attenuation, Ray& scattered) const override {
		auto reflected = Reflect(inRay.Direction(), payload.m_Normal);
		reflected = Normalize(reflected) + (m_Fuzz * RandomUnitVector3());
		scattered = Ray(payload.m_HitPoint, reflected);
		attenuation = m_Albedo;

		return (Dot(scattered.Direction(), payload.m_Normal) > 0);
	}

private:
	Color m_Albedo;
	float m_Fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float refractionIndex) : m_RefractionIndex(refractionIndex) {}

	bool Scatter(const Ray& inRay, const Payload& payload, Color& attenuation, Ray& scattered) const override {
		attenuation = Color(1, 1, 1);
		float ri = payload.m_IsFrontFace ? (1.0 / m_RefractionIndex) : m_RefractionIndex;

		Vector3 unitDir = Normalize(inRay.Direction());

		float cosTheta = std::fmin(Dot(-unitDir, payload.m_Normal), 1.0);
		float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		bool isCannotRefract = ri * sinTheta > 1.0;
		Vector3 direction;

		if (isCannotRefract || Reflectance(cosTheta, ri) > RNG::Get1D())
			direction = Reflect(unitDir, payload.m_Normal);
		else
			direction = Refract(unitDir, payload.m_Normal, ri);

		scattered = Ray(payload.m_HitPoint, direction);
		return true;
	}

private:
	static float Reflectance(float cosine, float refractionIndex) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}

	float m_RefractionIndex;
};





#endif

