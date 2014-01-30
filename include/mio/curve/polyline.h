#ifndef __MIO_CURVE_POLYLINE_H__
#define __MIO_CURVE_POLYLINE_H__

#include <map>
#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/quaternion.h>
#include <mio/reference.h>
#include <mio/curve/curve.h>

#define POLYLINE_HEADER(ValueType, ValueTypeName) \
	class MIO_API Polyline_##ValueTypeName : public Curve_##ValueTypeName \
	{ \
	private: \
		std::map< float, ValueType > control_points_; \
		friend class PolylineControlPointIterator_##ValueTypeName; \
		\
	public: \
		Polyline_##ValueTypeName(const char *uid=""); \
		virtual ~Polyline_##ValueTypeName(); \
		\
		ValueType zero() const; \
		ValueType interpolate(const ValueType &a, const ValueType &b, float x) const; \
		virtual ValueType evaluate(float time) const; \
		virtual float get_range_lower_bound() const; \
		virtual float get_range_upper_bound() const; \
		int get_control_point_count() const; \
		void set_control_point(float time, const ValueType &value); \
		mio::curve::PolylineControlPointIterator_##ValueTypeName get_control_point_iterator(); \
	}; \
	\
	struct MIO_API PolylineControlPoint_##ValueTypeName \
	{ \
		float time; \
		ValueType value; \
		\
		PolylineControlPoint_##ValueTypeName(float _time, ValueType &_value) \
		{ \
			time = _time; \
			value = _value; \
		} \
	}; \
	\
	class MIO_API PolylineControlPointIterator_##ValueTypeName \
	{ \
	private: \
		Reference< Polyline_##ValueTypeName > polyline_; \
		std::map< float, ValueType >::iterator current_; \
	public: \
		PolylineControlPointIterator_##ValueTypeName(); \
		PolylineControlPointIterator_##ValueTypeName(Polyline_##ValueTypeName *polyline); \
		virtual ~PolylineControlPointIterator_##ValueTypeName(); \
		bool has_next(); \
		mio::curve::PolylineControlPoint_##ValueTypeName get_next(); \
		mio::curve::PolylineControlPoint_##ValueTypeName peek(); \
	};

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace curve
    {
		POLYLINE_HEADER(float, float)
		POLYLINE_HEADER(Vector3, Vector3)
		POLYLINE_HEADER(Quaternion, Quaternion)
		POLYLINE_HEADER(mio::rigging::JointChange, JointChange)
    }
}

#pragma warning(pop)

#endif
