#include"color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <math.h>

double hit_sphere(const point3& center, const double radius, const ray& r)
{
	vec3 oc = r.orig - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return -1.0;
	else
		return (-half_b - sqrt(discriminant)) / (a);
}

color ray_color(const ray& r)
{
	// Get the point at the sphere to shade it
	auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);

	// Blending the two colors using a simple linear lerp i guess depending on y
	return (1.0 - t) * color(1.0, 1.0, 1.0) + (t) * color(0.5, 0.7, 1.0);
}

int main()
{
	// Image

	// Creates the size of the image
	const auto aspect_ratio = 16.0 / 9;
	const int img_width = 400;
	const int img_height = static_cast<int>(img_width / aspect_ratio);

	// Camera
	// Creates the values of the viewport (can be anything to be honest but we went with this height = 2, at distance of 2 from the centre)
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	//creates the origin
	auto origin = point3();
	//creates the horizontal vector that is the length of the viewport
	auto horizontal = vec3(viewport_width, 0, 0);
	//creates the vertical vector that is the length of the viewport
	auto vertical = vec3(0, viewport_height, 0);
	//creates the vector that points at the lower left corner of our view port
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	// Render

	std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

	for (int j = img_height; j >= 0; --j)
	{
		std::cerr << "\r Scanline remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < img_width; ++i)
		{
			auto u = double(i) / (img_width - 1);
			auto v = double(j) / (img_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);

			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\n Done. \n";
}