#pragma once

#include "vec.h"

namespace Shader
{
	// Returns an RGB value for the specified fragment.
	vec::vec3 mainImage(vec::vec2 fragCoord, vec::vec2 resolution);

	vec::vec2 mandelbrotMap(vec::vec2 fragCoord, vec::vec2 resolution);

	vec::vec3 colorMandelbrotMap(vec::vec2 map);
}
