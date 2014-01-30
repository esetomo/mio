%{
#include <mio/ik/ik_joint_parameters.h>
%}

%include <mio/ik/ik_joint_parameters.h>

%extend mio::ik::IKJointParameters
{
	%pythoncode
	%{
		X_ROTATE = 0
		Y_ROTATE = 1
		Z_ROTATE = 2
		X_TRANSLATE = 3
		Y_TRANSLATE = 4
		Z_TRANSLATE = 5
	%}
}