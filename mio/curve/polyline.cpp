#include <mio/curve/polyline.h>

#define POLYLINE_BODY(ValueType, ValueTypeName) \
	Polyline_##ValueTypeName::Polyline_##ValueTypeName(const char *uid) : Curve_##ValueTypeName(uid) \
	{ \
	} \
	\
	Polyline_##ValueTypeName::~Polyline_##ValueTypeName() \
	{ \
	} \
	\
	ValueType Polyline_##ValueTypeName::evaluate(float time) const \
	{ \
		if (control_points_.size() == 0) \
			return zero(); \
		else if (control_points_.size() == 1) \
			return control_points_.begin()->second; \
		else \
		{ \
			std::map< float, ValueType >::const_iterator left = control_points_.lower_bound(time); \
			std::map< float, ValueType >::const_iterator right = control_points_.upper_bound(time); \
			std::map< float, ValueType >::const_iterator end = control_points_.end(); \
			std::map< float, ValueType >::const_iterator begin = control_points_.begin(); \
			\
			if (left != right) \
				return left->second; \
			else if (left == begin) \
				return control_points_.begin()->second; \
			else if (left == end) \
				return control_points_.rbegin()->second; \
			else \
			{ \
				--left; \
				float x = (time - left->first) / (right->first - left->first); \
				return interpolate(left->second, right->second, x); \
			} \
		} \
	} \
	\
	int Polyline_##ValueTypeName::get_control_point_count() const \
	{ \
		return (int)control_points_.size(); \
	} \
	\
	void Polyline_##ValueTypeName::set_control_point(float time, const ValueType &value) \
	{ \
		control_points_[time] = value; \
	} \
	\
	mio::curve::PolylineControlPointIterator_##ValueTypeName Polyline_##ValueTypeName::get_control_point_iterator() \
	{ \
		return PolylineControlPointIterator_##ValueTypeName(this); \
	} \
	\
	float Polyline_##ValueTypeName::get_range_lower_bound() const \
	{ \
		if (control_points_.size() == 0) \
			return INFINITY; \
		else \
			return control_points_.begin()->first; \
	} \
	\
	float Polyline_##ValueTypeName::get_range_upper_bound() const \
	{ \
		if (control_points_.size() == 0) \
			return -INFINITY; \
		else \
			return control_points_.rbegin()->first; \
	} \
	\
	PolylineControlPointIterator_##ValueTypeName::PolylineControlPointIterator_##ValueTypeName() \
	{ \
	} \
	\
	PolylineControlPointIterator_##ValueTypeName::PolylineControlPointIterator_##ValueTypeName(Polyline_##ValueTypeName *polyline) \
	{ \
		polyline_ = polyline; \
		current_ = polyline_->control_points_.begin(); \
	} \
	\
	PolylineControlPointIterator_##ValueTypeName::~PolylineControlPointIterator_##ValueTypeName() \
	{ \
	} \
	\
	bool PolylineControlPointIterator_##ValueTypeName::has_next() \
	{ \
		return current_ != polyline_->control_points_.end(); \
	} \
	\
	mio::curve::PolylineControlPoint_##ValueTypeName PolylineControlPointIterator_##ValueTypeName::get_next() \
	{ \
		PolylineControlPoint_##ValueTypeName result(current_->first, current_->second); \
		++current_; \
		return result; \
	} \
	\
	mio::curve::PolylineControlPoint_##ValueTypeName PolylineControlPointIterator_##ValueTypeName::peek() \
	{ \
		return PolylineControlPoint_##ValueTypeName(current_->first, current_->second); \
	}

namespace mio
{
    namespace curve
    {
		POLYLINE_BODY(float, float)
		POLYLINE_BODY(Vector3, Vector3)
		POLYLINE_BODY(Quaternion, Quaternion)
		POLYLINE_BODY(mio::rigging::JointChange, JointChange)

		float Polyline_float::zero() const
		{
			return 0.0f;
		}

		float Polyline_float::interpolate(const float &a, const float &b, float x) const
		{
			return (1-x)*a + x*b;
		}

		Vector3 Polyline_Vector3::zero() const
		{
			return Vector3(0,0,0);
		}

		Vector3 Polyline_Vector3::interpolate(const Vector3 &a, const Vector3 &b, float x) const
		{
			return a*(1-x) + b*x;
		}

		Quaternion Polyline_Quaternion::zero() const
		{
			return Quaternion();
		}

		Quaternion Polyline_Quaternion::interpolate(const Quaternion &a, const Quaternion &b, float x) const
		{
			return slerp(a, b, x);
		}

		mio::rigging::JointChange Polyline_JointChange::zero() const
		{
			return mio::rigging::JointChange(Vector3(0,0,0), Quaternion());
		}

		mio::rigging::JointChange Polyline_JointChange::interpolate(
			const mio::rigging::JointChange &a, 
			const mio::rigging::JointChange &b, 
			float x) const
		{
			Vector3 position = a.position*(1-x) + b.position*x;
			Quaternion orientation = slerp(a.orientation, b.orientation, x);
			return mio::rigging::JointChange(position, orientation);
		}
	}
}
