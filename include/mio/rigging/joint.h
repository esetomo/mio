#ifndef __MIO_RIGGING_JOINT_H__
#define __MIO_RIGGING_JOINT_H__

#include <string.h>
#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/quaternion.h>
#include <mio/matrix4x4.h>
#include <mio/transform.h>
#include <mio/reference_counted.h>
#include <mio/rigging/joint_change.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace rigging
    {
		class MIO_API Joint : public ReferenceCounted
        {
		private:
			std::string name_;

        public:
			int index;
			int parent_index;
			Vector3 position;
			Quaternion orientation;

            Joint(
				const char *name,
				Vector3 _position=Vector3(0,0,0),
				Quaternion _orientation=Quaternion(),
				int _index=-1, 
				int _parent_index = -1);
			Joint(const Joint &other);
			virtual ~Joint();

			inline const char *get_name() const
			{
				return name_.c_str();
			}

			inline Matrix4x4 get_translation_matrix() const
			{
				return Matrix4x4::translate(
					position.x,
					position.y,
					position.z);
			}

			inline Transform get_translation_transform() const
			{
				return Transform::translate(
					position.x,
					position.y,
					position.z);
			}

			inline Matrix4x4 get_rotation_matrix() const
			{
				return orientation.get_matrix();
			}

			inline Transform get_rotation_transform() const
			{
				Matrix4x4 matrix = orientation.get_matrix();
				return Transform(matrix, transpose(matrix));
			}
		
			inline void apply_joint_change(const JointChange &joint_change)
			{
				position += joint_change.position;
				orientation = orientation * joint_change.orientation;
			}

			inline JointChange get_joint_change(const Joint &other) const
			{
				Vector3 position_change = other.position - position;
				Quaternion orientation_change = conjugate(orientation) * other.orientation;
				return JointChange(position_change, orientation_change);
			}			
        };
    }
}

#pragma warning(pop)

#endif
