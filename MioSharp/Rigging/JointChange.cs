using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.Rigging
{
    public class JointChange
    {
        public JointChange()
        {
            this.position = new Vector3D();
            this.orientation = new Quaternion();
        }

        public JointChange(Vector3D position, Quaternion orientation)
        {
            this.position = position;
            this.orientation = orientation;
        }

        private readonly Vector3D position;
        public Vector3D Position { get { return position; } }

        private readonly Quaternion orientation;
        public Quaternion Orientation { get { return orientation; } }
    }
}
