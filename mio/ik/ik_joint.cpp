#include <mio/ik/ik_joint.h>

namespace mio
{
    namespace ik
    {
		IKJoint::IKJoint(const char *name, int index) : ReferenceCounted()
		{
			index_ = index;
			name_ = name;
			rotation_axis_to_joint_ = Quaternion();
			FOR(i, IK_JOINT_PARAMETER_COUNT)
			{
				stiffness_[i] = 1.0f;
				parameters_[i] = 0.0f;
				lower_bound_[i] = -INFINITY;
				upper_bound_[i] = INFINITY;
				enabled_[i] = true;
			}
		}

		IKJoint::IKJoint(const IKJoint &other) : ReferenceCounted()
		{
			index_ = other.index_;
			name_ = other.name_;
			rotation_axis_to_joint_ = other.rotation_axis_to_joint_;
			FOR(i, IK_JOINT_PARAMETER_COUNT)
			{
				stiffness_[i] = other.stiffness_[i];
				parameters_[i] = other.parameters_[i];
				lower_bound_[i] = other.lower_bound_[i];
				upper_bound_[i] = other.upper_bound_[i];
				enabled_[i] = true;
			}
		}

		IKJoint::~IKJoint()
		{
		}
    }
}
