using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace MioSharp.Mmd.Vmd
{
    public class VmdParser
    {
        public static readonly Encoding FileEncoding = Encoding.GetEncoding(932);

        public static VmdAnimation Parse(string fileName)
        {
            var raw = Read(fileName);
            return new VmdAnimation(raw);
        }

        private static VmdData Read(string fileName)
        {
            using (var stream = new FileStream(fileName, FileMode.Open))
            using (var f = new BinaryReader(stream, FileEncoding))
            {
                string directory = Path.GetDirectoryName(fileName);
                return new VmdData(f);
            }
        }

        internal class VmdData
        {
            public VmdData(BinaryReader reader)
            {
                header = new VmdHeader(reader);

                int count;
                count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                    boneFrames.Add(new VmdBoneFrame(reader));

                count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                    expressionFrames.Add(new VmdExpressionFrame(reader));
            }

            private readonly VmdHeader header;
            public VmdHeader Header { get { return header; } }

            private readonly List<VmdBoneFrame> boneFrames = new List<VmdBoneFrame>();
            public IReadOnlyCollection<VmdBoneFrame> BoneFrames { get { return boneFrames; } }

            private readonly List<VmdExpressionFrame> expressionFrames = new List<VmdExpressionFrame>();
            public IReadOnlyCollection<VmdExpressionFrame> ExpressionFrames { get { return expressionFrames; } }
        }

        internal class VmdHeader
        {
            public VmdHeader(BinaryReader reader)
            {
                byte[] buf;

                buf = reader.ReadBytes(26);
                headerString = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));

                padding = reader.ReadBytes(4);

                if(headerString == "Vocaloid Motion Data file")
                {
                    pmdModelNameLength = 10;
                }
                else
                {
                    pmdModelNameLength = 20;
                }

                buf = reader.ReadBytes(pmdModelNameLength);
                pmdModelName = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
                pmdModelName = NameDictionary.ToEnglish(pmdModelName);
            }

            private readonly string headerString;
            private readonly byte[] padding;
            private readonly int pmdModelNameLength;
            private readonly string pmdModelName;
        }

        internal class VmdBoneFrame
        {
            public VmdBoneFrame(BinaryReader reader)
            {
                byte[] buf = reader.ReadBytes(15);
                boneName = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
                boneName = NameDictionary.ToEnglish(boneName);

                frameNumber = reader.ReadInt32();

                float x, y, z, w;
                x = reader.ReadSingle();
                y = reader.ReadSingle();
                z = reader.ReadSingle();
                position = new Vector3D(x, y, z);

                x = reader.ReadSingle();
                y = reader.ReadSingle();
                z = reader.ReadSingle();
                w = reader.ReadSingle();
                orientation = new Quaternion(x, y, z, w);

                padding = reader.ReadBytes(64);
            }

            private readonly string boneName;
            public string BoneName { get { return boneName; } }

            private readonly int frameNumber;
            public int FrameNumber { get { return frameNumber; } }

            private readonly Vector3D position;
            public Vector3D Position { get { return position; } }

            private readonly Quaternion orientation;
            public Quaternion Orientation { get { return orientation; } }

            private readonly byte[] padding;
            public byte[] Padding { get { return padding; } }
        }

        internal class VmdExpressionFrame
        {
            public VmdExpressionFrame(BinaryReader reader)
            {
                byte[] buf = reader.ReadBytes(15);
                expressionName = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
                expressionName = NameDictionary.ToEnglish(expressionName);

                frameNumber = reader.ReadInt32();
                weight = reader.ReadSingle();
            }

            private readonly string expressionName;
            private readonly int frameNumber;
            private readonly float weight;
        }
    }
}
