#ifndef __MIO_SKINNING_JOINT_WEIGHT_MAPPING_H__
#define __MIO_SKINNING_JOINT_WEIGHT_MAPPING_H__

#include <mio/mio.h>
#include <mio/mio_object.h>

namespace mio
{
    namespace skinning
    {
		class MIO_API JointWeightMapping : public MioObject
        {		
        public:
            JointWeightMapping(const char *uid = "");
            virtual ~JointWeightMapping();

			virtual int get_vertex_count() const = 0;						
			virtual int get_influencing_joint_count(int vertex_index) const = 0;
			virtual int get_influencing_joint_index(int vertex_index, int joint_order) const = 0;			
			virtual float get_influencing_joint_weight(int vertex_index, int joint_order) const = 0;		
        };
    }
}

#endif
