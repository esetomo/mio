#ifndef __MIO_IK_IK_JOINT_H__
#define __MIO_IK_IK_JOINT_H__

#include <string>
#include <mio/mio.h>
#include <mio/matrix4x4.h>
#include <mio/exceptions.h>
#include <mio/reference_counted.h>
#include <mio/quaternion.h>
#include <mio/definitions.h>
#include <mio/rigging/joint_change.h>
#include <mio/ik/ik_joint_parameters.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace ik
    {		
		class MIO_API IKJoint : public ReferenceCounted
        {
		private:
			int index_;
			std::string name_;
			bool enabled_[IK_JOINT_PARAMETER_COUNT];
			float parameters_[IK_JOINT_PARAMETER_COUNT];
			float lower_bound_[IK_JOINT_PARAMETER_COUNT];
			float upper_bound_[IK_JOINT_PARAMETER_COUNT];
			Quaternion rotation_axis_to_joint_;
			float stiffness_[IK_JOINT_PARAMETER_COUNT];

        public:            
			IKJoint(const char *name, int index);
			IKJoint(const IKJoint &other);
			virtual ~IKJoint();

			inline const char *get_name() const
			{
				return name_.c_str();
			}

			inline void _set_limit(int parameter, float lower_bound, float upper_bound)
			{
				if (parameter == X_ROTATE || parameter == Y_ROTATE || parameter == Z_ROTATE)
				{
					lower_bound_[parameter] = lower_bound * M_PI / 180;
					upper_bound_[parameter] = upper_bound * M_PI / 180;
				}
				else
				{
					lower_bound_[parameter] = lower_bound;
					upper_bound_[parameter] = upper_bound;
				}
			}

			void set_limit(int parameter, float lower_bound, float upper_bound)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::set_limit() : parameter index out of range");
				else
					_set_limit(parameter, lower_bound, upper_bound);
			}

			inline float _get_lower_bound(int parameter)
			{
				return lower_bound_[parameter];
			}

			float get_lower_bound(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::get_lower_bound() : parameter index out of range");
				else
					return _get_lower_bound(parameter);
			}

			inline float _get_upper_bound(int parameter)
			{
				return upper_bound_[parameter];
			}

			float get_upper_bound(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::get_upper_bound() : parameter index out of range");
				else
					return _get_upper_bound(parameter);
			}

			inline void _enable_parameter(int parameter)
			{
				enabled_[parameter] = true;
			}			
			
			void enable_parameter(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::enable_parameter() : parameter index out of range");
				else
					return _enable_parameter(parameter);
			}

			inline void _disable_parameter(int parameter)
			{
				enabled_[parameter] = false;
			}
			
			void disable_parameter(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::disable_parameter() : parameter index out of range");
				else
					return _disable_parameter(parameter);
			}

			inline bool _is_parameter_enabled(int parameter)
			{
				return enabled_[parameter];
			}

			bool is_parameter_enabled(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::is_parameter_enabled() : parameter index out of range");
				else
					return _is_parameter_enabled(parameter);
			}

			inline int get_index()
			{
				return index_;
			}			

			inline float _get_parameter(int parameter)
			{
				return parameters_[parameter];
			}

			float get_parameter(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::get_parameter() : parameter index out of range");
				else
					return _get_parameter(parameter);
			}

			inline void _set_parameter(int parameter, float value)
			{
				parameters_[parameter] = value;
			}

			void set_parameter(int parameter, float value)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::set_parameter() : parameter index out of range");
				else
					_set_parameter(parameter, value);
			}

			inline float _get_stiffness(int parameter)
			{
				return stiffness_[parameter];
			}

			float get_stiffness(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::stiffness() : parameter index out of range");
				else
					return _get_stiffness(parameter);
			}

			inline void _set_stiffness(int parameter, float value)
			{
				stiffness_[parameter] = value;
			}

			void set_stiffness(int parameter, float value)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::set_stiffness() : parameter index out of range");
				else
					_set_stiffness(parameter, value);
			}

			void assign_joint_parameters(const IKJointParameters &joint_parameters)
			{
				FOR(i, IK_JOINT_PARAMETER_COUNT)				
					_set_parameter(i, joint_parameters[i]);				
			}

		private:
			bool are_yaw_pitch_roll_within_limits(float yaw, float pitch, float roll)
			{
				return 
					lower_bound_[Y_ROTATE] <= yaw   &&   yaw <= upper_bound_[Y_ROTATE] &&
					lower_bound_[X_ROTATE] <= pitch && pitch <= upper_bound_[X_ROTATE] &&
					lower_bound_[Z_ROTATE] <= roll  &&  roll <= upper_bound_[Z_ROTATE];
			}

			void get_yaw_pitch_roll(float &yaw, float &pitch, float &roll, const Matrix4x4 &matrix)
			{
				if (fabs(matrix[0][1]) < EPSILON && fabs(matrix[1][1]) < EPSILON)
				{
					if (matrix[2][1] < 0)
						pitch = -M_PI / 2.0f;
					else
						pitch = M_PI / 2.0f;
		            roll = 0.0f;
				    yaw = (float)atan2(matrix[0][2], matrix[0][0]);
				}
				else
				{
					yaw = (float)atan2(-matrix[2][0], matrix[2][2]);
					roll = (float)atan2(-matrix[0][1], matrix[1][1]);
					pitch = (float)atan2(matrix[2][1], sqrt(matrix[2][0]*matrix[2][0] + matrix[2][2]*matrix[2][2]));
				}

				// If the yaw, pitch, and roll are not within the limits,
				// we try other equivalent angles.
				if (!are_yaw_pitch_roll_within_limits(yaw, pitch, roll))
				{
					float new_yaw = yaw + M_PI;
					float new_pitch = -pitch + M_PI;
					float new_roll = roll + M_PI;
					if (are_yaw_pitch_roll_within_limits(new_yaw, new_pitch, new_roll))
					{
						yaw = new_yaw;
						pitch = new_pitch;
						roll = new_roll;
						return;
					}
					new_yaw = yaw - M_PI;
					new_pitch = -pitch - M_PI;
					new_roll = roll - M_PI;
					if (are_yaw_pitch_roll_within_limits(new_yaw, new_pitch, new_roll))
					{
						yaw = new_yaw;
						pitch = new_pitch;
						roll = new_roll;
						return;
					}
				}
			}

		public:
			inline void set_according_to_joint_change(const mio::rigging::JointChange &joint_change)
			{
				_set_parameter(X_TRANSLATE, joint_change.position.x);
				_set_parameter(Y_TRANSLATE, joint_change.position.y);
				_set_parameter(Z_TRANSLATE, joint_change.position.z);

				Quaternion orientation = 
					inverse(rotation_axis_to_joint_) * 
					joint_change.orientation *
					rotation_axis_to_joint_;
				Matrix4x4 rotation_matrix = orientation.get_matrix();
				float yaw, pitch, roll;
				get_yaw_pitch_roll(yaw, pitch, roll, rotation_matrix);

				_set_parameter(Y_ROTATE, yaw);
				_set_parameter(X_ROTATE, pitch);
				_set_parameter(Z_ROTATE, roll);
			}

			inline Quaternion get_yaw_quaternion() const
			{
				float y = sinf(parameters_[Y_ROTATE] / 2.0f);
				float w = cosf(parameters_[Y_ROTATE] / 2.0f);
				return Quaternion(0,y,0,w);
			}

			inline Quaternion get_pitch_quaternion() const
			{
				float x = sinf(parameters_[X_ROTATE] / 2.0f);
				float w = cosf(parameters_[X_ROTATE] / 2.0f);
				return Quaternion(x,0,0,w);
			}

			inline Quaternion get_roll_quaternion() const
			{
				float z = sinf(parameters_[Z_ROTATE] / 2.0f);
				float w = cosf(parameters_[Z_ROTATE] / 2.0f);
				return Quaternion(0,0,z,w);
			}

			inline Vector3 get_rotation_axis_in_joint_space(int parameter) const
			{
				if (parameter != X_ROTATE && parameter != Y_ROTATE && parameter != Z_ROTATE)				
					throw mio::IndexException("IKJoint::get_rotation_axis_in_joint_space() : parameter must be rotation parameter");
				else if (parameter == Z_ROTATE)			
					return rotation_axis_to_joint_.rotate(Vector3(0,0,1));
				else if (parameter == X_ROTATE)
				{
					Quaternion rotation = rotation_axis_to_joint_ * get_pitch_quaternion();
					return rotation.rotate(Vector3(1,0,0));
				}				
				else
				{					
					Quaternion rotation = rotation_axis_to_joint_ *
						get_roll_quaternion() * 
						get_pitch_quaternion();
					return rotation.rotate(Vector3(0,1,0));
				}
			}

			inline Vector3 get_position_change() const
			{
				return Vector3(
					parameters_[X_TRANSLATE],
					parameters_[Y_TRANSLATE],
					parameters_[Z_TRANSLATE]);
			}

			inline Quaternion get_orientation_change() const
			{
				return rotation_axis_to_joint_ *
					get_yaw_quaternion() *
					get_pitch_quaternion() *
					get_roll_quaternion() *
					inverse(rotation_axis_to_joint_);
			}

			inline float get_yaw() const 
			{
				return parameters_[Y_ROTATE];
			}

			inline float get_pitch() const
			{
				return parameters_[X_ROTATE];
			}

			inline float get_roll() const
			{
				return parameters_[Z_ROTATE];
			}

			inline Vector3 rotate(const Vector3 &v) const
			{
				Quaternion rotation = get_orientation_change();
				return rotation.rotate(v);
			}

			inline Point3 rotate(const Point3 &p) const
			{
				Quaternion rotation = get_orientation_change();
				return rotation.rotate(p);
			}

			inline void _clamp_parameter(int parameter)
			{
				if (parameters_[parameter] < lower_bound_[parameter])
					parameters_[parameter] = lower_bound_[parameter];
				if (parameters_[parameter] > upper_bound_[parameter])
					parameters_[parameter] = upper_bound_[parameter];
			}

			void clamp_parameter(int parameter)
			{
				if (parameter < 0 || parameter >= IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("IKJoint::clamp_parameter() : parameter index out of range");
				else
					_clamp_parameter(parameter);
			}

			inline void clamp_all_parameters()
			{
				FOR(i, IK_JOINT_PARAMETER_COUNT)
					_clamp_parameter(i);
			}

			inline Quaternion get_rotation_axis_to_joint_rotation()
			{
				return rotation_axis_to_joint_;
			}

			inline void set_rotation_to_axis_joint_rotation(const Quaternion &quaternion)
			{
				rotation_axis_to_joint_ = quaternion;
			}

			inline int get_dof()
			{
				int result = 0;
				FOR(i, IK_JOINT_PARAMETER_COUNT)
					if (enabled_[i])
						result++;
				return result;
			}

			inline mio::rigging::JointChange get_joint_change()
			{
				return mio::rigging::JointChange(get_position_change(), get_orientation_change());
			}

			inline void clear_parameters()
			{
				FOR(i, IK_JOINT_PARAMETER_COUNT)
					parameters_[i] = 0.0f;
			}
        };
    }
}

#pragma warning(pop)

#endif
