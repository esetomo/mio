#ifndef __MIO_SKINNING_INDEXED_JOINT_WEIGHT_MAPPING_H__
#define __MIO_SKINNING_INDEXED_JOINT_WEIGHT_MAPPING_H__

#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/array.h>
#include <mio/skinning/joint_weight_mapping.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace skinning
    {
		class MIO_API IndexedJointWeightMapping : public JointWeightMapping
        {
		private:
			Array<int> influencing_joint_count_;
			Array<int> joint_indices_;
			Array<int> start_;
			Array<float> weights_;

        public:
            IndexedJointWeightMapping(const char *uid = "");
            virtual ~IndexedJointWeightMapping();

			virtual int get_vertex_count() const
			{
				return influencing_joint_count_.size();
			}

			inline void append_new_vertex()
			{
				start_.push_back(joint_indices_.size());
				influencing_joint_count_.push_back(0);				
			}

			inline void _append_joint_weight(int joint_index, float weight)
			{
				influencing_joint_count_[influencing_joint_count_.size()-1]++;
				joint_indices_.push_back(joint_index);
				weights_.push_back(weight);
			}

			void append_joint_weight(int joint_index, float weight)
			{
				if (influencing_joint_count_.size() <= 0)
					throw mio::ValueException("BoneWeightMap::append_joint_weight() : call append_new_vertex() first before append_joint_weight()");
				else
					_append_joint_weight(joint_index, weight);
			}

			inline int _get_influencing_joint_count(int vertex_index) const
			{
				return influencing_joint_count_[vertex_index];
			}

			virtual int get_influencing_joint_count(int vertex_index) const
			{
				if (vertex_index < 0 || vertex_index >= get_vertex_count())
					throw mio::IndexException("BoneWeightMap::get_influencing_joint_count() : vertex index out of range");
				else
					return _get_influencing_joint_count(vertex_index);
			}

			inline int _get_influencing_joint_index(int vertex_index, int joint_order) const
			{
				return joint_indices_[start_[vertex_index] + joint_order];
			}

			virtual int get_influencing_joint_index(int vertex_index, int joint_order) const
			{
				if (vertex_index < 0 || vertex_index >= get_vertex_count())
					throw mio::IndexException("BoneWeightMap::get_influencing_joint_index() : vertex index out of range");
				else if (joint_order < 0 || joint_order >= _get_influencing_joint_count(vertex_index))
					throw mio::IndexException("BoneWeightMap::get_influencing_joint_index() : joint order out of range");
				else
					return _get_influencing_joint_index(vertex_index, joint_order);
			}

			inline float _get_influencing_joint_weight(int vertex_index, int joint_order) const
			{
				return weights_[start_[vertex_index] + joint_order];
			}

			virtual float get_influencing_joint_weight(int vertex_index, int joint_order) const
			{
				if (vertex_index < 0 || vertex_index >= get_vertex_count())
					throw mio::IndexException("BoneWeightMap::get_influencing_joint_weight() : vertex index out of range");
				else if (joint_order < 0 || joint_order >= _get_influencing_joint_count(vertex_index))
					throw mio::IndexException("BoneWeightMap::get_influencing_joint_weight() : joint order out of range");
				else
					return _get_influencing_joint_weight(vertex_index, joint_order);
			}
        };
	}
}

#pragma warning(pop)

#endif
