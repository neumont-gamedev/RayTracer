#include "Material.h"
#include "Math.h"

bool Lambertian::Scatter(const ray& r, const raycastHit& hit, glm::vec3& attenuation, ray& scattered) const
{
    glm::vec3 target = hit.point + hit.normal + randomInUnitSphere();
    glm::vec3 direction = glm::normalize(target - hit.point);

    scattered = ray{ hit.point, direction };
    attenuation = m_albedo;

    return true;
}

bool Metal::Scatter(const ray& r, const raycastHit& hit, glm::vec3& attenuation, ray& scattered) const
{
    glm::vec3 reflected = reflect(glm::normalize(r.direction), hit.normal);
    scattered = ray(hit.point, reflected + m_fuzz * randomInUnitSphere());
    attenuation = m_albedo;

    return (glm::dot(scattered.direction, hit.normal) > 0);
}

bool Dielectric::Scatter(const ray& r, const raycastHit& hit, glm::vec3& attenuation, ray& scattered) const
{
	glm::vec3 outward_normal;

	glm::vec3 reflected = reflect(glm::normalize(r.direction), hit.normal);
	// Attenuation 1 means that it doesn't absorb anything.
	attenuation = glm::vec3(1, 1, 1);

	glm::vec3 refracted;
	float reflect_prob;
	float cosine;
	float ni_over_nt;
	// When ray shoot through object back into vacuum,
	// ni_over_nt = ref_idx, surface normal has to be inverted.
	if (glm::dot(r.direction, hit.normal) > 0)
	{
		outward_normal = -hit.normal;
		ni_over_nt = m_index;
		cosine = m_index * glm::dot(r.direction, hit.normal) / r.direction.length();
	}
	else
	{
		// when ray shoots into object, ni_over_nt = 1 / ref_idx.
		outward_normal = hit.normal;
		ni_over_nt = 1.0f / m_index;
		cosine = -glm::dot(r.direction, hit.normal) / r.direction.length();
	}

	// Dealing with Refraction/ Reflection
	if (refract(r.direction, outward_normal, ni_over_nt, refracted))
	{
		reflect_prob = schlick(cosine, m_index);
	}
	else
	{
		reflect_prob = 1.0;
	}

	if (random01() < reflect_prob)
	{
		scattered = ray{ hit.point, reflected };
	}
	else
	{
		scattered = ray{ hit.point, refracted };
	}

	return true;
}
