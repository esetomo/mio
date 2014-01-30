#ifndef __MIO_IK_IK_END_EFFECTOR_H__
#define __MIO_IK_IK_END_EFFECTOR_H__

#include <string>
#include <mio/definitions.h>
#include <mio/reference_counted.h>
#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/array.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace ik
    {
		class MIO_API IKEndEffector : public ReferenceCounted
        {		
		private:
			int index_;			
			std::string name_;
			Array<int> ik_joint_indices_;

        public:            
			IKEndEffector(const char *name, int index);
			IKEndEffector(const IKEndEffector &other);
			virtual ~IKEndEffector();

			inline int get_index() const
			{
				return index_;
			}

			inline const char *get_name() const
			{
				return name_.c_str();
			}

			inline void append_ik_joint_index(int index)
			{
				ik_joint_indices_.push_back(index);
			}

			inline IKEndEffector &operator=(const IKEndEffector &other)
			{
				index_ = other.index_;
				name_ = other.name_;
				ik_joint_indices_.adjust_size(other.ik_joint_indices_.size(), false);
				FOR(i, other.ik_joint_indices_.size())
					ik_joint_indices_[i] = other.ik_joint_indices_[i];
				return *this;
			}

			inline IKEndEffector __copy__()
			{
				IKEndEffector result = *this;
				return result;
			}

			inline IKEndEffector __deepcopy__()
			{
				return __copy__();
			}

			inline int get_ik_joint_index_count()
			{
				return ik_joint_indices_.size();
			}

			inline int _get_ik_joint_index(int index)
			{
				return ik_joint_indices_[index];
			}

			inline int get_ik_joint_index(int index)
			{
				if (index < 0 || index >= get_ik_joint_index_count())
					throw mio::IndexException("IKEndEffector::get_ik_joint_index() : ik joint index's index out of range");
				else
					return _get_ik_joint_index(index);
			}

			inline bool is_influenced_by(int ik_joint_index)
			{
				FOR(i, ik_joint_indices_.size())
					if (ik_joint_index == ik_joint_indices_[i])
						return true;
				return false;
			}
        };
    }
}

#pragma warning(pop)

#endif
