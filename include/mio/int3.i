%{
#include <mio/int3.h>
%}

%include <mio/int3.h>

%extend mio::Int3
{
	int __getitem__(int position)
	{
		if (position < 0 || position > 2)
			throw mio::IndexException("index to Int3 out of range");
		else
			return $self->_get(position);
	};
	
	void __setitem__(int position, int value)
	{
		if (position < 0 || position > 2)
			throw mio::IndexException("index to Int3 out of range");
		else
			return $self->_set(position, value);
	};

	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Int3(self)
	%}
}
