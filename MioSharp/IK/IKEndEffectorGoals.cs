using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;

namespace MioSharp.IK
{
    public class IKEndEffectorGoals
    {
        private readonly Dictionary<string, NamePositionPair> endEffectorPositions = new Dictionary<string, NamePositionPair>();

        internal void SetEndEffectorPosition(string endEffectorName, Point3D position)
        {
            if(endEffectorPositions.ContainsKey(endEffectorName))
            {
                endEffectorPositions[endEffectorName].Position = position;
                return;
            }

            endEffectorPositions[endEffectorName] = new NamePositionPair(endEffectorName, position);
        }
    }

    public class NamePositionPair
    {
        public NamePositionPair(string name, Point3D position)
        {
            this.name = name;
            this.Position = position;
        }

        private readonly string name;
        public string Name { get { return name; } }

        public Point3D Position { get; set; }
    }
}
