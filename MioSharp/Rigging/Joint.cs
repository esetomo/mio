using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.Rigging
{
    public class Joint
    {
        public Joint(string name, Vector3D position, Quaternion orientation)
        {
            this.name = name;
            this.Position = position;
            this.Orientation = orientation;
        }

        public Joint(Joint other)
        {
            this.name = other.name;
            this.Position = other.Position;
            this.Orientation = other.Orientation;
        }

        public Joint(string name)
        {
            this.name = name;
            this.Position = new Vector3D();
            this.Orientation = new Quaternion();
        }

        private readonly string name;
        public string Name { get { return name; } }
        
        public Vector3D Position { get; set; }
        public Quaternion Orientation { get; set; }

        public int Index { get; set; }
        public int ParentIndex { get; set; }
    }
}
