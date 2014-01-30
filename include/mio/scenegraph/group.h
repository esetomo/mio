#ifndef __MIO_SCENEGRAPH_GROUP_H__
#define __MIO_SCENEGRAPH_GROUP_H__

#include <mio/mio.h>
#include <vector>
#include <mio/scenegraph/node.h>
#include <mio/reference.h>

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace scenegraph
	{
		class MIO_API Group : public Node
		{
		private:
			vector< Reference<Node> > children_;			

		public:			
			Group(const char *name="");
			virtual ~Group();

			void append_child(Node *node);
			Node *_get_child(int index);
			Node *get_child(int index);
			int get_children_count();

			virtual void accept(NodeVisitor *visitor);
		};
	}
}

#pragma warning(pop)

#endif