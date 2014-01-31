using MioSharp.IK;
using MioSharp.Mmd.Pmd;
using MioSharp.Rigging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Vpd
{
    public class VpdPose
    {
        private readonly Dictionary<string, JointChange> jointChangeByBoneName = new Dictionary<string, JointChange>();
        public IReadOnlyDictionary<string, JointChange> JointChangeByBoneName { get { return jointChangeByBoneName; } }

        internal void SetJointChange(string boneName, JointChange jointChange)
        {
            jointChangeByBoneName[boneName] = jointChange;
        }

        internal IKPose GetIkPose(PmdModel pmdModel)
        {
            var ikPose = new IKPose();
            var normalPose = ikPose.NormalPose;
            var endEffectorGoals = ikPose.EndEffectorGoals;

            var assignableBoneTypes = new[]{Bone.BONE_ROTATE, 
                                 Bone.BONE_ROTATE_TRANSLATE,                                  
                                 Bone.BONE_IK_ROTATION_INFLUENCED,
                                 Bone.BONE_IK_TARGET};
            JointChange jointChange;

            for (int boneIndex = 0; boneIndex < pmdModel.Bones.Count; boneIndex++)
            {
                var bone = pmdModel.Bones.ElementAt(boneIndex);
                if (assignableBoneTypes.Contains(bone.BoneType))
                {
                    try
                    {
                        jointChange = GetRightHandedJointChange(bone.Name);
                    }
                    catch (KeyNotFoundException)
                    {
                        continue;
                    }
                    normalPose.SetJointChange(bone.Name, jointChange);

                }
                else if (bone.BoneType == Bone.BONE_ROTATION_INFLUENCED)
                {
                    var source_bone = pmdModel.Bones.ElementAt(bone.IkBoneIndex);
                    try
                    {
                        jointChange = GetRightHandedJointChange(source_bone.Name);
                    }
                    catch (KeyNotFoundException)
                    {
                        continue;
                    }
                    normalPose.SetJointChange(bone.Name, jointChange);

                }
                else if (bone.BoneType == Bone.BONE_IK)
                {
                    var ikChain = pmdModel.GetIkChainByIkBoneIndex(boneIndex);
                    var endEffectorBone = pmdModel.Bones.ElementAt(ikChain.EndEffectorIndex);

                    var newPosition = pmdModel.GetMorphedWorldPosition(boneIndex, this);
                    endEffectorGoals.SetEndEffectorPosition(endEffectorBone.Name, newPosition);
                }
            }

            return ikPose;
        }

        internal JointChange GetRightHandedJointChange(string boneName)
        {
            var jointChange = jointChangeByBoneName[boneName];
            var zFlipMatrix = Matrix3D.Identity;
            zFlipMatrix.Scale(new Vector3D(1, 1, -1));
            var orientation = new Quaternion(jointChange.Orientation.X,
                                     jointChange.Orientation.Y,
                                     -jointChange.Orientation.Z,
                                     -jointChange.Orientation.W);
            jointChange = new JointChange(zFlipMatrix.Transform(jointChange.Position), orientation);
            return jointChange;
        }
    }
}
