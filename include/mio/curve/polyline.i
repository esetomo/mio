%{
#include <mio/curve/polyline.h>
%}

%include <mio/curve/polyline.h>

%extend mio::curve::Polyline_float
{
	%pythoncode
	%{
		def get_control_points(self):
			return self.get_control_point_iterator()
	%}
}

%extend mio::curve::Polyline_Vector3
{
	%pythoncode
	%{
		def get_control_points(self):
			return self.get_control_point_iterator()
	%}
}

%extend mio::curve::Polyline_Quaternion
{
	%pythoncode
	%{
		def get_control_points(self):
			return self.get_control_point_iterator()
	%}
}

%extend mio::curve::Polyline_JointChange
{
	%pythoncode
	%{
		def get_control_points(self):
			return self.get_control_point_iterator()
	%}
}

%extend mio::curve::PolylineControlPointIterator_float
{
	%pythoncode
	%{
		def __iter__(self):
			return self
			
		def next(self):
			if not self.has_next():
				raise StopIteration
			else:
				return self.get_next()
	%}
}

%extend mio::curve::PolylineControlPointIterator_Vector3
{
	%pythoncode
	%{
		def __iter__(self):
			return self
			
		def next(self):
			if not self.has_next():
				raise StopIteration
			else:
				return self.get_next()
	%}
}

%extend mio::curve::PolylineControlPointIterator_Quaternion
{
	%pythoncode
	%{
		def __iter__(self):
			return self
			
		def next(self):
			if not self.has_next():
				raise StopIteration
			else:
				return self.get_next()
	%}
}

%extend mio::curve::PolylineControlPointIterator_JointChange
{
	%pythoncode
	%{
		def __iter__(self):
			return self
			
		def next(self):
			if not self.has_next():
				raise StopIteration
			else:
				return self.get_next()
	%}
}