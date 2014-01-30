using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MioSharp.Curve
{
    public class Polyline<T>
    {
        public IEnumerator<ControlPoint<T>> GetControlPointIterator()
        {
            throw new NotImplementedException();
        }

        internal void SetControlPoint(double earliestTime, T jointChange)
        {
            throw new NotImplementedException();
        }

        internal IEnumerable<ControlPoint<T>> GetControlPoints()
        {
            throw new NotImplementedException();
        }
    }

    public class ControlPoint<T>
    {
        public double Time { get { throw new NotImplementedException();  } }
        public T Value { get { throw new NotImplementedException(); } }
    }
}
