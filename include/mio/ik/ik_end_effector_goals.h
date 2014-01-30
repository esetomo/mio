#ifndef __MIO_IK_IK_END_EFFECTOR_GOALS_H__
#define __MIO_IK_IK_END_EFFECTOR_GOALS_H__

#include <map>
#include <vector>
#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/reference.h>
#include <mio/reference_counted.h>
#include <mio/point3.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace ik
    {
		class MIO_API NamePositionPair
		{
		private:
			std::string name_;
			Point3 position_;

		public:
			NamePositionPair()
			{
				name_ = "";
				position_ = Point3(0,0,0);
			}

			NamePositionPair(const char *_name, const Point3 &_position)
			{
				name_ = _name;
				position_ = _position;
			}

			NamePositionPair(const NamePositionPair &other)
			{
				name_ = other.name_;
				position_ = other.position_;
			}

			inline const char *get_name()
			{
				return name_.c_str();
			}

			inline Point3 get_position()
			{
				return position_;
			}

			inline void set_position(const Point3 &position)
			{
				position_ = position;
			}
		};		

		class IKEndEffectorPositionIterator;

		typedef std::map<std::string, NamePositionPair> NamePositionMap;

		class MIO_API IKEndEffectorGoals : public ReferenceCounted
        {
		private:						
			NamePositionMap end_effector_positions_;
			friend class IKEndEffectorPositionIterator;			

			bool contains_end_effector_with_given_name(const char *end_effector_name)
			{
				NamePositionMap::iterator ii = end_effector_positions_.find(end_effector_name);
				return ii == end_effector_positions_.end();
			}			

        public:
            IKEndEffectorGoals();
            virtual ~IKEndEffectorGoals();

			inline void set_end_effector_position(const char *end_effector_name, const Point3 &position)
			{
				NamePositionMap::iterator ii = end_effector_positions_.find(end_effector_name);
				if (ii != end_effector_positions_.end())
					ii->second.set_position(position);
				else
					end_effector_positions_[end_effector_name] = NamePositionPair(end_effector_name, position);					
			}
			
			Point3 get_end_effector_position(const char *end_effector_name)
			{
				NamePositionMap::iterator ii = end_effector_positions_.find(end_effector_name);
				if (ii == end_effector_positions_.end())
					throw mio::IndexException("IKPose::get_end_effector_position_by_name() : the pose does not specify end effector position of the given end effector");
				else
					return ii->second.get_position();
			}

			inline int get_designated_end_effector_count()
			{
				return (int)end_effector_positions_.size();
			}

			mio::ik::IKEndEffectorPositionIterator get_end_effector_position_iterator();
        };

		class MIO_API IKEndEffectorPositionIterator
		{
		private:
			NamePositionMap::iterator current_;
			Reference<IKEndEffectorGoals> end_effector_goals_;

		public:
			IKEndEffectorPositionIterator();
			IKEndEffectorPositionIterator(IKEndEffectorGoals *end_effector_goals);
			virtual ~IKEndEffectorPositionIterator();

			bool has_next();
			NamePositionPair get_next();
		};
    }
}

#pragma warning(pop)

#endif
