#ifndef __MIO_IK_IK_POSE_H__
#define __MIO_IK_IK_POSE_H__

#include <vector>
#include <mio/mio.h>
#include <mio/array.h>
#include <mio/point3.h>
#include <mio/reference.h>
#include <mio/exceptions.h>
#include <mio/rigging/pose.h>
#include <mio/ik/ik_end_effector_goals.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace ik
    {		
		class MIO_API IKPose : public mio::ReferenceCounted
        {
		private:
			Reference<mio::rigging::Pose> normal_pose_;
			Reference<IKEndEffectorGoals> end_effector_goals_;

        public:
            IKPose();
			virtual ~IKPose();

			inline mio::rigging::Pose *get_normal_pose()
			{
				return normal_pose_.get_pointer();
			}			

			inline void set_normal_pose(mio::rigging::Pose *pose)
			{
				normal_pose_ = pose;
			}

			inline IKEndEffectorGoals *get_end_effector_goals()
			{
				return end_effector_goals_.get_pointer();
			}

			inline void set_end_effector_goals(IKEndEffectorGoals *end_effector_goals)
			{
				end_effector_goals_ = end_effector_goals;
			}
        };
    }
}

#pragma warning(pop)

#endif
