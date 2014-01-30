%{
#include <mio/vector3.h>
%}

%include <mio/vector3.h>

%extend mio::Vector3 
{
	float __getitem__(int position)
	{
		switch(position)
		{
		case 0:
			return $self->x;
		case 1:
			return $self->y;
		case 2:
			return $self->z;		
		default:
			throw mio::IndexException("index to Vector3 element out of range");
		}
	};
	
	void __setitem__(int position, float value)
	{
		switch(position)
		{
		case 0:
			$self->x = value;
			break;
		case 1:
			$self->y = value;
			break;
		case 2:
			$self->z = value;
			break;		
		default:
			throw mio::IndexException("index to Vector3 element out of range");
		}
	};
	
	%pythoncode 
	%{
		def accept(self, visitor):
			visitor.visit_Vector3(self)
			
		def __repr__(self):
			return "Vector3<%f, %f, %f>" % (self.x, self.y, self.z)
	%}
}