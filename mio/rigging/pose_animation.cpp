#include <mio/rigging/pose_animation.h>
#include <mio/curve/polyline.h>

namespace mio
{
    namespace rigging
    {
		PoseAnimation::PoseAnimation(const char *uid) : MioObject(uid)        
		{
			range_lower_bound = INFINITY;
			range_upper_bound = -INFINITY;
        }

        PoseAnimation::~PoseAnimation()
        {
        }
		
		mio::curve::Curve_JointChange *PoseAnimation::get_joint_curve(const char *joint_name)
		{
			std::map< std::string, Reference<mio::curve::Curve_JointChange> >::iterator ii =
				joint_change_curves_.find(joint_name);
			if (ii == joint_change_curves_.end())
				throw mio::IndexException("PoseAnimation::get_joint_curve() : there is no curve for the joint with the given name");
			else
				return joint_change_curves_[joint_name].get_pointer();
		}

		void PoseAnimation::set_joint_curve(const char *joint_name, mio::curve::Curve_JointChange *joint_curve)
		{
			Reference< mio::curve::Curve_JointChange > ref = joint_curve;
			joint_change_curves_[joint_name] = ref;
		}

		void PoseAnimation::get_pose(mio::rigging::Pose &pose,  float time, bool clear_data) const
		{
			if (clear_data)
				pose.clear();
			std::map< std::string, Reference<mio::curve::Curve_JointChange> >::const_iterator ii =
				joint_change_curves_.begin();
			while (ii != joint_change_curves_.end())
			{
				JointChange joint_change = ii->second->evaluate(time);
				pose.set_joint_change(ii->first.c_str(), joint_change);
				++ii;
			}
		}

		mio::rigging::Pose *PoseAnimation::get_pose(float time) const
		{
			mio::rigging::Pose *pose = new mio::rigging::Pose();
			get_pose(*pose, time);
			return pose;
		}

		void PoseAnimation::update_range() const
		{
			std::map< std::string, Reference<mio::curve::Curve_JointChange> >::const_iterator ii =
				joint_change_curves_.begin();
			while (ii!= joint_change_curves_.end())
			{
				float lower_bound = ii->second->get_range_lower_bound();
				float upper_bound = ii->second->get_range_upper_bound();
				if (lower_bound < range_lower_bound)
					range_lower_bound = lower_bound;
				if (upper_bound > range_upper_bound)
					range_upper_bound = upper_bound;
				++ii;
			}
		}

		float PoseAnimation::get_range_lower_bound() const
		{
			return range_lower_bound;
		}

		float PoseAnimation::get_range_upper_bound() const
		{
			return range_upper_bound;
		}
    }
}
