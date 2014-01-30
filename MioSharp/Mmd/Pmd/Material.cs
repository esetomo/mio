using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace MioSharp.Mmd.Pmd
{
    public class Material
    {
        internal Material(PmdParser.PmdMaterial info)
        {
            diffuse = Color.FromScRgb(info.Data.ElementAt(3),
                              info.Data.ElementAt(0),
                              info.Data.ElementAt(1),
                              info.Data.ElementAt(2));
            shininess = info.Data.ElementAt(4);
            specular = Color.FromScRgb(diffuse.ScA,
                                           info.Data.ElementAt(5),
                                           info.Data.ElementAt(6),
                                           info.Data.ElementAt(7));
            ambient = Color.FromScRgb(diffuse.ScA,
                                          info.Data.ElementAt(8),
                                          info.Data.ElementAt(9),
                                          info.Data.ElementAt(10));
            shaderNumber = info.ToonTex;
            shaderEdgeNumber = info.ToonEdge;
            triangleCount = (int)info.Edge / 3;
            textureFilename = info.BmpFile;
        }

        private readonly Color diffuse;
        private readonly float shininess;
        private readonly Color specular;
        private readonly Color ambient;
        private readonly int shaderNumber;
        private readonly int shaderEdgeNumber;
        private readonly int triangleCount;
        private readonly string textureFilename;
    }
}
