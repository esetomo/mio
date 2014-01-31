using MioSharp.IK;
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
        private readonly List<Vertex> verteces = new List<Vertex>();
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
                verteces.Add(new Vertex(vertexInfo, weightInfo));
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

        public IKArmature GetRestArmature()
        {
            throw new NotImplementedException();
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

            var ikJointIndexSet = new Dictionary<object, bool>();
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
                    var jointName = joint.GetName();

                    ikJointIndexSet[ikJointIndex] = true;
                }
            }

            foreach (var ikJointIndex in ikJointIndexSet.Keys)
                ikArmature.MakeIkJoint(ikJointIndex);

            return ikArmature;
        }

        internal Rigging.Pose GetIkLessPose(object vpd_pose)
        {
            throw new NotImplementedException();
        }
    }
}
