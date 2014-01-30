using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Pmd
{
    public class Vertex
    {
        internal Vertex(PmdParser.PmdVertex vertexInfo, PmdParser.PmdWeight weightInfo)
        {
            position = new Point3D(vertexInfo.Data.ElementAt(0),
                          vertexInfo.Data.ElementAt(1),
                          vertexInfo.Data.ElementAt(2));
            normal = new Vector3D(vertexInfo.Data.ElementAt(3),
                                     vertexInfo.Data.ElementAt(4),
                                     vertexInfo.Data.ElementAt(5));
            texCoord = new Point(vertexInfo.Data.ElementAt(6),
                                    vertexInfo.Data.ElementAt(7));

            bone0Number = weightInfo.Data.ElementAt(0);
            bone1Number = weightInfo.Data.ElementAt(1);
            weight = weightInfo.Data.ElementAt(2) / 100.0f;
        }

        private readonly Point3D position;
        private readonly Vector3D normal;
        private readonly Point texCoord;
        private readonly int bone0Number;
        private readonly int bone1Number;
        private readonly float weight;
    }
}
