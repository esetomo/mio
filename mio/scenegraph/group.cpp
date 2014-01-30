#include <mio/scenegraph/group.h>
#include <mio/scenegraph/node_visitor.h>

namespace mio
{
	namespace scenegraph
	{
		Group::Group(const char *name) : Node(name)
		{
		}

		Group::~Group()
		{			
		}

		void Group::append_child(Node *node)
		{
			children_.push_back(Reference<Node>(node));
		}

		Node *Group::_get_child(int index)
		{
			return children_[index].get_pointer();
		}

		Node *Group::get_child(int index)
		{
			if (index < 0 || index >= get_children_count())
				throw IndexException("index to group's children out of range");
			else
				return children_[index].get_pointer();
		}

		int Group::get_children_count()
		{
			return (int)children_.size();
		}

		void Group::accept(NodeVisitor *visitor)
		{
			visitor->visit_Group(this);
		}
	}
}