%{
#include <mio/ik/ik_pose.h>
%}

%include <mio/ik/ik_pose.h>

%extend mio::ik::IKPose
{
	%pythoncode
	%{
		def get_end_effector_indices_and_positions(self):
			for i in xrange(self.get_designated_end_effector_count()):
				index = self._get_end_effector_index_by_order(i)
				position = self._get_end_effector_position_by_order(i)
				yield (index, position)
	%}
}