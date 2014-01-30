#include <mio/ik/ik_joint_parameters.h>

namespace mio
{
    namespace ik
    {
        IKJointParameters::IKJointParameters()
        {
			FOR(i, IK_JOINT_PARAMETER_COUNT)
				parameters_[i] = 0.0f;
        }        
    }
}
