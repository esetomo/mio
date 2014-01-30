#include <mio/rigging/joint.h>

namespace mio
{
    namespace rigging
    {
        Joint::Joint(
			const char *name,
			Vector3 _position,
			Quaternion _orientation,
			int _index,
			int _parent_index) : ReferenceCounted()
        {
			index = _index;
			position = _position;
			orientation = _orientation;
			parent_index = _parent_index;
			name_ = name;
        }

		Joint::Joint(const Joint &other)
		{
			index = other.index;
			position = other.position;
			orientation = other.orientation;
			parent_index = other.parent_index;
			name_ = other.name_;
		}

		Joint::~Joint()
		{
		}
    }
}
