#pragma once
#include <limits>
#include <algorithm>

// sources:
// 1. http://realtimecollisiondetection.net/blog/?p=89 
// 2. https://bitbashing.io/comparing-floats.html
// 3. https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
// 4. https://stackoverflow.com/questions/67842444/modern-practice-to-compare-double-float-for-equality-in-modern-c

namespace lal 
{
	constexpr float defaultEpsilonRel {0.0001};
	constexpr float defaultEpsilonAbs {std::numeric_limits<float>::epsilon()};

	inline bool floatEqualAbs(float a, float b, float epsilonAbs){
		return std::abs(a - b) <= epsilonAbs;
	}

	inline bool floatEqualAbs(float a, float b){
		return floatEqualAbs(a, b, defaultEpsilonAbs);
	}

	inline bool floatEqualRel(float a, float b, float epsilonRel){
		return std::abs(a - b) <= std::max({1.0f, std::abs(a), std::abs(b)}) * epsilonRel;
	}

	inline bool floatEqualRel(float a, float b){
		return floatEqualRel(a, b, defaultEpsilonRel);
	}

	inline bool floatEqual(float a, float b, float epsilonRel, float epsilonAbs){
		return floatEqualAbs(a, b, epsilonAbs) || floatEqualRel(a, b, epsilonRel);
	}

	inline bool floatEqual(float a, float b){
		return floatEqualAbs(a, b, defaultEpsilonAbs) || floatEqualRel(a, b, defaultEpsilonRel);
	}
}
