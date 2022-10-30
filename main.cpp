#include"color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <math.h>

color ray_color(const ray& r)
{
	vec3 unit_direction = unit_vector(r.direction());
	auto dy = 0.5 * (unit_direction.y() + 1.0);
	auto dx = 0.5 * (unit_direction.x() + 1.0);

	vec3 final_color =	(dy * color(1.0, 1.0, 1.0) + (1.0 - dy) * color(0.5, 0.7, 1.0)) *
						(dx * color(1.0, 1.0, 1.0) + (1.0 - dx) * color(1.0, 0.7, 0.5));

	// Blending the two colors using a simple linear lerp i guess depending on y
	return final_color;
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

	for (int j = 0; j < img_height; ++j)
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