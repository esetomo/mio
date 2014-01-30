#ifndef __MIO_SCENEGRAPH_NODE_VISITOR_H__
#define __MIO_SCENEGRAPH_NODE_VISITOR_H__

#include <mio/scenegraph/node.h>

#include <mio/scenegraph/simple_primitive.h>
#include <mio/scenegraph/group.h>
#include <mio/scenegraph/transform_node.h>
#include <mio/scenegraph/rigged_primitive.h>

namespace mio
{
	namespace scenegraph
	{
		class MIO_API NodeVisitor
		{
		public:
			NodeVisitor();
			virtual ~NodeVisitor();			

			virtual void visit_SimplePrimitive(mio::scenegraph::SimplePrimitive *primitive);			
			virtual void visit_Group(mio::scenegraph::Group *group);
			virtual void visit_TransformNode(mio::scenegraph::TransformNode *transformNode);
			virtual void visit_RiggedPrimitive(mio::scenegraph::RiggedPrimitive *primitive);
		};
	}
}

#endif