#include <mio/ik/ik_end_effector_goals.h>

namespace mio
{
    namespace ik
    {
		IKEndEffectorPositionIterator::IKEndEffectorPositionIterator()
		{
			end_effector_goals_ = NULL;			
		}

		IKEndEffectorPositionIterator::IKEndEffectorPositionIterator(IKEndEffectorGoals *end_effector_goals)
		{
			end_effector_goals_ = end_effector_goals;
			current_ = end_effector_goals->end_effector_positions_.begin();
		}

		IKEndEffectorPositionIterator::~IKEndEffectorPositionIterator()
		{
		}

		bool IKEndEffectorPositionIterator::has_next()
		{
			return current_ != end_effector_goals_->end_effector_positions_.end();
		}

		NamePositionPair IKEndEffectorPositionIterator::get_next()
		{
			NamePositionPair result = current_->second;
			++current_;
			return result;
		}

		IKEndEffectorGoals::IKEndEffectorGoals() : ReferenceCounted()
        {
        }

        IKEndEffectorGoals::~IKEndEffectorGoals()
        {
        }

		mio::ik::IKEndEffectorPositionIterator IKEndEffectorGoals::get_end_effector_position_iterator()
		{
			return IKEndEffectorPositionIterator(this);
		}
    }
}
