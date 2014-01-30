using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MioSharp.Mmd.Pmd
{
    public class Expression
    {
        internal Expression(PmdParser.PmdSkin info)
        {
            name = info.Name;
            foreach (var v in info.Verts)
                vertexModifications.Add(new VertexModification(v));
        }

        private readonly string name;
        private readonly List<VertexModification> vertexModifications = new List<VertexModification>();
    }
}
