#ifndef __MIO_RIGGING_POSE_ANIMATION_H__
#define __MIO_RIGGING_POSE_ANIMATION_H__

#include <map>
#include <mio/mio.h>
#include <mio/mio_object.h>
#include <mio/reference.h>
#include <mio/curve/curve.h>
#include <mio/rigging/pose.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace rigging
    {
		class MIO_API PoseAnimation : public MioObject
        {
		private:
			std::map< std::string, Reference<mio::curve::Curve_JointChange> > joint_change_curves_;
			mutable float range_lower_bound;
			mutable float range_upper_bound;

        public:
            PoseAnimation(const char *uid="");
            virtual ~PoseAnimation();

			mio::curve::Curve_JointChange *get_joint_curve(const char *joint_name);

			void set_joint_curve(const char *joint_name, mio::curve::Curve_JointChange *joint_curve);

			void get_pose(mio::rigging::Pose &pose, float time, bool clear_data=false) const;
			mio::rigging::Pose *get_pose(float time) const;

			void update_range() const;
			float get_range_lower_bound() const;
			float get_range_upper_bound() const;
        };
    }
}

#pragma warning(push)

#endif
