#include <mio/scenegraph/transform_node.h>
#include <mio/scenegraph/node_visitor.h>

namespace mio
{
    namespace scenegraph
    {
		TransformNode::TransformNode(const char *name) : Node(name)
        {
        }

		TransformNode::TransformNode(const Transform &transform, const char *name) : Node(name)
		{
			transform_ = transform;
		}

        TransformNode::~TransformNode()
        {
        }

		Transform &TransformNode::get_transform()
		{
			return transform_;
		}

		void TransformNode::set_transform(const Transform &transform)
		{
			transform_ = transform;
		}

		void TransformNode::accept(NodeVisitor *visitor)
		{
			visitor->visit_TransformNode(this);
		}

		Node *TransformNode::get_child()
		{
			return child_.get_pointer();
		}

		void TransformNode::set_child(Node *child)
		{
			child_ = child;
		}
    }
}
