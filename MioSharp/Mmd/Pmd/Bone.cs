using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Pmd
{
    class Bone
    {
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
        private readonly int parentIndex;
        private readonly int tipIndex;
        private readonly int boneType;
        private readonly int ikBoneIndex;
        private readonly Point3D position;
    }
}
