#ifndef __MIO_RIGGING_ARMATURE_H__
#define __MIO_RIGGING_ARMATURE_H__

#include <map>
#include <vector>
#include <stdlib.h>
#include <mio/mio.h>
#include <mio/array.h>
#include <mio/reference.h>
#include <mio/matrix4x4.h>
#include <mio/mio_object.h>
#include <mio/rigging/joint.h>
#include <mio/rigging/joint_change.h>
#include <mio/rigging/pose.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace rigging
    {		
		class MIO_API Armature : public mio::MioObject
        {
		private:
			vector< Reference<Joint> > joints_;
			std::map< std::string, int > name_to_index_map_;		

			friend class JointIterator;
			
        public:
			Armature(const char *uid="");
			Armature(Armature *other, const char *uid="");
            virtual ~Armature();

			inline void append_joint(Joint *joint)
			{
				std::string joint_name = joint->get_name();
				if (name_to_index_map_.find(joint_name) != name_to_index_map_.end())
					throw mio::ValueException("Armature::append_joint() : joint with the same name already exist");
				
				Reference<Joint> ref = joint;
				joints_.push_back(ref);
				joint->index = (int)joints_.size()-1;
				name_to_index_map_[joint_name] = joint->index;
			}

			inline Joint *_get_joint(int index)
			{
				return joints_[index].get_pointer();
			}

			inline Joint *get_joint(int index)
			{
				if (index < 0 || index >= (int)joints_.size())
					throw mio::IndexException("Armature::get_joint() : joint index out of range");
				else
					return _get_joint(index);
			}

			inline void _set_joint_parameter(int index, const Vector3 &position, const Quaternion &orientation)
			{
				Joint *joint = joints_[index].get_pointer();
				joints_[index]->position = position;
				joints_[index]->orientation = orientation;
			}

			inline void set_joint_parameter(int index, const Vector3 &position, const Quaternion &orientation)
			{
				if (index < 0 || index >= (int)joints_.size())
					throw mio::IndexException("Armature::set_joint_parameter() : joint index out of range");
				else
					_set_joint_parameter(index, position, orientation);
			}

			inline void set_joint_parameter(const char *joint_name, const Vector3 &position, const Quaternion &orientation)
			{
				Joint *joint = get_joint(joint_name);
				_set_joint_parameter(joint->index, position, orientation);
			}

			inline Joint *get_joint(const char *name)
			{
				if (name_to_index_map_.find(name) == name_to_index_map_.end())
					throw mio::ValueException("Armature::get_joint() : no joint with the given name.");
				else
				{
					int joint_index = name_to_index_map_[name];
					return joints_[joint_index].get_pointer();
				}
			}			

			inline int get_joint_count() const
			{
				return (int)joints_.size();
			}

			inline void _set_parent(int child_index, int parent_index)
			{
				joints_[child_index]->parent_index = parent_index;				
			}			

			inline void set_parent(int child_index, int parent_index)
			{
				if (child_index < 0 || child_index >= (int)joints_.size())
					throw mio::IndexException("Armature::set_parent() : child index out of range");
				if (parent_index < -1 || parent_index >= (int)joints_.size())
					throw mio::IndexException("Armature::set_parent() : parent index out of range");
				if (parent_index == child_index)
					throw mio::IndexException("Armature::set_parent() : parent index same as child index");
				_set_parent(child_index, parent_index);
			}

			inline void set_parent(const char *child_name, const char *parent_name)
			{
				Joint *child = get_joint(child_name);
				Joint *parent = get_joint(parent_name);
				set_parent(child->index, parent->index);
			}

			inline Point3 _get_joint_world_position(int joint_index)
			{
				Point3 result(0,0,0);
				while (joint_index >= 0)
				{
					Joint *joint = joints_[joint_index].get_pointer();
					result = joint->orientation.rotate(result);
					result += joint->position;
					joint_index = joint->parent_index;
				}
				return result;
			}

			inline Point3 get_joint_world_position(int joint_index)
			{
				if (joint_index < 0 || joint_index >= (int)joints_.size())
					throw mio::IndexException("Armature::get_joint_world_position() : joint index out of range");
				else
					return _get_joint_world_position(joint_index);
			}

			inline Quaternion _get_joint_world_orientation(int joint_index)
			{
				Quaternion result = Quaternion();
				while (joint_index >= 0)
				{
					Joint *joint = joints_[joint_index].get_pointer();
					result = joint->orientation * result;					
					joint_index = joint->parent_index;
				}
				return result;
			}

			inline Quaternion get_joint_world_orientation(int joint_index)
			{
				if (joint_index < 0 || joint_index >= (int)joints_.size())
					throw mio::IndexException("Armature::get_joint_world_orientation() : joint index out of range");
				else
					return _get_joint_world_orientation(joint_index);
			}			

			inline Point3 get_joint_world_position(const char *joint_name)
			{
				Joint *joint = get_joint(joint_name);
				return get_joint_world_position(joint->index);
			}

			inline Quaternion get_joint_world_orientation(const char *joint_name)
			{
				Joint *joint = get_joint(joint_name);
				return get_joint_world_orientation(joint->index);
			}

			inline Matrix4x4 _get_joint_to_world_matrix(int joint_index)
			{
				Matrix4x4 result = Matrix4x4::identity();
				while (joint_index >= 0)
				{
					Joint *joint = joints_[joint_index].get_pointer();
					result = joint->orientation.get_matrix() * result;
					Vector3 position = joint->position;
					result = Matrix4x4::translate(position.x, position.y, position.z) * result;
					joint_index = joint->parent_index;
				}
				return result;
			}

			inline Matrix4x4 get_joint_to_world_matrix(int joint_index)
			{
				if (joint_index < 0 || joint_index >= get_joint_count())
					throw mio::IndexException("Armature::get_joint_world_matrix() : joint index out of range");
				else
					return _get_joint_to_world_matrix(joint_index);
			}

			inline Matrix4x4 get_joint_to_world_matrix(const char *joint_name)
			{
				Joint *joint = get_joint(joint_name);
				return get_joint_to_world_matrix(joint->index);
			}

			inline Transform _get_joint_to_world_transform(int joint_index)
			{
				Transform result = Transform::identity();
				while (joint_index >= 0)
				{
					Joint *joint = joints_[joint_index].get_pointer();
					Matrix4x4 m = joint->orientation.get_matrix();
					Transform t(m, transpose(m));
					result = t * result;
					Vector3 position = joint->position;
					result = Transform::translate(position.x, position.y, position.z) * result;
					joint_index = joint->parent_index;
				}
				return result;
			}

			inline Transform get_joint_to_world_transform(int joint_index)
			{
				if (joint_index < 0 || joint_index >= get_joint_count())
					throw mio::IndexException("Armature::get_joint_world_transform() : joint index out of range");
				else
					return _get_joint_to_world_transform(joint_index);
			}

			inline Transform get_joint_to_world_transform(char *joint_name)
			{
				Joint *joint = get_joint(joint_name);
				return get_joint_to_world_transform(joint->index);
			}

			inline void get_pose(Pose &pose, Armature &other)
			{
				if (other.get_joint_count() != get_joint_count())
					throw mio::ValueException("Armature::get_post() : other armature does not have the same number of joints.");
				FOR(joint_index, get_joint_count())
				{
					Joint *this_joint = _get_joint(joint_index);
					Joint *other_joint = other._get_joint(joint_index);
					if (strcmp(this_joint->get_name(), other_joint->get_name()) != 0)
						throw mio::ValueException("Armature::get_post(): joints with the same index have different names");
					JointChange change = this_joint->get_joint_change(*other_joint);
					pose.set_joint_change(this_joint->get_name(), change);
				}
			}

			inline Pose *get_pose(Armature &other)
			{
				Pose *pose = new Pose();
				get_pose(*pose, other);
				return pose;
			}

			inline void morph(Armature &result, const Pose &pose)
			{
				if (result.get_joint_count() != get_joint_count())
					throw mio::ValueException("Armature::morph() : result armature does not have the same number of joints.");
				FOR(joint_index, get_joint_count())
				{								
					Joint *joint = _get_joint(joint_index);
					JointChange joint_change = pose.get_joint_change(joint->get_name());					
					
					Vector3 new_position = joint->position + joint_change.position;
					Quaternion new_orientation = joint->orientation * joint_change.orientation;					
					result.set_joint_parameter(joint_index, new_position, new_orientation);					
				}
			}

			Armature *__copy__()
			{
				Armature *result = new Armature();
				FOR(i, get_joint_count())
				{
					Joint *joint = joints_[i].get_pointer();
					Reference<Joint> new_joint = new Joint(*joint);
					result->append_joint(new_joint.get_pointer());					
				}
				return result;
			}

			Armature *__deepcopy__()
			{
				return __copy__();
			}

			mio::rigging::JointIterator get_joint_iterator();
        };

		class MIO_API JointIterator
		{
		private:
			Reference<Armature> armature_;
			vector< Reference<Joint> >::iterator current_;

		public:
			JointIterator();
			JointIterator(Armature *armature);
			virtual ~JointIterator();

			bool has_next();
			Joint *get_next();
		};
    }
}

#pragma warning(pop)

#endif
