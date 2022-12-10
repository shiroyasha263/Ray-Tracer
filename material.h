#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "texture.h"

struct hit_record;

class material {
public:
	// This =0 makes the virtual function pure virtual
	// Which kinda means that the function is a prototype to be overriden and defined in subclasses
	virtual color emitted(
		double u, double v, const point3& p
	) const {
		return color(0, 0, 0);
	}
	virtual bool scatter(
		const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered
	) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
	lambertian(shared_ptr<texture> a) : albedo(a) {}
	
	virtual bool scatter(
		const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		auto scatter_direction = random_in_hemisphere(rec.normal);

		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction, ray_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	shared_ptr<texture> albedo;
};

class metal : public material {
public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		auto reflected_direction = reflect(ray_in.direction() + fuzz * random_in_hemisphere(ray_in.direction()), rec.normal);

		scattered = ray(rec.p, reflected_direction, ray_in.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz;
};

class dielectric : public material {
public:

	dielectric(double index_of_refraction) : ir(index_of_refraction){}

	virtual bool scatter(
		const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1 / ir) : ir;

		vec3 unit_direction = unit_vector(ray_in.direction());

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sine_theta = sqrt(1 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sine_theta > 1.0;

		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, direction, ray_in.time());
		return true;
	}

private:
	static double reflectance(double cos_theta, double refraction_ratio) {
		auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cos_theta), 5);
	}

public:
	double ir;
};

class diffuse_light : public material {
public:
	diffuse_light(shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	)const override {
		return false;
	}

	virtual color emitted(double u, double v, const point3& p) const override {
		return emit->value(u, v, p);
	}
public:
	shared_ptr<texture> emit;
};

#endif