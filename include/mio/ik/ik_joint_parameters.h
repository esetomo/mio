#ifndef __MIO_IK_IK_JOINT_PARAMETERS_H__
#define __MIO_IK_IK_JOINT_PARAMETERS_H__

#include <mio/definitions.h>
#include <mio/mio.h>
#include <mio/exceptions.h>

#define X_ROTATE 0
#define Y_ROTATE 1
#define Z_ROTATE 2
#define X_TRANSLATE 3
#define Y_TRANSLATE 4
#define Z_TRANSLATE 5

#define IK_JOINT_PARAMETER_COUNT 6

namespace mio
{
    namespace ik
    {
        struct MIO_API IKJointParameters
        {
		private:			
			float parameters_[6];

        public:
            IKJointParameters();

			float operator[](int index) const
			{
				return parameters_[index];
			}

			float &operator[](int index)
			{
				return parameters_[index];
			}

			float __getitem__(int position)
			{
				if (position < 0 || position > IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("index to joint parameter out of range");
				else
					return parameters_[position];
			};
	
			void __setitem__(int position, float value)
			{
				if (position < 0 || position > IK_JOINT_PARAMETER_COUNT)
					throw mio::IndexException("index to joint parameter out of range");
				else
					parameters_[position] = value;		
			};

			bool is_near(const IKJointParameters &other, float threshold=0.0001f) const 
			{
				FOR(i, IK_JOINT_PARAMETER_COUNT)
					if (fabs(parameters_[i] - other.parameters_[i]) > threshold)
						return false;
				return true;
			}

			IKJointParameters __copy__()
			{
				IKJointParameters result;
				FOR(i, IK_JOINT_PARAMETER_COUNT)
					result.parameters_[i] = parameters_[i];
				return result;
			}

			IKJointParameters __deepcopy__()
			{
				return __copy__();
			}
        };
    }
}

#endif
