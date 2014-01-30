#include <mio/scenegraph/node.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace scenegraph
	{
		Node::Node(const char *name) : MioObject(name)
		{
		}

		Node::~Node()
		{
		}

		void Node::accept(NodeVisitor *visitor)
		{
			throw NotImplementedException();
		}
	}
}