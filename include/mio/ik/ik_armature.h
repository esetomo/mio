#ifndef __MIO_IK_IK_ARMATURE_H__
#define __MIO_IK_IK_ARMATURE_H__

#include <map>
#include <vector>
#include <mio/mio.h>
#include <mio/array.h>
#include <mio/exceptions.h>
#include <mio/reference.h>
#include <mio/mio_object.h>
#include <mio/rigging/armature.h>
#include <mio/ik/ik_pose.h>
#include <mio/ik/ik_joint.h>
#include <mio/ik/ik_end_effector.h>
#include <mio/ik/ik_end_effector_goals.h>

#pragma warning(push)
#pragma warning(disable: 4251)

#define DEFAULT_MAX_ITERATION 20
#define DEFAULT_ERROR_THRESHOLD 0.0001f

namespace mio
{
    namespace ik
    {
		class MIO_API IKArmature : public MioObject
        {
		private:
			struct IKArmatureImplementation;
			IKArmatureImplementation *self;

			friend class IKEndEffectorIterator;
			friend class IKJointIterator;

        public:
			IKArmature(mio::rigging::Armature *rest_armature, const char *uid="");
            virtual ~IKArmature();

			int get_end_effector_count() const;		
			IKEndEffector *get_end_effector(int end_effector_index);
			IKEndEffector *get_end_effector(const char *end_effector_name);
			IKEndEffector *make_end_effector(int joint_index);
			IKEndEffector *make_end_effector(const char *end_effector_name);
			mio::ik::IKEndEffectorIterator get_end_effector_iterator();

			void add_ik_joint_to_end_effector(int ik_joint_index, int end_effector_index);
			void add_ik_joint_to_end_effector(const char *ik_joint_name, const char *end_effector_name);

			int get_ik_joint_count() const;			
			IKJoint *get_ik_joint(int ik_joint_index);			
			IKJoint *get_ik_joint(const char *ik_joint_name);
			IKJoint *make_ik_joint(int joint_index);
			IKJoint *make_ik_joint(const char *joint_name);
			mio::ik::IKJointIterator get_ik_joint_iterator();
			
			mio::rigging::Armature *get_morphed_armature();
			mio::rigging::Armature *get_rest_armature();

			void solve(IKEndEffectorGoals *goals, 
				int max_iteration=DEFAULT_MAX_ITERATION, 
				float error_threshold=DEFAULT_ERROR_THRESHOLD,
				bool reset_ik_joint_parameters=true);
			void solve(IKPose *pose,
				int max_iteration=DEFAULT_MAX_ITERATION,
				float error_threshold=DEFAULT_ERROR_THRESHOLD);
			void solve_for_pose(mio::rigging::Pose &result_pose, IKPose *pose, 
				int max_iteration=DEFAULT_MAX_ITERATION, 
				float error_threshold=DEFAULT_ERROR_THRESHOLD);			
        };

		class MIO_API IKEndEffectorIterator
		{
		private:
			Reference<IKArmature> ik_armature_;
			std::map<int, Reference<IKEndEffector> >::iterator current_;
		public:
			IKEndEffectorIterator();
			IKEndEffectorIterator(IKArmature *ik_armature);
			virtual ~IKEndEffectorIterator();

			IKEndEffector *get_next();
			bool has_next();
		};

		class MIO_API IKJointIterator
		{
		private:
			Reference<IKArmature> ik_armature_;
			std::map<int, Reference<IKJoint> >::iterator current_;
		public:
			IKJointIterator();
			IKJointIterator(IKArmature *ik_armature);
			virtual ~IKJointIterator();

			IKJoint *get_next();
			bool has_next();
		};
    }
}

#pragma warning(pop)

#endif
