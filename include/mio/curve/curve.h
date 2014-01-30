#ifndef __MIO_CURVE_CURVE_H__
#define __MIO_CURVE_CURVE_H__

#include <mio/mio.h>
#include <mio/mio_object.h>
#include <mio/vector3.h>
#include <mio/quaternion.h>
#include <mio/rigging/joint_change.h>

#define CURVE_HEADER(ValueType, ValueTypeName) \
	class MIO_API Curve_##ValueTypeName : public MioObject\
	{ \
	public: \
		Curve_##ValueTypeName(const char *uid=""); \
		virtual ~Curve_##ValueTypeName(); \
		\
		virtual ValueType evaluate(float time) const = 0; \
		virtual float get_range_lower_bound() const = 0; \
		virtual float get_range_upper_bound() const = 0; \
		\
		ValueType operator()(float time) const \
		{ \
			return evaluate(time); \
		} \
	}; 
	

namespace mio
{
    namespace curve
    {   
		CURVE_HEADER(float, float);
		CURVE_HEADER(Vector3, Vector3);
		CURVE_HEADER(Quaternion, Quaternion);		
		CURVE_HEADER(mio::rigging::JointChange, JointChange);
    }
}

#endif
