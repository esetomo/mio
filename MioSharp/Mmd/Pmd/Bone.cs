using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Pmd
{
    class Bone
    {
        public const int BONE_ROTATE = 0;
        public const int BONE_ROTATE_TRANSLATE = 1;
        public const int BONE_IK = 2;
        public const int BONE_UNKNOWN = 3;
        public const int BONE_IK_ROTATION_INFLUENCED = 4;
        public const int BONE_ROTATION_INFLUENCED = 5;
        public const int BONE_IK_TARGET = 6;
        public const int BONE_INVISIBLE = 7;

        internal Bone(PmdParser.PmdBone info)
        {
            name = info.Name;
            parentIndex = info.Data.ElementAt(0);
            tipIndex = info.Data.ElementAt(0);
            boneType = info.Kind;
            ikBoneIndex = info.Knum;
            position = new Point3D(info.Pos.ElementAt(0),
                                   info.Pos.ElementAt(1),
                                   info.Pos.ElementAt(2));
        }

        private readonly string name;
        public string Name { get { return name; } }

        private readonly int parentIndex;
        public int ParentIndex { get { return parentIndex; } }

        private readonly int tipIndex;
        public int TipIndex { get { return tipIndex; } }

        private readonly int boneType;
        public int BoneType { get { return boneType; } }

        private readonly int ikBoneIndex;
        public int IkBoneIndex { get { return ikBoneIndex; } }

        private readonly Point3D position;
        public Point3D Position { get { return position; } }
    }
}
