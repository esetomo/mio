using CommandLine;
using CommandLine.Text;
using MioSharp.Exporter;
using MioSharp.Mmd.Pmd;
using MioSharp.Mmd.Vmd;
using System;
using System.Collections.Generic;
using System.IO;

// C# Version of vmd2bvh.py
namespace Vmd2Bvh
{
    class Options
    {
        [Option('l', "lang", 
            HelpText = "the language to write joint names in ('english' or 'japanese')",
            DefaultValue="english")]
        public string Language { get; set; }

        [Option('s', "scale",
            HelpText="how much to scale the skeleton",
            DefaultValue=1.0f)]
        public float Scale { get; set; }

        [ParserState]
        public IParserState LastParserState { get; set; }

        [ValueList(typeof(List<string>))]
        public IList<string> Files { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this,
                (HelpText current) => HelpText.DefaultParsingErrorsHandler(this, current));
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                var options = new Options();
                if (!CommandLine.Parser.Default.ParseArguments(args, options))
                    Environment.Exit(0);

                if (options.Language != "english" && options.Language != "japanese")
                {
                    Console.WriteLine("Language must be 'english' or 'japanese' (without the quote)");
                    Environment.Exit(0);
                }

                if (options.Scale < 0f)
                {
                    Console.WriteLine("The scaling factor must be a positive real number.");
                    Environment.Exit(0);
                }

                if (options.Files.Count < 2)
                {
                    Console.WriteLine(options.GetUsage());
                    Environment.Exit(0);
                }

                Console.WriteLine("Loading PMD file {0}...", options.Files[0]);
                var pmdModel = PmdParser.Parse(options.Files[0]);

                Console.WriteLine("Loading VMD file...");
                var vmdAnimation = VmdParser.Parse(options.Files[1]);

                int firstFrame = vmdAnimation.GetFirstFrame();
                int lastFrame = vmdAnimation.GetLastFrame();

                var poseAnimation = vmdAnimation.GetIkLessPoseAnimation(pmdModel, (frame) =>
                {
                    Console.WriteLine("Frame #{0} out of {1}", frame, lastFrame - firstFrame + 1);
                    return true;
                });

                var exportedContent = BvhExporter.Export(pmdModel.GetRestArmature(),
                                                         poseAnimation,
                                                         options.Language,
                                                         30,
                                                         options.Scale);

                string bvhFileName;

                if (options.Files.Count < 3)
                {
                    var modelName = Path.GetFileNameWithoutExtension(options.Files[0]);
                    var animationPath = Path.GetFullPath(options.Files[1]);
                    var animationName = Path.GetFileNameWithoutExtension(animationPath);
                    var animationDir = Path.GetDirectoryName(animationPath);
                    bvhFileName = Path.Combine(animationDir, modelName + "-" + animationName + ".bvh");
                }
                else
                {
                    bvhFileName = Path.GetFullPath(options.Files[2]);
                }

                Console.WriteLine("Saving BVH data to {0}", bvhFileName);

                using (StreamWriter writer = new StreamWriter(bvhFileName))
                {
                    writer.Write(exportedContent);
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}
