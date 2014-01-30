#include <mio/rigging/joint_change.h>

namespace mio
{
    namespace rigging
    {
        JointChange::JointChange(Vector3 _position, Quaternion _quaternion)
        {
			position = _position;
			orientation = _quaternion;
        }
    }
}
