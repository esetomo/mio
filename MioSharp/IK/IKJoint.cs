using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;

namespace MioSharp.IK
{
    public class IKJoint
    {
        private readonly int index;
        public int Index { get { return index; } }

        private readonly string name;
        internal string Name { get { return name; } }

        private readonly bool[] enabled = new bool[IKJointParameters.Count];
        private readonly float[] parameters = new float[IKJointParameters.Count];
        private readonly float[] lowerBound = new float[IKJointParameters.Count];
        private readonly float[] upperBound = new float[IKJointParameters.Count];
        private readonly Quaternion rotationAxisToJoint;
        private readonly float[] stiffness = new float[IKJointParameters.Count];

        public IKJoint(string name, int index)
        {
            this.name = name;
            this.index = index;

            for(int i=0; i<IKJointParameters.Count; i++)
            {
                stiffness[i] = 1.0f;
                parameters[i] = 0.0f;
                lowerBound[i] = float.NegativeInfinity;
                upperBound[i] = float.PositiveInfinity;
                enabled[i] = true;
            }
        }

        internal void DisableParameter(int parameter)
        {
			if (parameter < 0 || parameter >= IKJointParameters.Count)
				throw new IndexException("IKJoint::disable_parameter() : parameter index out of range");

            enabled[parameter] = false;
        }

        internal void SetLimit(int parameter, int lowerBound, int upperBound)
        {
            if (parameter < 0 || parameter >= IKJointParameters.Count)
                throw new IndexException("IKJoint::set_limit() : parameter index out of range");

            if (parameter == IKJointParameters.XRotate || parameter == IKJointParameters.YRotate || parameter == IKJointParameters.ZRotate)
            {
                this.lowerBound[parameter] = (float)(lowerBound * Math.PI / 180);
                this.upperBound[parameter] = (float)(upperBound * Math.PI / 180);
            }
            else
            {
                this.lowerBound[parameter] = lowerBound;
                this.upperBound[parameter] = upperBound;
            }
        }
    }
}
