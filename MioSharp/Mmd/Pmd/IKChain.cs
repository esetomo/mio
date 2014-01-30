using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MioSharp.Mmd.Pmd
{
    public class IKChain
    {
        internal IKChain(PmdParser.PmdIK info)
        {
            ikBoneIndex = info.Parm1.ElementAt(0);
            endEffectorIndex = info.Parm1.ElementAt(1);
            affectedBoneIndices = info.Bones;
            constant0 = info.Parm3;
            constant1 = info.Parm4;
        }

        private readonly int ikBoneIndex;
        private readonly int endEffectorIndex;
        private readonly IReadOnlyCollection<UInt16> affectedBoneIndices;
        private readonly UInt16 constant0;
        private readonly float constant1;
    }
}
