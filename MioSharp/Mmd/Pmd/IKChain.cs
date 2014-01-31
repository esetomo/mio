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
            affectedBoneIndices = info.Bones.Select((i) => (int)i).ToList();
            constant0 = info.Parm3;
            constant1 = info.Parm4;
        }

        private readonly int ikBoneIndex;
        public int IkBoneIndex { get { return ikBoneIndex; } }

        private readonly int endEffectorIndex;
        public int EndEffectorIndex { get { return endEffectorIndex; } }
        
        private readonly IReadOnlyCollection<int> affectedBoneIndices;
        public IReadOnlyCollection<int> AffectedBoneIndices { get { return affectedBoneIndices; } }

        private readonly UInt16 constant0;
        public UInt16 Constant0 { get { return constant0; } }

        private readonly float constant1;
        public float Constant1 { get { return constant1; } }
    }
}
