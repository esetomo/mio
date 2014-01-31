using MioSharp.IK;
using MioSharp.Mmd.Vpd;
using MioSharp.Rigging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Pmd
{
    public class PmdModel
    {
        private readonly List<Vertex> vertices = new List<Vertex>();
        private readonly int triangleCount;
        private readonly List<int> triangleVertexIndex = new List<int>();
        private readonly List<Material> materials = new List<Material>();
        
        private readonly List<Bone> bones = new List<Bone>();
        internal IReadOnlyCollection<Bone> Bones { get { return bones; } }

        private readonly Dictionary<string, Bone> bonesByName = new Dictionary<string, Bone>();
        internal IReadOnlyDictionary<string, Bone> BonesByName { get { return bonesByName; } }
        internal Bone GetBoneByName(string name)
        {
            return bonesByName[name];
        }

        private readonly List<IKChain> ikChains = new List<IKChain>();
        private readonly Expression baseExpression;
        private readonly List<Expression> expressions = new List<Expression>();

        internal PmdModel(PmdParser.PmdData raw)
        {
            for (int i = 0; i < raw.VertexList.VertexCount; i++)
            {
                var vertexInfo = raw.VertexList.Verteces.ElementAt(i);
                var weightInfo = raw.VertexList.Weights.ElementAt(i);
                vertices.Add(new Vertex(vertexInfo, weightInfo));
            }

            triangleCount = (int)raw.PolyList.Count / 3;

            foreach(var index in raw.PolyList.Data)
                triangleVertexIndex.Add(index);

            foreach(var materialInfo in raw.MaterialList.Data)
                materials.Add(new Material(materialInfo));

            foreach (var boneInfo in raw.BoneList.Data)
            {
                var bone = new Bone(boneInfo); 
                bones.Add(bone);
                bone.Index = bones.Count - 1;
                bonesByName[bone.Name] = bone;
            }

            foreach (var ikChainInfo in raw.IKList.Data)
                ikChains.Add(new IKChain(ikChainInfo));

            foreach(var skinInfo in raw.SkinList.Data)
            {
                var expression = new Expression(skinInfo);

                if (baseExpression == null)
                {
                    baseExpression = expression;
                }
                else
                {
                    expressions.Add(expression);
                }
            }
        }

        public Armature GetRestArmature()
        {
            var armature = new Armature();
            var necessary = Enumerable.Repeat(false, bones.Count).ToArray();

            foreach (var ikChain in ikChains)
            {
                foreach (var index in ikChain.AffectedBoneIndices)
                    necessary[index] = true;
                necessary[ikChain.EndEffectorIndex] = true;
            }

            foreach (var vertex in vertices)
            {
                necessary[vertex.Bone0Number] = true;
                necessary[vertex.Bone1Number] = true;
            }

            for (int boneIndex = 0; boneIndex < bones.Count; boneIndex++)
            {
                var bone = bones[boneIndex];

                if (necessary[boneIndex] ||
                    (new int[]{
                    Bone.BONE_ROTATE, 
                    Bone.BONE_ROTATE_TRANSLATE,       
                    Bone.BONE_IK_ROTATION_INFLUENCED,
                    Bone.BONE_IK_TARGET,
                    Bone.BONE_ROTATION_INFLUENCED
                }.Contains(bone.BoneType)))
                {
                    var current = boneIndex;
                    while (current >= 0)
                    {
                        necessary[current] = true;
                        current = bones[current].ParentIndex;
                    }
                }
            }

            for (int boneIndex = 0; boneIndex < bones.Count; boneIndex++)
            {
                var bone = bones[boneIndex];
                if (necessary[boneIndex])
                {
                    Vector3D position;
                    if (bone.ParentIndex >= 0)
                    {
                        var parent = bones[bone.ParentIndex];
                        position = bone.Position - parent.Position;
                        position = new Vector3D(position.X, position.Y, -position.Z);
                    }
                    else
                    {
                        position = new Vector3D(bone.Position.X, bone.Position.Y, -bone.Position.Z);
                    }
                    var joint = new Joint(bone.Name, position, Quaternion.Identity);
                    armature.AppendJoint(joint);
                }
            }

            for (int boneIndex = 0; boneIndex < bones.Count; boneIndex++)
            {
                var bone = bones[boneIndex];
                Joint joint;
                try
                {
                    joint = armature.GetJoint(bone.Name);
                }
                catch (ValueException)
                {
                    continue;
                }
                if (bone.ParentIndex >= 0)
                    armature.SetParent(joint.Name, bones[bone.ParentIndex].Name);
            }

            return armature;
        }

        internal IKArmature GetIkArmature()
        {
            var restArmature = GetRestArmature();
            var ikArmature = new IKArmature(restArmature);

            var getCorrespondingRestJoint = new Func<int, Joint>((jointIndex) =>
            {
                var bone = bones[jointIndex];
                var restJoint = restArmature.GetJoint(bone.Name);
                return restJoint;
            });

            var ikJointIndexSet = new HashSet<int>();
            foreach (var ikChain in ikChains)
            {
                var endEffectorJoint = getCorrespondingRestJoint(ikChain.EndEffectorIndex);
                var endEffector = ikArmature.MakeEndEffector(endEffectorJoint.Index);

                foreach (var ikBoneIndex in ikChain.AffectedBoneIndices)
                {
                    var ikRestJoint = getCorrespondingRestJoint(ikBoneIndex);
                    var ikJointIndex = ikRestJoint.Index;
                    endEffector.AppendIkJointIndex(ikRestJoint.Index);

                    var joint = restArmature.GetJoint(ikRestJoint.Index);
                    var jointName = joint.Name;

                    ikJointIndexSet.Add(ikJointIndex);
                }
            }

            foreach (var ikJointIndex in ikJointIndexSet)
                ikArmature.MakeIkJoint(ikJointIndex);

            return ikArmature;
        }

        internal Pose GetIkLessPose(VpdPose vpdPose)
        {
            var ikPose = vpdPose.GetIkPose(this);
            var normalPose = new Pose(ikPose.NormalPose);
            var restArmature = GetRestArmature();
            var morphed_armature = new Armature(restArmature);
            restArmature.Morph(morphed_armature, normalPose);

            var ikArmaturesAndOtherInfo = new List<Tuple<int, string, IKArmature, string, string>>();
            for (int i = 0; i < ikChains.Count; i++)
            {
                var ik_chain = ikChains[i];
                var v = GetIkChainIkArmature(i);
                var ik_armature = v.Item1;
                var root_name = v.Item2;
                var end_effector_name = v.Item3;
                ikArmaturesAndOtherInfo.Add(Tuple.Create(i, bones[ik_chain.IkBoneIndex].Name, ik_armature, root_name, end_effector_name));
            }
            ikArmaturesAndOtherInfo = TopologicallySortIkArmatures(ikArmaturesAndOtherInfo);

            foreach (var data_quadruple in ikArmaturesAndOtherInfo)
            {
                var ik_chain_index = data_quadruple.Item1;
                var ik_bone_name = data_quadruple.Item2;
                var ik_armature = data_quadruple.Item3;
                var root_name = data_quadruple.Item4;
                var end_effector_name = data_quadruple.Item5;
                ComputeIkChainIk(normalPose,
                                           restArmature, morphed_armature,
                                           vpdPose,
                                           ik_bone_name, ik_armature,
                                           root_name, end_effector_name);
            }
            return normalPose;
        }

        private Tuple<IKArmature, string, string> GetIkChainIkArmature(int ik_chain_index)
        {
            var ik_chain = ikChains[ik_chain_index];
            var rest_armature = new Armature();

            var joint_indices = ik_chain.AffectedBoneIndices.ToList();
            joint_indices.Add(ik_chain.EndEffectorIndex);

            foreach (var joint_index in joint_indices)
            {
                rest_armature.AppendJoint(new Joint(bones[joint_index].Name));
            }
            string root_name = null;
            foreach (var joint_index in joint_indices)
            {
                var bone = bones[joint_index];
                if (joint_indices.Contains(bone.ParentIndex))
                {
                    var child_name = bones[joint_index].Name;
                    var parent_index = bones[joint_index].ParentIndex;
                    var parent_name = bones[parent_index].Name;
                    rest_armature.SetParent(child_name, parent_name);
                    var joint = rest_armature.GetJoint(child_name);
                    var m = Matrix3D.Identity;
                    m.Scale(new Vector3D(1, 1, -1));
                    var position = m.Transform(bones[joint_index].Position - bones[parent_index].Position);
                    joint.Position = position;
                }
                else
                {
                    root_name = bone.Name;
                }
            }

            var ik_bone_name = bones[ik_chain.IkBoneIndex].Name;
            var ik_armature = new IKArmature(rest_armature);
            var end_effector_name = bones[ik_chain.EndEffectorIndex].Name;
            ik_armature.MakeEndEffector(end_effector_name);
            foreach (var joint_index in ik_chain.AffectedBoneIndices)
            {
                var bone = bones[joint_index];
                var ik_joint = ik_armature.MakeIkJoint(bone.Name);
                ik_armature.AddIkJointToEndEffector(bone.Name, end_effector_name);
                ik_joint.DisableParameter(IKJointParameters.XTranslate);
                ik_joint.DisableParameter(IKJointParameters.YTranslate);
                ik_joint.DisableParameter(IKJointParameters.ZTranslate);
            }

            if (new[] { "leg_ik.R", "leg_ik.L" }.Contains(ik_bone_name))
            {
                IKJoint knee_joint;
                IKJoint leg_joint;

                if (ik_bone_name == "leg_ik.R")
                {
                    knee_joint = ik_armature.GetIkJoint("knee.R");
                    leg_joint = ik_armature.GetIkJoint("leg.R");
                }
                else
                {
                    leg_joint = ik_armature.GetIkJoint("leg.L");
                    knee_joint = ik_armature.GetIkJoint("knee.L");
                }

                knee_joint.DisableParameter(IKJointParameters.YRotate);
                knee_joint.DisableParameter(IKJointParameters.ZRotate);
                knee_joint.SetLimit(IKJointParameters.XRotate, 0, 180);

                leg_joint.DisableParameter(IKJointParameters.YRotate);
                leg_joint.DisableParameter(IKJointParameters.ZRotate);
            }

            return Tuple.Create(ik_armature, root_name, end_effector_name);
        }

        private void MakeEndEffector(string end_effector_name)
        {
            throw new NotImplementedException();
        }

        private class Node
        {
            internal Node(Tuple<int, string, IKArmature, string, string> quintet)
            {
                this.quintet = quintet;
                this.Indegree = 0;
            }

            private readonly Tuple<int, string, IKArmature, string, string> quintet;
            internal Tuple<int, string, IKArmature, string, string> Quintet { get { return quintet; } }

            internal int Indegree { get; set; }

            private readonly List<Node> pointsTo = new List<Node>();
            internal IEnumerable<Node> PointsTo { get { return pointsTo; } }

            internal void AddEdgeTo(Node other)
            {
                other.Indegree += 1;
                pointsTo.Add(other);
            }
        }

        private List<Tuple<int, string, IKArmature, string, string>> TopologicallySortIkArmatures(List<Tuple<int, string, IKArmature, string, string>> ikArmaturesAndOtherInfo)
        {
            var nodes = new List<Node>();
            foreach (var quintet in ikArmaturesAndOtherInfo)
            {
                var newNode = new Node(quintet);
                nodes.Add(newNode);
            }
            foreach (var node in nodes)
            {
                foreach (var other in nodes)
                {
                    if (other == node)
                        continue;
                    var ikChainNode = ikChains[node.Quintet.Item1];
                    var ikChainOther = ikChains[other.Quintet.Item1];

                    if (ikChainOther.AffectedBoneIndices.Contains(ikChainNode.EndEffectorIndex))
                        node.AddEdgeTo(other);
                }
            }
            // topological sort
            ikArmaturesAndOtherInfo = new List<Tuple<int, string, IKArmature, string, string>>();
            while (nodes.Count > 0)
            {
                var indegreeZero = nodes.Where((x) => x.Indegree == 0).ToList();
                foreach (var node in indegreeZero)
                {
                    ikArmaturesAndOtherInfo.Add(node.Quintet);
                    foreach (var other in node.PointsTo)
                        other.Indegree -= 1;
                    nodes.Remove(node);
                }
            }
            return ikArmaturesAndOtherInfo;
        }

        private void ComputeIkChainIk(Pose target_pose, Armature rest_armature, Armature morphed_armature, VpdPose vpd_pose, string ik_bone_name, IKArmature ik_armature, string root_name, string end_effector_name)
        {
            var root_bone = bonesByName[root_name];
            var root_position = morphed_armature.GetJointWorldPosition(root_bone.Name);
            var root_orientation = morphed_armature.GetJointWorldOrientation(root_bone.Name);

            var end_effector_bone = bonesByName[end_effector_name];
            var end_effector_position = morphed_armature.GetJointWorldPosition(end_effector_name);

            var ik_bone = bonesByName[ik_bone_name];
            var goal_position = GetMorphedWorldPosition(ik_bone.Index, vpd_pose);
            var goal_orientation = GetMorphedWorldOrientation(ik_bone.Index, vpd_pose);

            if (new[] { "leg_ik.R", "leg_ik.L", "tiptoe_ik.R", "tiptoe_ik.L" }.Contains(ik_bone_name))
            {
                // calculate the rotation need to rotate the vector
                // (end_effector - root) to (goal - root)
                var root_to_end_effector = end_effector_position - root_position;
                var root_to_end_effector_length = root_to_end_effector.Length;
                var root_to_goal = goal_position - root_position;
                var root_to_goal_length = root_to_goal.Length;
                var dot_prod = Vector3D.DotProduct(root_to_end_effector, root_to_goal);

                if (root_to_end_effector_length > 0.00001)
                    dot_prod /= root_to_end_effector_length;
                if (root_to_goal_length > 0.00001)
                    dot_prod /= root_to_goal_length;

                if (Math.Abs(dot_prod - 1) > 0.0001)
                {
                    var cos_theta = dot_prod;
                    var theta = Math.Acos(cos_theta);
                    var axis = Vector3D.CrossProduct(root_to_end_effector, root_to_goal);
                    axis.Normalize();

                    var rotation = new Quaternion(axis, theta * 180 / Math.PI);

                    root_orientation = rotation * root_orientation;
                }
            }
            var chain_rest_armature = ik_armature.GetRestArmature();
            chain_rest_armature.SetJointParameter(root_name,
                                                    (Vector3D)root_position,
                                                    root_orientation);

            // create a pose with joint changes
            // derived from the VPD pose. 
            var chain_normal_pose = new Pose();
            foreach (var ik_joint in ik_armature.GetIkJoints())
            {
                var ik_joint_name = ik_joint.Name;
                chain_normal_pose.SetJointChange(
                    ik_joint_name,
                    target_pose.GetJointChange(ik_joint_name));
            }
            chain_normal_pose.SetJointChange(root_name, new JointChange());

            var chain_ik_pose = new IKPose();
            chain_ik_pose.SetNormalPose(chain_normal_pose);
            chain_ik_pose.EndEffectorGoals.SetEndEffectorPosition(end_effector_name, goal_position);

            var result_pose = new Pose();
            ik_armature.SolveForPose(result_pose, chain_ik_pose, 30, 0.0001);

            foreach (var ik_joint in ik_armature.GetIkJoints())
            {
                var joint_change = result_pose.GetJointChange(ik_joint.Name);
                target_pose.SetJointChange(ik_joint.Name, joint_change);
            }

            if (new[] { "leg_ik.R", "leg_ik.L", "tiptoe_ik.R", "tiptoe_ik.L" }.Contains(ik_bone_name))
            {
                // Fix the root joint change so that it is given
                // with respect to its parent.
                var root_parent_orientation = morphed_armature.GetJointWorldOrientation(bones[bonesByName[root_name].ParentIndex].Name);
                root_parent_orientation.Invert();
                var fixed_root_orientation = root_parent_orientation * root_orientation * result_pose.GetJointChange(root_name).Orientation;
                target_pose.SetJointChange(root_name,
                                             new JointChange(new Vector3D(0, 0, 0), fixed_root_orientation));
            }

            rest_armature.Morph(morphed_armature, target_pose);
        }

        private object GetMorphedWorldOrientation(int p, VpdPose vpd_pose)
        {
            throw new NotImplementedException();
        }

        internal IKChain GetIkChainByIkBoneIndex(int ikBoneIndex)
        {
            return ikChains.First((ikChain) => ikChain.IkBoneIndex == ikBoneIndex);
        }

        internal Point3D GetMorphedWorldPosition(int boneIndex, VpdPose vpdPose)
        {
            var bone = bones[boneIndex];
            var result = new Point3D(0, 0, 0);
            Matrix3D m;
            while (bone != null)
            {
                if (vpdPose.JointChangeByBoneName.ContainsKey(bone.Name))
                {
                    var jointChange = vpdPose.GetRightHandedJointChange(bone.Name);
                    m = Matrix3D.Identity;
                    m.Rotate(jointChange.Orientation);
                    result = m.Transform(result);
                    result += jointChange.Position;
                }
                m = Matrix3D.Identity;
                m.Scale(new Vector3D(1, 1, -1));
                result += m.Transform(GetBoneDisplacementFromParent(boneIndex));
                if (bone.ParentIndex >= 0)
                {
                    boneIndex = bone.ParentIndex;
                    bone = bones[bone.ParentIndex];
                }
                else
                {
                    bone = null;
                }
            }
            return result;
        }

        private Vector3D GetBoneDisplacementFromParent(int boneIndex)
        {
            var bone = bones[boneIndex];
            if (bone.ParentIndex >= 0)
            {
                var parent = bones[bone.ParentIndex];
                return bone.Position - parent.Position;
            }
            return (Vector3D)bone.Position;
        }
    }
}
