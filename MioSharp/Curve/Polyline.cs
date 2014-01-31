using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MioSharp.Curve
{
    public class Polyline<T>
    {
        private readonly Dictionary<float, T> controlPoints = new Dictionary<float, T>();

        internal void SetControlPoint(float time, T value)
        {
            controlPoints[time] = value;
        }

        internal IEnumerable<ControlPoint<T>> GetControlPoints()
        {
            return from item in controlPoints
                   select new ControlPoint<T>(item.Key, item.Value);
        }

        internal IEnumerator<ControlPoint<T>> GetControlPointIterator()
        {
            return GetControlPoints().GetEnumerator();
        }
    }

    public class ControlPoint<T>
    {
        public ControlPoint(float time, T value)
        {
            this.time = time;
            this.value = value;
        }

        private readonly float time;
        public float Time { get { return time;  } }

        private readonly T value;
        public T Value { get { return value; } }
    }
}
