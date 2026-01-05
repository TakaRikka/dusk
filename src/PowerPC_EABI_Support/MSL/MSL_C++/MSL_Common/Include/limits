#ifndef _MSL_LIMITS_H
#define _MSL_LIMITS_H

#include <climits>
#include <float.h>

namespace std {
template <typename T>
class numeric_limits {
public:
	inline static T min();
	inline static T max();
};

template <>
class numeric_limits<char> {
public:
	inline static char min() { return -0x80; }
	inline static char max() { return 0x7F; }
};

template <>
class numeric_limits<short> {
public:
	inline static short min() { return -0x8000; }
	inline static short max() { return 0x7FFF; }
};

template <>
class numeric_limits<int> {
public:
	inline static int min() { return -0x80000000; }
	inline static int max() { return 0x7FFFFFFF; }
};

template <>
class numeric_limits<long> {
public:
	inline static long min() { return -0x80000000; }
	inline static long max() { return 0x7FFFFFFF; }
};

template <>
class numeric_limits<unsigned char> {
public:
	inline static unsigned char min() { return 0x0; }
	inline static unsigned char max() { return 0xFF; }
};

template <>
class numeric_limits<unsigned short> {
public:
	inline static unsigned short min() { return 0x0; }
	inline static unsigned short max() { return 0xFFFF; }
};

template <>
class numeric_limits<unsigned int> {
public:
	inline static unsigned int min() { return 0x0; }
	inline static unsigned int max() { return 0xFFFFFFFF; }
};

template <>
class numeric_limits<unsigned long> {
public:
	inline static unsigned long min() { return 0x0; }
	inline static unsigned long max() { return 0xFFFFFFFF; }
};

template <>
class numeric_limits<float> {
public:
	inline static float min();
	inline static float max() { return FLT_MAX; }
	inline static float signaling_NaN() { return *(float*)__float_nan; }
};

#if __REVOLUTION_SDK__
template <>
class numeric_limits<double> {
public:
	static const unsigned long long x = 0x7ff0000000000001ULL;
	inline static double min();
	inline static double max();
	inline static double signaling_NaN() { return *reinterpret_cast<const double*>(&x); }
};
#endif

} // namespace std

#endif
