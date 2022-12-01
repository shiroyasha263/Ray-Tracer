#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

//Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions

inline double degrees_to_radians(double degree) {
	return degree * pi / 180;
}

inline double random_double() {
	//Returns a random real [0,1)
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	//Returns a number between [min, max)
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
	if (x > max) return max;
	if (x < min) return min;
	return x;
}

//Common Headers

#include "ray.h"
#include "vec3.h"

#endif