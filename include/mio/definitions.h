#ifndef __MIO_DEFINITIONS_H__
#define __MIO_DEFINITIONS_H__

#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <math.h>
#include <float.h>

#define M_PI 3.14159265358979323846f
#define INV_PI 0.31830988618379067154f
#define INV_TWOPI 0.15915494309189533577f
#define INV_FOURPI 0.079577471545947673f

#define RAY_EPSILON 1e-4f
#define EPSILON 1e-5f

#define INFINITY FLT_MAX

#define FOR(__i,__times) for(int __i=0;__i<__times;__i++)
#define FOR2(__i,__from,__to) for(int __i=__from;__i<=__to;__i++)
#define FOR3(__i,__from,__to) for(int __i=__from;__i<__to;__i++)

namespace mio
{
	inline float lerp(float t, float v1, float v2) {
		return (1.f - t) * v1 + t * v2;
	}

	inline float clamp(float val, float low, float high) {
		if (val < low) return low;
		else if (val > high) return high;
		else return val;
	}

	inline int clamp(int val, int low, int high) {
		if (val < low) return low;
		else if (val > high) return high;
		else return val;
	}

	inline int mod(int a, int b) {
		int n = int(a/b);
		a -= n*b;
		if (a < 0)
			a += b;
		return a;
	}

	inline float radians(float deg) {
		return ((float)M_PI/180.f) * deg;
	}

	inline float degrees(float rad) {
		return (180.f/(float)M_PI) * rad;
	}

	inline float randf()
	{
		return (float)rand() / RAND_MAX;
	}

	inline float random(float lower=0.0, float upper=1.0, int granularity=RAND_MAX-1)
	{
		float width = (upper - lower) / granularity;
		return lower + width * (rand() % (granularity+1));
	}

	inline int ceilpow2(int x)
	{
		if (x < 0) return 1;
		int ans = 1;
		while (ans < x)
			ans = ans << 1;
		return ans;
	}

	// Compute ceil(dividend/divisor).
	inline int divceil(int dividend, int divisor)
	{
		if (dividend <= 0) return 0;
		return (dividend-1) / divisor + 1;
	}
}

#endif