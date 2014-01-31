using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.Rigging
{
    public class Pose
    {
        private readonly Dictionary<string, JointChange> jointChangesByName = new Dictionary<string, JointChange>();

        public Pose()
        {
        }

        public Pose(Pose other)
        {
            foreach (var item in other.jointChangesByName)
            {
                jointChangesByName[item.Key] = item.Value;
            }
        }

        internal JointChange GetJointChange(string name)
        {
            if (jointChangesByName.ContainsKey(name))
                return jointChangesByName[name];

            return new JointChange(new Vector3D(0, 0, 0), new Quaternion());
        }

        internal void SetJointChange(string name, JointChange jointChange)
        {
            jointChangesByName[name] = jointChange;
        }
    }
}
