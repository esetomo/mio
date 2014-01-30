#include <mio/curve/curve.h>

#define CURVE_IMPLEMENTATION(ValueType, ValueTypeName) \
	Curve_##ValueTypeName::Curve_##ValueTypeName(const char *uid) : MioObject(uid) \
	{ \
	} \
	\
	Curve_##ValueTypeName::~Curve_##ValueTypeName() \
	{ \
	}

namespace mio
{
	namespace curve
	{
		CURVE_IMPLEMENTATION(float, float)
		CURVE_IMPLEMENTATION(Vector3, Vector3)
		CURVE_IMPLEMENTATION(Quaternion, Quaternion)
		CURVE_IMPLEMENTATION(mio::rigging::JointChange, JointChange)
	}
}