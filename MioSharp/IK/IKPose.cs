using MioSharp.Rigging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MioSharp.IK
{
    public class IKPose
    {
        internal IKPose()
        {
            NormalPose = new Pose();
            EndEffectorGoals = new IKEndEffectorGoals();
        }

        internal Pose NormalPose { get; set; }
        internal IKEndEffectorGoals EndEffectorGoals { get; set; }

        internal void SetNormalPose(Pose chain_normal_pose)
        {
            throw new NotImplementedException();
        }
    }
}
