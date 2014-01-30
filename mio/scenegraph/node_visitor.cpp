#include <mio/scenegraph/node_visitor.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace scenegraph
	{
		NodeVisitor::NodeVisitor()
		{
		}

		NodeVisitor::~NodeVisitor()
		{
		}

		void NodeVisitor::visit_SimplePrimitive(mio::scenegraph::SimplePrimitive *primitive)
		{
			throw NotImplementedException();
		}

		void NodeVisitor::visit_Group(mio::scenegraph::Group *group)
		{
			throw NotImplementedException();
		}

		void NodeVisitor::visit_TransformNode(mio::scenegraph::TransformNode *transformNode)
		{
			throw NotImplementedException();
		}

		void NodeVisitor::visit_RiggedPrimitive(mio::scenegraph::RiggedPrimitive *primitive)
		{
			throw NotImplementedException();
		}
	}
}