using MioSharp.Rigging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MioSharp.Curve
{
    public abstract class Polyline<T>
    {
        private readonly SortedList<float, T> controlPoints = new SortedList<float, T>();        

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

        internal T Evaluate(float time)
        {
            if (controlPoints.Count() == 0)
                return Zero();

            if (controlPoints.Count() == 1)
                return controlPoints.First().Value;

            if (controlPoints.ContainsKey(time))
                return controlPoints[time];

            var first = controlPoints.First();
            if (first.Key > time)
                return first.Value;

            var last = controlPoints.Last();
            if (last.Key < time)
                return last.Value;

            var left = controlPoints.TakeWhile((i) => i.Key < time).Last();
            var right = controlPoints.SkipWhile((i) => i.Key < time).First();
            float x = (time - left.Key) / (right.Key - left.Key);
            return Interpolate(left.Value, right.Value, x);
        }

        internal abstract T Zero();
        internal abstract T Interpolate(T a, T b, float x);
    }

    public class Polyline_JointChange : Polyline<JointChange>
    {
        internal override JointChange Zero()
        {
            throw new NotImplementedException();
        }

        internal override JointChange Interpolate(JointChange a, JointChange b, float x)
        {
            throw new NotImplementedException();
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
