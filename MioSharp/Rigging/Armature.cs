using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.Rigging
{
    public class Armature
    {
        private readonly List<Joint> joints = new List<Joint>();
        private readonly Dictionary<string, int> nameToIndexMap = new Dictionary<string, int>();

        public Armature()
        {
        }

        public Armature(Armature other)
        {
            for(int i=0; i<other.joints.Count(); i++)
			{
                Joint newJoint = new Joint(other.joints[i]);
                AppendJoint(newJoint);
			}
        }

        internal Joint GetJoint(string name)
        {
            if (! nameToIndexMap.ContainsKey(name))
				throw new ValueException("Armature::get_joint() : no joint with the given name.");

            int jointIndex = nameToIndexMap[name];
            return joints[jointIndex];
        }

        internal Joint GetJoint(int index)
        {
            if(index < 0 || index >= joints.Count)
                throw new IndexException("Armature::get_joint() : joint index out of range");

            return joints[index];
        }

        internal void AppendJoint(Joint joint)
        {
            var jointName = joint.Name;
            if(nameToIndexMap.ContainsKey(jointName))
                throw new ValueException("Armature::append_joint() : joint with the same name already exist");

            joints.Add(joint);
            joint.Index = joints.Count - 1;
            nameToIndexMap[jointName] = joint.Index;
        }

        internal void SetParent(int childIndex, int parentIndex)
        {
            if (childIndex < 0 || childIndex >= joints.Count())
                throw new IndexException("Armature::set_parent() : child index out of range");
            if (parentIndex < -1 || parentIndex >= joints.Count())
                throw new IndexException("Armature::set_parent() : parent index out of range");
            if (parentIndex == childIndex)
                throw new IndexException("Armature::set_parent() : parent index same as child index");

            joints[childIndex].ParentIndex = parentIndex;
        }

        internal void SetParent(string childName, string parentName)
        {
            Joint child = GetJoint(childName);
            Joint parent = GetJoint(parentName);
            SetParent(child.Index, parent.Index);
        }

        internal int GetJointCount()
        {
            return joints.Count;
        }

        internal void Morph(Armature result, Pose pose)
        {
            if (result.GetJointCount() != GetJointCount())
                throw new ValueException("Armature::morph() : result armature does not have the same number of joints.");

            for (int joint_index = 0; joint_index < GetJointCount(); joint_index++)
            {
                Joint joint = GetJoint(joint_index);
                JointChange joint_change = pose.GetJointChange(joint.Name);

                Vector3D new_position = joint.Position + joint_change.Position;
                Quaternion new_orientation = joint.Orientation * joint_change.Orientation;
                result.SetJointParameter(joint_index, new_position, new_orientation);
            }
        }

        private void SetJointParameter(int index, Vector3D position, Quaternion orientation)
        {
            if (index < 0 || index >= joints.Count)
                throw new IndexException("Armature::set_joint_parameter() : joint index out of range");

            joints[index].Position = position;
            joints[index].Orientation = orientation;
        }

        internal Point3D GetJointWorldPosition(string jointName)
        {
            Joint joint = GetJoint(jointName);
            return GetJointWorldPosition(joint.Index);
        }

        internal Point3D GetJointWorldPosition(int jointIndex)
        {
            if (jointIndex < 0 || jointIndex >= joints.Count)
                throw new IndexException("Armature::get_joint_world_position() : joint index out of range");

            Point3D result = new Point3D(0, 0, 0);
            while (jointIndex >= 0)
            {
                Joint joint = joints[jointIndex];
                var m = Matrix3D.Identity;
                m.Rotate(joint.Orientation);
                result = m.Transform(result);
                result += joint.Position;
                jointIndex = joint.ParentIndex;
            }
            return result;
        }

        internal Quaternion GetJointWorldOrientation(string jointName)
        {
            Joint joint = GetJoint(jointName);
            return GetJointWorldOrientation(joint.Index);
        }

        private Quaternion GetJointWorldOrientation(int jointIndex)
        {
            if (jointIndex < 0 || jointIndex >= joints.Count)
					throw new IndexException("Armature::get_joint_world_orientation() : joint index out of range");

            Quaternion result = new Quaternion();
            while (jointIndex >= 0)
            {
                Joint joint = joints[jointIndex];
                result = joint.Orientation * result;
                jointIndex = joint.ParentIndex;
            }
            return result;
        }

        internal void SetJointParameter(string root_name, Vector3D vector3D, Quaternion root_orientation)
        {
            throw new NotImplementedException();
        }
    }
}
