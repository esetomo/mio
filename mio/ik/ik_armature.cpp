#include <iostream>
#include <stdio.h>
#include <mio/ik/ik_armature.h>
#include <mio/rigging/joint.h>
#include <mio/rigging/joint_change.h>
#include <mio/rigging/pose.h>
#include <mio/vector3.h>
#include <mio/matrix4x4.h>
#include <mio/vector3.h>
#include <Eigen/Core>
#include <Eigen/SVD>

#include <set>

#define MIN(a,b) (((a)<(b))?(a):(b))

namespace mio
{
    namespace ik
    {
		struct IKArmature::IKArmatureImplementation
		{
			Reference<mio::rigging::Armature> rest_armature_;
			Reference<mio::rigging::Armature> morphed_armature_;

			std::map<int, Reference<IKEndEffector> > end_effectors_by_index_;
			std::map<std::string, Reference<IKEndEffector> > end_effectors_by_name_;

			std::map<int, Reference<IKJoint> > ik_joints_by_index_;
			std::map<std::string, Reference<IKJoint> > ik_joints_by_name_;

			float D_max_;

			vector< Reference<IKEndEffector> > relevant_end_effector_list_;
			vector< Reference<IKJoint> > relevant_ik_joint_list_;			

			int jacobian_column_count_;
			int jacobian_row_count_;
			vector< std::pair<Reference<IKJoint>, int> > jacobian_column_list_;
			Eigen::MatrixXd goal_vector_;
			Eigen::MatrixXd end_effector_position_vector_;
			Eigen::MatrixXd error_vector_;
			Eigen::MatrixXd Delta_;
			Eigen::MatrixXd H_;
			Eigen::MatrixXd jacobian_;

			Array<float> N, M, gamma, alpha;
			Eigen::MatrixXd U, V, sigma, phi_i;

			void clear_ik_joint_parameters();
			void get_end_effector_position_vector(Eigen::MatrixXd &v);
			void compute_jacobian();			
			void clamp_mag(Eigen::MatrixXd &output, Eigen::MatrixXd &input, float D_max_);
			void clamp_rotation_component(Eigen::MatrixXd &v, float gamma_max);

			void selectively_damped_least_square();			
			void jacobian_transpose();

			bool ik_joint_exists(int ik_joint_index) const;
			bool ik_joint_exists(const char *ik_joint_name) const;
			
			bool end_effector_exists(int end_effector_index) const;
			bool end_effector_exists(const char *end_effector_name) const;

			void solve(IKEndEffectorGoals *goals, 
				int max_iteration=DEFAULT_MAX_ITERATION, 
				float error_threshold=DEFAULT_ERROR_THRESHOLD,
				bool reset_ik_joint_parameters=true);

			void morph_morphed_armature(mio::rigging::Pose *pose);			
		};

		IKArmature::IKArmature(mio::rigging::Armature *armature, const char *uid) : MioObject(uid)
        {
			self = new IKArmatureImplementation();
			
			self->rest_armature_ = armature;
			if (self->rest_armature_.get_pointer() == NULL)
			{
				self->morphed_armature_ = NULL;
				self->D_max_ = 0.0f;
			}
			else			
			{
				self->morphed_armature_ = new mio::rigging::Armature(self->rest_armature_.get_pointer());

				self->D_max_ = 0.0f;
				FOR(i, self->rest_armature_->get_joint_count())
				{
					mio::rigging::Joint *joint = self->rest_armature_->get_joint(i);
					float l = joint->position.length() / 2.0f;
					if (self->D_max_ < l)
						self->D_max_ = l;	
				}
			}
        }

		IKArmature::~IKArmature()
        {
			delete self;
        }

		int IKArmature::get_end_effector_count() const
		{
			return (int)self->end_effectors_by_index_.size();
		}

		
		int IKArmature::get_ik_joint_count() const
		{
			return (int)self->ik_joints_by_index_.size();
		}
		
		bool IKArmature::IKArmatureImplementation::ik_joint_exists(int ik_joint_index) const
		{
			return ik_joints_by_index_.find(ik_joint_index) != ik_joints_by_index_.end();
		}

		bool IKArmature::IKArmatureImplementation::ik_joint_exists(const char *ik_joint_name) const
		{
			return ik_joints_by_name_.find(ik_joint_name) != ik_joints_by_name_.end();
		}

		bool IKArmature::IKArmatureImplementation::end_effector_exists(int end_effector_index) const
		{
			return end_effectors_by_index_.find(end_effector_index) != end_effectors_by_index_.end();
		}

		bool IKArmature::IKArmatureImplementation::end_effector_exists(const char *end_effector_name) const
		{
			return end_effectors_by_name_.find(end_effector_name) != end_effectors_by_name_.end();
		}

		IKJoint *IKArmature::get_ik_joint(int ik_joint_index)
		{
			if (!self->ik_joint_exists(ik_joint_index))
				throw mio::IndexException("IKArmature::get_ik_joint() : ik joint with the given id does not exist");
			else
				return self->ik_joints_by_index_[ik_joint_index].get_pointer();
		}

		IKJoint *IKArmature::get_ik_joint(const char *ik_joint_name)
		{
			if (!self->ik_joint_exists(ik_joint_name))
				throw mio::IndexException("IKArmature::get_ik_joint() : ik joint with the given name does not exist");
			else
				return self->ik_joints_by_name_[ik_joint_name].get_pointer();
		}

		IKEndEffector *IKArmature::get_end_effector(int end_effector_index)
		{
			if (!self->end_effector_exists(end_effector_index))
				throw mio::IndexException("IKArmature::get_end_effector() : end effector with the given id does not exist");
			else
				return self->end_effectors_by_index_[end_effector_index].get_pointer();
		}

		IKEndEffector *IKArmature::get_end_effector(const char *end_effector_name)
		{
			if (!self->end_effector_exists(end_effector_name))
				throw mio::IndexException("IKArmature::get_end_effector() : end effector with the given name does not exist");
			else
				return self->end_effectors_by_name_[end_effector_name].get_pointer();
		}

		IKJoint *IKArmature::make_ik_joint(int joint_index)
		{
			if (self->ik_joint_exists(joint_index))
				throw mio::ValueException("IKArmature::max_ik_joint() : the joint with the given index is already an IK joint");
			else
			{
				mio::rigging::Joint *joint = self->rest_armature_->get_joint(joint_index);
				Reference<IKJoint> ref = new IKJoint(joint->get_name(), joint_index);
				self->ik_joints_by_index_[joint_index] = ref;
				self->ik_joints_by_name_[joint->get_name()] = ref;
				return ref.get_pointer();
			}
		}

		IKJoint *IKArmature::make_ik_joint(const char *joint_name)
		{
			if (self->ik_joint_exists(joint_name))
				throw mio::ValueException("IKArmature::max_ik_joint() : the joint with the given name is already an IK joint");
			else
			{
				mio::rigging::Joint *joint = self->rest_armature_->get_joint(joint_name);				
				Reference<IKJoint> ref = new IKJoint(joint->get_name(), joint->index);
				self->ik_joints_by_index_[joint->index] = ref;
				self->ik_joints_by_name_[joint->get_name()] = ref;
				return ref.get_pointer();
			}
		}

		IKEndEffector *IKArmature::make_end_effector(int joint_index)
		{
			if (self->end_effector_exists(joint_index))
				throw mio::ValueException("IKArmature::max_ik_joint(): the joint with the given index is already an end effector");
			else
			{
				mio::rigging::Joint *joint = self->rest_armature_->get_joint(joint_index);
				Reference<IKEndEffector> ref = new IKEndEffector(joint->get_name(), joint->index);
				self->end_effectors_by_index_[joint->index] = ref;
				self->end_effectors_by_name_[joint->get_name()] = ref;
				return ref.get_pointer();
			}
		}

		IKEndEffector *IKArmature::make_end_effector(const char *joint_name)
		{
			if (self->end_effector_exists(joint_name))
				throw mio::ValueException("IKArmature::max_ik_joint(): the joint with the given name is already an end effector");
			else
			{
				mio::rigging::Joint *joint = self->rest_armature_->get_joint(joint_name);
				Reference<IKEndEffector> ref = new IKEndEffector(joint->get_name(), joint->index);
				self->end_effectors_by_index_[joint->index] = ref;
				self->end_effectors_by_name_[joint->get_name()] = ref;
				return ref.get_pointer();
			}
		}

		void IKArmature::add_ik_joint_to_end_effector(int ik_joint_index, int end_effector_index)
		{
			IKEndEffector *end_effector = get_end_effector(end_effector_index);
			IKJoint *ik_joint = get_ik_joint(ik_joint_index);
			end_effector->append_ik_joint_index(ik_joint->get_index());
		}
		
		void IKArmature::add_ik_joint_to_end_effector(const char *ik_joint_name, const char *end_effector_name)
		{
			IKEndEffector *end_effector = get_end_effector(end_effector_name);
			IKJoint *ik_joint = get_ik_joint(ik_joint_name);
			end_effector->append_ik_joint_index(ik_joint->get_index());
		}

		mio::rigging::Armature *IKArmature::get_morphed_armature()
		{
			return self->morphed_armature_.get_pointer();
		}

		mio::rigging::Armature *IKArmature::get_rest_armature()
		{
			return self->rest_armature_.get_pointer();
		}

		void IKArmature::IKArmatureImplementation::get_end_effector_position_vector(Eigen::MatrixXd &v)
		{
			v = Eigen::MatrixXd(jacobian_row_count_, 1);			
			FOR(i, (int)relevant_end_effector_list_.size())
			{
				Reference<IKEndEffector> end_effector = relevant_end_effector_list_[i];
				Point3 position = morphed_armature_->_get_joint_world_position(end_effector->get_index());
				
				v(3*i  , 0) = position.x;
				v(3*i+1, 0) = position.y;
				v(3*i+2, 0) = position.z;
			}
		}

		void IKArmature::IKArmatureImplementation::compute_jacobian()
		{
			// Iterate by rows, i.e., by end effectors.
			FOR(end_effector_order, (int)relevant_end_effector_list_.size())
			{
				Reference<IKEndEffector> end_effector = relevant_end_effector_list_[end_effector_order];
				
				// In the inner loop, iterate by column.
				FOR(column_index, (int)jacobian_column_list_.size())
				{
					Reference<IKJoint> ik_joint = jacobian_column_list_[column_index].first;					
					int parameter = jacobian_column_list_[column_index].second;
					double stiffness = ik_joint->get_stiffness(parameter);
					int joint_index = ik_joint->get_index();
					
					// Check whether the joint that owns this component is
					// adjustable with respect to the end effector or not.
					if (!end_effector->is_influenced_by(joint_index))
					{						
						jacobian_(3*end_effector_order  , column_index) = 0.0f;
						jacobian_(3*end_effector_order+1, column_index) = 0.0f;
						jacobian_(3*end_effector_order+2, column_index) = 0.0f;
					}
					else
					{
						// Otherwise, we first retrieve the IK joint and
						// the corresponding joint of the morphed armature.
						mio::rigging::Joint *morphed_joint = morphed_armature_->get_joint(joint_index);
						// We also find the matrix that maps the joint's
						// parent's local space to world space.
						Matrix4x4 joint_parent_to_world_matrix = Matrix4x4::identity();
						if (morphed_joint->parent_index >= 0)
							joint_parent_to_world_matrix = morphed_armature_->_get_joint_to_world_matrix(morphed_joint->parent_index);

						// For each parameter, there are two cases:
						if (parameter == X_TRANSLATE || parameter == Y_TRANSLATE || parameter == Z_TRANSLATE)
						{
							// If the parameter is a translational parameter,                            
							// then we find the axis along with the joint moves
							// in world space.
							Vector3 axis;
							if (parameter == X_TRANSLATE)
								axis = Vector3(1,0,0);
							else if (parameter == Y_TRANSLATE)
								axis = Vector3(0,1,0);
							else
								axis = Vector3(0,0,1);
							axis = joint_parent_to_world_matrix * axis;
							// And assign the axis to the appropriate place in the Jacobian.							
							jacobian_(3*end_effector_order  , column_index) = axis.x * stiffness;
							jacobian_(3*end_effector_order+1, column_index) = axis.y * stiffness;
							jacobian_(3*end_effector_order+2, column_index) = axis.z * stiffness;
						}
						else
						{
							// Otherwise, the parameter must be rotational.
							// We first find the rotation axis in world space.
							Vector3 axis = ik_joint->get_rotation_axis_in_joint_space(parameter);
							mio::rigging::Joint *rest_joint = rest_armature_->get_joint(joint_index);
							axis = rest_joint->orientation.rotate(axis);
							axis = joint_parent_to_world_matrix * axis;

							// We find the world position of the end effector.
							// This has already been computed and stored in
							// end_effector_position_vector_, so we just
							// retrieve the position from it.
							Point3 end_effector_position(
								(float)end_effector_position_vector_(3*end_effector_order  , 0),
								(float)end_effector_position_vector_(3*end_effector_order+1, 0),
								(float)end_effector_position_vector_(3*end_effector_order+2, 0));
							// And then the world position of the joint.
							Point3 joint_position = morphed_armature_->_get_joint_world_position(joint_index);
							// We compute the displacement from the joint position to the
							// end effector.
							Vector3 displacement = end_effector_position - joint_position;
							// The partial derivative is the cross product between
							// the axis and the displacment.
							Vector3 partial = cross(axis, displacement);

							jacobian_(3*end_effector_order  , column_index) = partial.x * stiffness;
							jacobian_(3*end_effector_order+1, column_index) = partial.y * stiffness;
							jacobian_(3*end_effector_order+2, column_index) = partial.z * stiffness;
						}
					}
				}
			}
		}

		void IKArmature::IKArmatureImplementation::clamp_mag(Eigen::MatrixXd &output, Eigen::MatrixXd &input, float D_max)
		{
			output = input;
			int component_count = input.rows() / 3;
			FOR(i, component_count)
			{
				Vector3 v(
					(float)output(3*i  ,0),
					(float)output(3*i+1,0),
					(float)output(3*i+2,0));
				float l = v.length();
				if (l > D_max)
				{
					output(3*i  ,0) = v.x / l * D_max;
					output(3*i+1,0) = v.y / l * D_max;
					output(3*i+2,0) = v.z / l * D_max;
				}
			}
		}

		void IKArmature::IKArmatureImplementation::clamp_rotation_component(Eigen::MatrixXd &v, float gamma_max)
		{
			FOR(i, jacobian_column_count_)
			{
				int parameter = jacobian_column_list_[i].second;
				if (parameter == X_ROTATE || parameter == Y_ROTATE || parameter == Z_ROTATE)
				{
					if (v(i,0) > gamma_max)
						v(i,0) = gamma_max;
					if (v(i,0) < -gamma_max)
						v(i,0) = -gamma_max;
				}
			}
		}

		void IKArmature::IKArmatureImplementation::selectively_damped_least_square()
		{
			// We compute the SVD of the Jacobian.
			if (jacobian_.rows() >= jacobian_.cols())
			{
				Eigen::SVD<Eigen::MatrixXd> svd(jacobian_);
				U = svd.matrixU();
				V = svd.matrixV();
				sigma = svd.singularValues();
			}
			else
			{
				Eigen::SVD<Eigen::MatrixXd> svd(jacobian_.transpose());
				U = svd.matrixV();
				V = svd.matrixU();
				sigma = svd.singularValues();
			}

			// We clamp the error vector.
			Eigen::MatrixXd clamped_error;
			clamp_mag(clamped_error, error_vector_, D_max_);			

			// Find Ns.			
			N.clear();			
			FOR(i, U.cols())
			{
				float N_i = 0.0f;
				FOR(j, jacobian_row_count_ / 3)
				{				
					double u0 = U(3*j,  i);
					double u1 = U(3*j+1,i);					
					double u2 = U(3*j+2,i);					
					N_i += (float)sqrt(u0*u0 + u1*u1 + u2*u2);				
				}
				N.push_back(N_i);
			}

			// Find Ms
			M.clear();
			FOR(i, U.cols())
			{
				double M_i = 0.0f;
				double sigma_i = sigma(i, 0);
				if (sigma_i < EPSILON)
					M.push_back(0.0f);
				else
				{
					double sigma_i_inverse = 1.0 / sigma_i;
					FOR(l, jacobian_row_count_ / 3)
					{
						double M_i_l = 0.0f;
						FOR(j, jacobian_column_count_)
						{
							double jj0 = jacobian_(3*l  ,j);
							double jj1 = jacobian_(3*l+1,j);
							double jj2 = jacobian_(3*l+2,j);
							double rho_l_j = sqrt(jj0*jj0 + jj1*jj1 + jj2*jj2);
							M_i_l += abs(V(j,i)) * rho_l_j;
						}
						M_i += M_i_l;
					}
					M_i *= sigma_i_inverse;
					M.push_back((float)M_i);
				}
			}

			// Find gammas.
			gamma.clear();
			float gamma_max = M_PI / 4;
			FOR(i, U.cols())
			{
				float gamma_i;
				if (M[i] < EPSILON)
					gamma_i = gamma_max;
				else
					gamma_i = gamma_max * MIN(1, N[i]/M[i]);
				gamma.push_back(gamma_i);
			}

			// Find alphas.
			alpha.clear();
			FOR(i, U.cols())
			{
				double alpha_i = 0.0f;
				FOR(j, jacobian_row_count_)				
					alpha_i += U(j,i) * clamped_error(j,0);
				alpha.push_back((float)alpha_i);
			}

			// Find the update.			
			Delta_ = Eigen::MatrixXd::Zero(jacobian_column_count_, 1);
			FOR(i, U.cols())
			{				
				if (sigma(i,0) < EPSILON)
					continue;				
				phi_i = V.col(i) * (alpha[i] / sigma(i,0));
				/*
				FOR(j, (int)jacobian_column_list_.size())
				{
					Reference<IKJoint> ik_joint = jacobian_column_list_[j].first;
					int parameter = jacobian_column_list_[j].second;
					phi_i(j,0) = phi_i(j,0) / ik_joint->get_stiffness(parameter);
				}
				*/
				clamp_rotation_component(phi_i, gamma[i]);
				Delta_ += phi_i;
			}
			
			/*
			// H_ = minus current joint parameters.
			H_ = Eigen::MatrixXd::Zero(jacobian_column_count_, 1);			
			FOR(i, (int)jacobian_column_list_.size())
			{
				Reference<IKJoint> ik_joint = jacobian_column_list_[i].first;
				int parameter = jacobian_column_list_[i].second;

				H_(i,0) = -1 * ik_joint->get_parameter(parameter);
			}
			// Project H_ to the null space, and subtract it form Delta_.
			FOR(i, U.cols())
			{
				if (sigma(i,0) > 0)
					continue;
				double beta_i = (V.col(i).transpose() * H_)(0,0);
				Delta_ += V.col(i) * beta_i;
			}
			*/

			clamp_rotation_component(Delta_, gamma_max);
		}
		
		void IKArmature::IKArmatureImplementation::jacobian_transpose()
		{			
			Eigen::MatrixXd J = jacobian_;
			Eigen::MatrixXd JT = jacobian_.transpose();

			Eigen::MatrixXd J_JT_e = J * JT * error_vector_;
			double nom = (error_vector_.transpose() * J_JT_e)(0,0);
			double denom = (J_JT_e.transpose() * J_JT_e)(0,0);
			double alpha = nom / denom;
			Delta_ = alpha * JT * error_vector_;			
		}
	
		void IKArmature::IKArmatureImplementation::clear_ik_joint_parameters()
		{
			std::map<int, Reference<IKJoint> >::iterator ii = ik_joints_by_index_.begin();
			while (ii != ik_joints_by_index_.end())
			{
				ii->second->clear_parameters();
				++ii;
			}
		}

		void IKArmature::IKArmatureImplementation::solve(IKEndEffectorGoals *goals, 
			int max_iteration, 
			float error_threshold,
			bool reset_ik_joint_parameters)
		{				
			if (reset_ik_joint_parameters)
				clear_ik_joint_parameters();
			
			// Compute the index of columns of the Jacobian.
			// First, we collect the joints we are allowed to adjust, and
			// also the relevant end effectors.
			std::set<int> relevant_ik_joint_index_set;
			relevant_end_effector_list_.clear();
			{
				IKEndEffectorPositionIterator iterator = goals->get_end_effector_position_iterator();
				while (iterator.has_next())
				{					
					NamePositionPair name_position_pair = iterator.get_next();
					const char *end_effector_name = name_position_pair.get_name();

					IKEndEffector *end_effector = NULL;
					if (end_effectors_by_name_.find(end_effector_name) == end_effectors_by_name_.end())
						continue;
					else
						end_effector = end_effectors_by_name_[end_effector_name].get_pointer();					

					relevant_end_effector_list_.push_back(end_effector);
					FOR(j, end_effector->get_ik_joint_index_count())
					{
						relevant_ik_joint_index_set.insert(end_effector->get_ik_joint_index(j));					
					}
				}
			}

			// Second, we add the adjustable joints' degrees of freedom together,
			// and make a list of relevant IK joints.
			jacobian_column_count_ = 0;
			relevant_ik_joint_list_.clear();			
			for(std::set<int>::iterator ii = relevant_ik_joint_index_set.begin();
				ii != relevant_ik_joint_index_set.end();
				++ii)
			{
				Reference<IKJoint> ik_joint = ik_joints_by_index_[*ii];
				relevant_ik_joint_list_.push_back(ik_joint);
				jacobian_column_count_ += ik_joint->get_dof();
			}

			// Third, we compute the index of rows of the Jacobian, which
			// is just three multiplied by the index of relevant end effectors.
			jacobian_row_count_ = 3*(int)relevant_end_effector_list_.size();			

	        // jacobian_column_list_ is a list of 2-tuple (ik_joint, parameter).
		    // Each element in the list corresponds to a column in the Jacobian.	
			jacobian_column_list_.clear();
			for(std::vector<Reference<IKJoint>>::iterator ii=relevant_ik_joint_list_.begin();
				ii != relevant_ik_joint_list_.end();
				++ii)
			{
				Reference<IKJoint> ik_joint = *ii;
				FOR(parameter, IK_JOINT_PARAMETER_COUNT)				
					if (ik_joint->is_parameter_enabled(parameter))					
						jacobian_column_list_.push_back(std::pair<Reference<IKJoint>, int>(ik_joint, parameter));				
			}

			// Initially, the Jacobian is zero.
			jacobian_ = Eigen::MatrixXd::Zero(jacobian_row_count_, jacobian_column_count_);			

			// The goal vector is the column vector of the desired positions.
			goal_vector_ = Eigen::MatrixXd::Zero(jacobian_row_count_, 1);			
			FOR(i, (int)relevant_end_effector_list_.size())
			{
				Reference<IKEndEffector> end_effector = relevant_end_effector_list_[i];
				Point3 desired_position = goals->get_end_effector_position(end_effector->get_name());
				
				goal_vector_(3*i  , 0) = desired_position.x;
				goal_vector_(3*i+1, 0) = desired_position.y;
				goal_vector_(3*i+2, 0) = desired_position.z;
			}

			// end_effector_position_vector_ is the column vector containing
			// the world positions of the end effectors.
			// We update it using the function update_end_effector_position_vector().
			get_end_effector_position_vector(end_effector_position_vector_);
		
			// new_end_effector_position_vector_ is used to hold the
			// positions of the end effectors after one round of IK iteration
			// have passed.
			Eigen::MatrixXd new_end_effector_position_vector;
			// change is used to hold the difference between the
			// positions of the end effectors before and after an iteration.
			Eigen::MatrixXd change;

			// We proceed with the IK iterations.
			FOR(iteration_count, max_iteration)
			{				
				// Compute the error vector.
				error_vector_ = goal_vector_ - end_effector_position_vector_;

				// if the infinity norm of the error is less
				// than the threshold, we are done.
				double max_coeff = 0.0f;
				FOR(i, error_vector_.rows())
				{
					if (fabs(error_vector_(i,0)) > max_coeff)
						max_coeff = fabs(error_vector_(i,0));
				}
				if (max_coeff < error_threshold)
					break;

				// Compute the Jacobian.
				compute_jacobian();
				
				// SDLS algorithm
				selectively_damped_least_square();	
				//jacobian_transpose();

				// Add Delta to joint parameters.
				FOR(i, (int)jacobian_column_list_.size())
				{
					Reference<IKJoint> ik_joint = jacobian_column_list_[i].first;
					int parameter = jacobian_column_list_[i].second;

					float old_value = ik_joint->_get_parameter(parameter);
					float new_value = (float)Delta_(i,0) + old_value;
					ik_joint->_set_parameter(parameter, new_value);
					ik_joint->_clamp_parameter(parameter);
				}

				// Apply changes to the morphed armature.
				FOR(i, (int)relevant_ik_joint_list_.size())
				{
					Reference<IKJoint> ik_joint = relevant_ik_joint_list_[i];
					int joint_index = ik_joint->get_index();					
					mio::rigging::Joint *rest_joint = rest_armature_->get_joint(joint_index);
					mio::rigging::JointChange joint_change = ik_joint->get_joint_change();

					Vector3 new_position = rest_joint->position + joint_change.position;
					Quaternion new_orientation = rest_joint->orientation * joint_change.orientation;					
					morphed_armature_->_set_joint_parameter(joint_index, new_position, new_orientation);
				}

				// Compute the new positions of the end effectors.
				get_end_effector_position_vector(new_end_effector_position_vector);

				/*
				// Calculate the change made by this iteration.
				change = new_end_effector_position_vector - end_effector_position_vector_;
				// If the infinity norm of the change is less
				// than the threshold, the situation is hopeless and we quit.
				FOR(i, change.rows())
				{
					if (fabs(change(i,0)) > max_coeff)
						max_coeff = fabs(error_vector_(i,0));
				}
				if (max_coeff < error_threshold)
					break;				
				// Otherwise, we change the world positions of the end
				// effectors to the new positions.
				*/

				end_effector_position_vector_ = new_end_effector_position_vector;
			}			
		}

		void IKArmature::solve(IKEndEffectorGoals *goals, 
			int max_iteration, 
			float error_threshold,
			bool reset_ik_joint_parameters)
		{
			self->solve(goals, max_iteration, error_threshold, reset_ik_joint_parameters);
		}		

		void IKArmature::IKArmatureImplementation::morph_morphed_armature(mio::rigging::Pose *pose)
		{
			FOR(joint_index, rest_armature_->get_joint_count())
			{
				mio::rigging::Joint *joint = rest_armature_->_get_joint(joint_index);
				mio::rigging::JointChange joint_change = pose->get_joint_change(joint->get_name());

				Vector3 new_position = joint->position + joint_change.position;
				Quaternion new_orientation = joint->orientation * joint_change.orientation;				
				morphed_armature_->_set_joint_parameter(joint_index, new_position, new_orientation);
			}
		}

		void IKArmature::solve(IKPose *pose, int max_iteration, float error_threshold)
		{			
			self->morph_morphed_armature(pose->get_normal_pose());
			std::map<int, Reference<IKJoint> >::iterator ii = self->ik_joints_by_index_.begin();
			while (ii != self->ik_joints_by_index_.end())
			{
				IKJoint *ik_joint = ii->second.get_pointer();
				ik_joint->set_according_to_joint_change(pose->get_normal_pose()->get_joint_change(ik_joint->get_name()));
				++ii;
			}
			self->solve(pose->get_end_effector_goals(), max_iteration, error_threshold, false);
		}

		void IKArmature::solve_for_pose(mio::rigging::Pose &result_pose, IKPose *pose, 
			int max_iteration, float error_threshold)
		{									
			solve(pose, max_iteration, error_threshold);			
			result_pose.clear();			
			self->rest_armature_->get_pose(result_pose, *self->morphed_armature_.get_pointer());			
		}				

		IKEndEffectorIterator::IKEndEffectorIterator()
		{
			ik_armature_ = NULL;
		}

		IKEndEffectorIterator::IKEndEffectorIterator(IKArmature *ik_armature)
		{
			ik_armature_ = ik_armature;
			current_ = ik_armature_->self->end_effectors_by_index_.begin();
		}

		IKEndEffectorIterator::~IKEndEffectorIterator()
		{
		}

		bool IKEndEffectorIterator::has_next()
		{
			return current_ != ik_armature_->self->end_effectors_by_index_.end();
		}

		IKEndEffector *IKEndEffectorIterator::get_next()
		{
			IKEndEffector *result = current_->second.get_pointer();
			++current_;
			return result;
		}

		IKJointIterator::IKJointIterator()
		{
			ik_armature_ = NULL;
		}

		IKJointIterator::IKJointIterator(IKArmature *ik_armature)
		{
			ik_armature_ = ik_armature;
			current_ = ik_armature_->self->ik_joints_by_index_.begin();
		}

		IKJointIterator::~IKJointIterator()
		{
		}

		bool IKJointIterator::has_next()
		{
			return current_ != ik_armature_->self->ik_joints_by_index_.end();
		}

		IKJoint *IKJointIterator::get_next()
		{
			IKJoint *result = current_->second.get_pointer();
			++current_;
			return result;
		}

		mio::ik::IKEndEffectorIterator IKArmature::get_end_effector_iterator()
		{
			return IKEndEffectorIterator(this);
		}

		mio::ik::IKJointIterator IKArmature::get_ik_joint_iterator()
		{
			return IKJointIterator(this);
		}
    }
}