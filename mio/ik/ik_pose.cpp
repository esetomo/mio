#include <mio/ik/ik_pose.h>

namespace mio
{
    namespace ik
    {
        IKPose::IKPose()
        {
			normal_pose_ = new mio::rigging::Pose();
			end_effector_goals_ = new IKEndEffectorGoals();
        }

		IKPose::~IKPose()
		{
		}
    }
}
