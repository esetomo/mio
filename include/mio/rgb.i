%{
#include <mio/rgb.h>
%}

%include <mio/rgb.h>

%extend mio::Rgb
{
	float __getitem__(int position)
	{
		switch(position)
		{
		case 0:
			return $self->r;
		case 1:
			return $self->g;
		case 2:
			return $self->b;		
		default:
			throw mio::IndexException("index to Rgb element out of range");
		}
	};
	
	void __setitem__(int position, float value)
	{
		switch(position)
		{
		case 0:
			$self->r = value;
			break;
		case 1:
			$self->g = value;
			break;
		case 2:
			$self->b = value;
			break;		
		default:
			throw mio::IndexException("index to Rgb element out of range");
		}
	};
	
	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Rgb(self)
	%}
}