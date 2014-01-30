#ifndef __MIO_SCENEGRAPH_TRANSFORM_NODE_H__
#define __MIO_SCENEGRAPH_TRANSFORM_NODE_H__

#include <mio/mio.h>
#include <mio/transform.h>
#include <mio/scenegraph/node.h>
#include <mio/reference.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace scenegraph
    {
		class MIO_API TransformNode : public Node
        {
		private:
			Transform transform_;
			Reference<Node> child_;

        public:
            TransformNode(const char *name="");
			TransformNode(const Transform &transform, const char *name="");
            virtual ~TransformNode();

			Node *get_child();
			void set_child(Node *child);

			Transform &get_transform();
			void set_transform(const Transform &transform);
			
			void accept(NodeVisitor *visitor);
        };
    }
}

#pragma warning(pop)

#endif
