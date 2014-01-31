using MioSharp.Rigging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MioSharp.IK
{
    public class IKArmature
    {
        public IKArmature(IKArmature restArmature)
        {
            throw new NotImplementedException();
        }

        internal IEnumerable<IKJoint> GetIkJoints()
        {
            throw new NotImplementedException();
        }

        internal Joint GetJoint(string p)
        {
            throw new NotImplementedException();
        }

        internal Joint GetJoint(int p)
        {
            throw new NotImplementedException();
        }

        internal IKEndEffector MakeEndEffector(int p)
        {
            throw new NotImplementedException();
        }

        internal void MakeIkJoint(object ik_joint_index)
        {
            throw new NotImplementedException();
        }
    }
}
