using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Pmd
{
    class VertexModification
    {
        public VertexModification(PmdParser.PmdSkinVertex info)
        {
            vertexIndex = (int)info.Data1;
            displacement = new Vector3D(info.Data2.ElementAt(0),
                                        info.Data2.ElementAt(1),
                                        info.Data2.ElementAt(2));
        }

        private int vertexIndex;
        private Vector3D displacement;
    }
}
