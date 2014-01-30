%{
#include <mio/vector.h>
%}

%feature("shadow") mio::Vector::__dummy__() %{
def accept(self, visitor):
    visitor.visit_Vector(self)
%}

%include <mio/vector.h>

%extend mio::Vector 
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
			throw mio::IndexException("index to Vector element out of range");
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
			throw mio::IndexException("index to Vector element out of range");
		}
	};
	
	void __dummy__()
	{
	};
}

%feature("shadow") mio::Point::__dummy__() %{
def accept(self, visitor):
    visitor.visit_Point(self)
%}


%extend mio::Point 
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
			throw mio::IndexException("index to Point element out of range");
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
			throw mio::IndexException("index to Vector element out of range");
		}
	};
	
	void __dummy__()
	{
	};
}

%feature("shadow") mio::Normal::__dummy__() %{
def accept(self, visitor):
    visitor.visit_Normal(self)
%}

%extend mio::Normal
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
			throw mio::IndexException("index to Normal element out of range");
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
			throw mio::IndexException("index to Vector element out of range");
		}
	};
	
	void __dummy__()
	{
	};
}