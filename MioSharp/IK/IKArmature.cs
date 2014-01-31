using MioSharp.Rigging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.IK
{
    public class IKArmature
    {
        private readonly Armature restArmature;
        private readonly Armature morphedArmature;

        private readonly Dictionary<int, IKEndEffector> endEffectorsByIndex = new Dictionary<int, IKEndEffector>();
        private readonly Dictionary<string, IKEndEffector> endEffectorsByName = new Dictionary<string, IKEndEffector>();

        private readonly Dictionary<int, IKJoint> ikJointsByIndex = new Dictionary<int, IKJoint>();
        private readonly Dictionary<string, IKJoint> ikJointsByName = new Dictionary<string, IKJoint>();

        private readonly float dMax;

        public IKArmature(Armature armature)
        {
            restArmature = armature;
			if (restArmature == null)
			{
				morphedArmature = null;
				dMax = 0.0f;
			}
			else			
			{
				morphedArmature = new Armature(restArmature);
				dMax = 0.0f;

                for(int i=0; i<restArmature.GetJointCount(); i++)
				{
					Joint joint = restArmature.GetJoint(i);
					float l = (float)joint.Position.Length / 2.0f;

					if (dMax < l)
						dMax = l;	
				}
			}
        }

        internal IEnumerable<IKJoint> GetIkJoints()
        {
            return ikJointsByIndex.Values;
        }

        internal IKEndEffector MakeEndEffector(int jointIndex)
        {
            if (EndEffectorExists(jointIndex))
                throw new ValueException("IKArmature::max_ik_joint(): the joint with the given index is already an end effector");

            Joint joint = restArmature.GetJoint(jointIndex);
            IKEndEffector effector = new IKEndEffector(joint.Name, joint.Index);
            endEffectorsByIndex[joint.Index] = effector;
            endEffectorsByName[joint.Name] = effector;

            return effector;
        }

        internal IKEndEffector MakeEndEffector(string jointName)
        {
            if (EndEffectorExists(jointName))
                throw new ValueException("IKArmature::max_ik_joint(): the joint with the given name is already an end effector");

            Joint joint = restArmature.GetJoint(jointName);
            IKEndEffector effector = new IKEndEffector(joint.Name, joint.Index);
            endEffectorsByIndex[joint.Index] = effector;
            endEffectorsByName[joint.Name] = effector;
            return effector;
        }

        internal IKJoint MakeIkJoint(int jointIndex)
        {
            if (IkJointExists(jointIndex))
                throw new ValueException("IKArmature::max_ik_joint() : the joint with the given index is already an IK joint");

            Joint joint = restArmature.GetJoint(jointIndex);
            IKJoint r = new IKJoint(joint.Name, jointIndex);
            ikJointsByIndex[jointIndex] = r;
            ikJointsByName[joint.Name] = r;

            return r;
        }

        internal bool EndEffectorExists(int jointIndex)
        {
            return endEffectorsByIndex.ContainsKey(jointIndex);
        }

        private bool EndEffectorExists(string jointName)
        {
            return endEffectorsByName.ContainsKey(jointName);
        }

        internal bool IkJointExists(int jointIndex)
        {
            return ikJointsByIndex.ContainsKey(jointIndex);
        }

        private bool IkJointExists(string jointName)
        {
            return ikJointsByName.ContainsKey(jointName);
        }

        internal IKJoint MakeIkJoint(string jointName)
        {
            if (IkJointExists(jointName))
                throw new ValueException("IKArmature::max_ik_joint() : the joint with the given name is already an IK joint");

            Joint joint = restArmature.GetJoint(jointName);
            IKJoint ikJoint = new IKJoint(joint.Name, joint.Index);
            ikJointsByIndex[joint.Index] = ikJoint;
            ikJointsByName[joint.Name] = ikJoint;
            return ikJoint;
        }

        internal void AddIkJointToEndEffector(string ikJointName, string endEffectorName)
        {
            IKEndEffector endEffector = GetEndEffector(endEffectorName);
            IKJoint ikJoint = GetIkJoint(ikJointName);
            endEffector.AppendIkJointIndex(ikJoint.Index);
        }

        private IKEndEffector GetEndEffector(string endEffectorName)
        {
            if (!endEffectorsByName.ContainsKey(endEffectorName))
                throw new IndexException("IKArmature::get_end_effector() : end effector with the given name does not exist");

            return endEffectorsByName[endEffectorName];
        }

        internal IKJoint GetIkJoint(string ikJointName)
        {
            if (! IkJointExists(ikJointName))
                throw new IndexException("IKArmature::get_ik_joint() : ik joint with the given name does not exist");

            return ikJointsByName[ikJointName];
        }

        internal Armature GetRestArmature()
        {
            throw new NotImplementedException();
        }

        internal void SolveForPose(Pose result_pose, IKPose chain_ik_pose, int p1, double p2)
        {
            throw new NotImplementedException();
        }
    }
}
