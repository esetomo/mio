#include <mio/rigging/pose.h>

namespace mio
{
    namespace rigging
    {
        Pose::Pose()
        {
        }

		Pose::Pose(const Pose &other)
		{
			map<std::string, JointChange>::const_iterator ii = other.joint_changes_by_name_.begin();
			while (ii != other.joint_changes_by_name_.end())
			{
				joint_changes_by_name_[ii->first] = ii->second;
				++ii;
			}
		}

        Pose::~Pose()
        {
        }

		NameJointChangePairIterator::NameJointChangePairIterator()
		{
			pose_ = NULL;
		}

		NameJointChangePairIterator::NameJointChangePairIterator(Pose *pose)
		{
			pose_ = pose;
			current_ = pose->joint_changes_by_name_.begin();
		}

		NameJointChangePairIterator::~NameJointChangePairIterator()
		{
		}

		bool NameJointChangePairIterator::has_next()
		{
			return current_ != pose_->joint_changes_by_name_.end();
		}

		NameJointChangePair NameJointChangePairIterator::get_next()
		{
			NameJointChangePair result(current_->first.c_str(), current_->second);
			++current_;
			return result;
		}

		mio::rigging::NameJointChangePairIterator Pose::get_iterator()
		{
			return NameJointChangePairIterator(this);
		}
    }
}
