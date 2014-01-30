#include <mio/rigging/armature.h>

namespace mio
{
    namespace rigging
    {
		Armature::Armature(const char *uid) : MioObject(uid)
        {			
        }

		Armature::Armature(Armature *other, const char *uid) : MioObject(uid)
		{			
			FOR(i, (int)other->joints_.size())
			{
				Reference<Joint> new_joint = new Joint(*other->joints_[i].get_pointer());
				append_joint(new_joint.get_pointer());				
			}
		}

        Armature::~Armature()
        {			
        }

		JointIterator::JointIterator()
		{
			armature_ = NULL;
		}

		JointIterator::JointIterator(Armature *armature)
		{
			armature_ = armature;
			current_ = armature->joints_.begin();
		}

		JointIterator::~JointIterator()
		{
		}

		bool JointIterator::has_next()
		{
			return current_ != armature_->joints_.end();
		}

		Joint *JointIterator::get_next()
		{
			Joint *result = current_->get_pointer();
			++current_;
			return result;
		}

		mio::rigging::JointIterator Armature::get_joint_iterator()
		{
			return JointIterator(this);
		}
    }
}
