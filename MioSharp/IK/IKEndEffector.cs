using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MioSharp.IK
{
    public class IKEndEffector
    {
        private readonly string name;
        private readonly int index;
        private readonly List<int> ikJointIndices = new List<int>();

        public IKEndEffector(string name, int index)
        {
            this.name = name;
            this.index = index;
        }

        internal void AppendIkJointIndex(int index)
        {
            ikJointIndices.Add(index);
        }
    }
}
