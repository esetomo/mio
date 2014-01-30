#include <mio/ik/ik_end_effector.h>

namespace mio
{
    namespace ik
    {
		IKEndEffector::IKEndEffector(const char *name, int index) : ReferenceCounted()
		{
			name_ = name;
			index_ = index;
			name_ = name;
		}

		IKEndEffector::IKEndEffector(const IKEndEffector &other) : ReferenceCounted()
		{
			name_ = other.name_;
			index_ = other.index_;
			name_ = other.name_;
			ik_joint_indices_.adjust_size(other.ik_joint_indices_.size(), false);
			FOR(i, other.ik_joint_indices_.size())
				ik_joint_indices_[i] = other.ik_joint_indices_[i];
		}

		IKEndEffector::~IKEndEffector()
		{
		}
    }
}
