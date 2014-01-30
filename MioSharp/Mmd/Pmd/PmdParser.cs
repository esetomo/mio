using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace MioSharp.Mmd.Pmd
{
    public class PmdParser
    {
        public static readonly Encoding FileEncoding = Encoding.GetEncoding(932);

        public static PmdModel Parse(string fileName)
        {
            var raw = Read(fileName);
            return new PmdModel(raw);
        }

        /// <summary>
        /// Read PMD data from the given file and return it as a dictionary.
        /// </summary>
        /// <param name="fileName">a file name</param>
        private static PmdData Read(string fileName)
        {
            using(var stream = new FileStream(fileName, FileMode.Open))
            using (var f = new BinaryReader(stream, FileEncoding))
            {
                string directory = Path.GetDirectoryName(fileName);
                var dPmd = new PmdData(f);

                foreach (var material in dPmd.MaterialList.Data)
                {
                    if (material.BmpFile.Length > 0)
                        material.BmpFile = Path.GetFullPath(Path.Combine(directory, material.BmpFile));
                }

                return dPmd;
            }
        }

        internal class PmdData
        {
            public PmdData(BinaryReader reader)
            {
                header = new PmdHeader(reader);
                if (header.Magic != "Pmd")
                    throw new Exception("The file is not a valid PMD file.");

                vertexList = new PmdVertexList(reader);
                polyList = new PmdPolyList(reader);
                materialList = new PmdMaterialList(reader);
                boneList = new PmdBoneList(reader);

                ikList = new PmdIKList(reader);
                skinList = new PmdSkinList(reader);
                skinIndexList = new PmdSkinIndexList(reader);
                dispNameList = new PmdDispNameList(reader);
                boneIndexList = new PmdBoneIndexList(reader);
            }

            private readonly PmdHeader header;
            public PmdHeader Header { get { return header; } }

            private readonly PmdVertexList vertexList;
            public PmdVertexList VertexList { get { return vertexList; } }

            private readonly PmdPolyList polyList;
            public PmdPolyList PolyList { get { return polyList; } }

            private readonly PmdMaterialList materialList;
            public PmdMaterialList MaterialList { get { return materialList; } }

            private readonly PmdBoneList boneList;
            public PmdBoneList BoneList { get { return boneList; } }

            private readonly PmdIKList ikList;
            public PmdIKList IKList { get { return ikList; } }

            private readonly PmdSkinList skinList;
            public PmdSkinList SkinList { get { return skinList; } }

            private readonly PmdSkinIndexList skinIndexList;
            public PmdSkinIndexList SkinIndexList { get { return skinIndexList; } }

            private readonly PmdDispNameList dispNameList;
            public PmdDispNameList DispNameList { get { return dispNameList; } }

            private readonly PmdBoneIndexList boneIndexList;
            public PmdBoneIndexList BoneIndexList { get { return boneIndexList; } }
        }

        internal class PmdHeader
        {
            public PmdHeader(BinaryReader reader)
            {
                byte[] buf;

                buf = reader.ReadBytes(3);
                magic = FileEncoding.GetString(buf);

                version = reader.ReadSingle();

                buf = reader.ReadBytes(20);
                modelName = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));

                buf = reader.ReadBytes(256);
                comment = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
            }

            private readonly string magic;
            public string Magic { get { return magic; } }

            private readonly float version;
            public float Version { get { return version; } }

            private readonly string modelName;
            public string ModelName { get { return modelName; } }

            private readonly string comment;
            public string Comment { get { return comment; } }
        }

        internal class PmdVertexList
        {
            public PmdVertexList(BinaryReader reader)
            {
                vertexCount = reader.ReadUInt32();

                for(UInt32 i=0; i<vertexCount; i++)
                {
                    verteces.Add(new PmdVertex(reader));
                    weights.Add(new PmdWeight(reader));
                }
            }

            private readonly UInt32 vertexCount;
            public UInt32 VertexCount { get { return vertexCount; } }

            private readonly List<PmdVertex> verteces = new List<PmdVertex>();
            public IReadOnlyCollection<PmdVertex> Verteces { get { return verteces; } }

            private readonly List<PmdWeight> weights = new List<PmdWeight>();
            public IReadOnlyCollection<PmdWeight> Weights { get { return weights; } }
        }

        internal class PmdVertex
        {
            public PmdVertex(BinaryReader reader)
            {
                for (int i = 0; i < data.Length; i++)
                    data[i] = reader.ReadSingle();
            }

            private readonly float[] data = new float[8];
            public IReadOnlyCollection<float> Data { get { return data; } }
        }

        internal class PmdWeight
        {
            public PmdWeight(BinaryReader reader)
            {
                data[0] = reader.ReadUInt16();
                data[1] = reader.ReadUInt16();
                data[2] = reader.ReadByte();
                data[3] = reader.ReadByte();
            }

            private readonly UInt16[] data = new UInt16[4];
            public IReadOnlyCollection<UInt16> Data { get { return data; } }
        }

        internal class PmdPolyList
        {
            public PmdPolyList(BinaryReader reader)
            {
                count = reader.ReadUInt32();
                for (UInt32 i = 0; i < count; i++)
                    data.Add(reader.ReadUInt16());
            }

            private readonly UInt32 count;
            public UInt32 Count { get { return count; } }

            private readonly List<UInt16> data = new List<UInt16>();
            public IReadOnlyCollection<UInt16> Data { get { return data; } }
        }

        internal class PmdMaterialList
        {
            public PmdMaterialList(BinaryReader reader)
            {
                count = reader.ReadUInt32();
                for (UInt32 i = 0; i < count; i++)
                    data.Add(new PmdMaterial(reader));
            }

            private readonly UInt32 count;
            public UInt32 Count { get { return count; } }

            private readonly List<PmdMaterial> data = new List<PmdMaterial>();
            public IReadOnlyCollection<PmdMaterial> Data { get { return data; } }
        }

        internal class PmdMaterial
        {
            public PmdMaterial(BinaryReader reader)
            {
                for (int i = 0; i < data.Length; i++)
                    data[i] = reader.ReadSingle();

                toonTex = reader.ReadByte();
                toonEdge = reader.ReadByte();
                edge = reader.ReadUInt32();

                byte[] buf = reader.ReadBytes(20);
                BmpFile = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
            }

            private readonly float[] data = new float[11];
            public IReadOnlyCollection<float> Data { get { return data; } }

            private readonly byte toonTex;
            public byte ToonTex { get { return toonTex; } }

            private readonly byte toonEdge;
            public byte ToonEdge { get { return toonEdge; } }

            private readonly UInt32 edge;
            public UInt32 Edge { get { return edge; } }

            public string BmpFile { get; internal set; }
        }

        internal class PmdBoneList
        {
            public PmdBoneList(BinaryReader reader)
            {
                count = reader.ReadUInt16();
                for (UInt16 i = 0; i < count; i++)
                    data.Add(new PmdBone(reader));
            }

            private readonly UInt16 count;
            public UInt16 Count { get { return count; } }

            private readonly List<PmdBone> data = new List<PmdBone>();
            public IReadOnlyCollection<PmdBone> Data { get { return data; } }
        }

        internal class PmdBone
        {
            public PmdBone(BinaryReader reader)
            {
                byte[] buf = reader.ReadBytes(20);
                name = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
                name = NameDictionary.ToEnglish(name);

                for (int i = 0; i < data.Length; i++)
                    data[i] = reader.ReadUInt16();

                kind = reader.ReadByte();
                knum = reader.ReadUInt16();

                for (int i = 0; i < pos.Length; i++)
                    pos[i] = reader.ReadSingle();
            }

            private readonly string name;
            public string Name { get { return name; } }

            private readonly UInt16[] data = new UInt16[2];
            public IReadOnlyCollection<UInt16> Data { get { return data; } }

            private readonly byte kind;
            public byte Kind { get { return kind; } }

            private readonly UInt16 knum;
            public UInt16 Knum { get { return knum; } }

            private readonly float[] pos = new float[3];
            public IReadOnlyCollection<float> Pos { get { return pos; } }
        }

        internal class PmdIKList
        {
            public PmdIKList(BinaryReader reader)
            {
                count = reader.ReadUInt16();
                for (UInt16 i = 0; i < count; i++)
                    data.Add(new PmdIK(reader));
            }

            private readonly UInt16 count;
            public UInt16 Count { get { return count; } }

            private readonly List<PmdIK> data = new List<PmdIK>();
            public IReadOnlyCollection<PmdIK> Data { get { return data; } }
        }

        internal class PmdIK
        {
            public PmdIK(BinaryReader reader)
            {
                for (int i = 0; i < parm1.Length; i++)
                    parm1[i] = reader.ReadUInt16();
                parm2 = reader.ReadByte();
                parm3 = reader.ReadUInt16();
                parm4 = reader.ReadSingle();
                for (int i = 0; i < parm2; i++)
                    bones.Add(reader.ReadUInt16());
            }

            private readonly UInt16[] parm1 = new UInt16[2];
            public IReadOnlyCollection<UInt16> Parm1 { get { return parm1; } }

            private readonly byte parm2;
            public byte Parm2 { get { return parm2; } }

            private readonly UInt16 parm3;
            public UInt16 Parm3 { get { return parm3; } }

            private readonly float parm4;
            public float Parm4 { get { return parm4; } }

            private readonly List<UInt16> bones = new List<ushort>();
            public IReadOnlyCollection<UInt16> Bones { get { return bones; } }
        }

        internal class PmdSkinList
        {
            public PmdSkinList(BinaryReader reader)
            {
                count = reader.ReadUInt16();
                for (UInt16 i = 0; i < count; i++)
                    data.Add(new PmdSkin(reader));
            }

            private readonly UInt16 count;
            public UInt16 Count { get { return count; } }

            private readonly List<PmdSkin> data = new List<PmdSkin>();
            public IReadOnlyCollection<PmdSkin> Data { get { return data; } }
        }

        internal class PmdSkin
        {
            public PmdSkin(BinaryReader reader)
            {
                byte[] buf = reader.ReadBytes(20);
                name = FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0));
                name = NameDictionary.ToEnglish(name);

                skinSize = reader.ReadUInt32();
                parm = reader.ReadByte();

                for (int i = 0; i < skinSize; i++)
                    verts.Add(new PmdSkinVertex(reader));
            }

            private readonly string name;
            public string Name { get { return name; } }

            private readonly UInt32 skinSize;
            public UInt32 SkinSize { get { return skinSize; } }

            private readonly byte parm;
            public byte Parm { get { return parm; } }

            private readonly List<PmdSkinVertex> verts = new List<PmdSkinVertex>();
            public IReadOnlyCollection<PmdSkinVertex> Verts { get { return verts; } }
        }

        internal class PmdSkinVertex
        {
            public PmdSkinVertex(BinaryReader reader)
            {
                data1 = reader.ReadUInt32();

                for (int i = 0; i < data2.Length; i++)
                    data2[i] = reader.ReadSingle();
            }

            private readonly UInt32 data1;
            public UInt32 Data1 { get { return data1; } }

            private readonly float[] data2 = new float[3];
            public IReadOnlyCollection<float> Data2 { get { return data2; } }
        }

        internal class PmdSkinIndexList
        {
            public PmdSkinIndexList(BinaryReader reader)
            {
                count = reader.ReadByte();
                for (byte i = 0; i < count; i++)
                    data.Add(reader.ReadUInt16());
            }

            private readonly byte count;
            public byte Count { get { return count; } }

            private readonly List<UInt16> data = new List<UInt16>();
            public IReadOnlyCollection<UInt16> Data { get { return data; } }
        }

        internal class PmdDispNameList
        {
            public PmdDispNameList(BinaryReader reader)
            {
                count = reader.ReadByte();
                for(byte i = 0; i<count; i++)
                {
                    byte[] buf = reader.ReadBytes(50);
                    data.Add(FileEncoding.GetString(buf, 0, Array.IndexOf(buf, (byte)0)));
                }
            }

            private readonly byte count;
            public byte Count { get { return count; } }

            private readonly List<string> data = new List<string>();
            public IReadOnlyCollection<string> Data { get { return data; } }
        }

        internal class PmdBoneIndexList
        {
            public PmdBoneIndexList(BinaryReader reader)
            {
                count = reader.ReadUInt32();
                for (UInt32 i = 0; i < count; i++)
                    data.Add(new PmdBoneIndex(reader));
            }

            private readonly UInt32 count;
            public UInt32 Count { get { return count; } }

            private readonly List<PmdBoneIndex> data = new List<PmdBoneIndex>();
            public IReadOnlyCollection<PmdBoneIndex> Data { get { return data; } }
        }

        internal class PmdBoneIndex
        {
            public PmdBoneIndex(BinaryReader reader)
            {
                bone = reader.ReadUInt16();
                type = reader.ReadByte();
            }

            private readonly UInt16 bone;
            public UInt16 Bone { get { return bone; } }

            private readonly byte type;
            public byte Type { get { return type; } }
        }
    }
}
