#ifndef __MIO_RIGGING_JOINT_CHANGE_H__
#define __MIO_RIGGING_JOINT_CHANGE_H__

#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/quaternion.h>

namespace mio
{
    namespace rigging
    {
        struct MIO_API JointChange
        {
        public:
			Vector3 position;
			Quaternion orientation;

            JointChange(Vector3 _position=Vector3(0,0,0), Quaternion _quaternion=Quaternion());

			inline JointChange __copy__()
			{
				return JointChange(position, orientation);
			}

			inline bool is_near(const JointChange &other, float threshold=0.0001f) const
			{
				return position.is_near(other.position, threshold) && orientation.is_near(other.orientation, threshold);
			}
        };
    }
}

#endif
