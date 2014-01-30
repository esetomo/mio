%{
#include <mio/scenegraph/group.h>
%}

%include <mio/scenegraph/group.h>

%extend mio::scenegraph::Group
{
	%pythoncode
	%{
		def children(self):
			for i in xrange(self.get_children_count()):
				yield self.get_child(i)
	%}
}