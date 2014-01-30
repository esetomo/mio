#ifndef __MIO_RIGGING_POSE_H__
#define __MIO_RIGGING_POSE_H__

#include <string>
#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/reference_counted.h>
#include <mio/rigging/joint_change.h>
#include <mio/reference.h>
#include <map>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace rigging
    {
		using namespace std;

		class MIO_API NameJointChangePair
		{
		private:
			std::string name_;
			JointChange joint_change_;
		public:
			NameJointChangePair(const char *name, JointChange joint_change)
			{
				name_ = name;
				joint_change_ = joint_change;
			}

			inline const char *get_name() const
			{
				return name_.c_str();
			}

			inline JointChange get_joint_change() const
			{
				return joint_change_;
			}
		};

		class MIO_API Pose : public mio::ReferenceCounted
        {
		private:
			map<std::string, JointChange> joint_changes_by_name_;
			friend class NameJointChangePairIterator;

        public:
            Pose();
			Pose(const Pose &other);
            virtual ~Pose();

			inline bool has_joint_change_of(const char *name) const
			{
				return joint_changes_by_name_.find(name) != joint_changes_by_name_.end();
			}

			inline JointChange get_joint_change(const char *name) const
			{
				map<std::string, JointChange>::const_iterator ii = joint_changes_by_name_.find(name);
				if (ii == joint_changes_by_name_.end())
					return JointChange(Vector3(0,0,0), Quaternion());
				else
					return ii->second;
			}

			inline void set_joint_change(const char *name, const JointChange &joint_change)
			{
				joint_changes_by_name_[name] = joint_change;				
			}

			inline void clear()
			{
				joint_changes_by_name_.clear();
			}

			mio::rigging::NameJointChangePairIterator get_iterator();
        };

		class MIO_API NameJointChangePairIterator
		{
		private:
			Reference<Pose> pose_;
			map<std::string, JointChange>::iterator current_;

		public:
			NameJointChangePairIterator();
			NameJointChangePairIterator(Pose *pose);
			virtual ~NameJointChangePairIterator();

			bool has_next();
			NameJointChangePair get_next();
		};

    }
}

#pragma warning(pop)

#endif
