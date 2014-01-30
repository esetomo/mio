using Microsoft.VisualBasic.FileIO;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace MioSharp.Mmd
{
    public class NameDictionary
    {
        private static Dictionary<string, string> toEnglishMap;
        private static Dictionary<string, string> toJapaneseMap;

        static NameDictionary()
        {
            toEnglishMap = new Dictionary<string, string>();
            toJapaneseMap = new Dictionary<string, string>();

            var assembly = Assembly.GetExecutingAssembly();
            var filename = Path.Combine(assembly.Location, @"..\..\..\..\data\mmd\dictionary.txt");

            using(var parser = new TextFieldParser(filename, Encoding.GetEncoding(932)))
            {
                parser.TextFieldType = FieldType.Delimited;
                parser.SetDelimiters(",");

                while (!parser.EndOfData)
                {
                    string[] row = parser.ReadFields();
                    var japaneseName = row[0];
                    var englishName = row[1];

                    toEnglishMap[japaneseName] = englishName;
                    toJapaneseMap[englishName] = japaneseName;
                }
            }
        }

        public static string ToEnglish(string name)
        {
            if (toEnglishMap.ContainsKey(name))
                return toEnglishMap[name];

            return EncodeNonAsciiCharacters(name);
        }

        public static string ToJapanese(string name)
        {
            if (toJapaneseMap.ContainsKey(name))
                return toJapaneseMap[name];

            return DecodeEncodedNonAsciiCharacters(name);
        }

        private static string EncodeNonAsciiCharacters(string value)
        {
            StringBuilder sb = new StringBuilder();
            foreach (char c in value)
            {
                if (c > 127)
                {
                    string encodedValue = "\\u" + ((int)c).ToString("x4");
                    sb.Append(encodedValue);
                }
                else
                {
                    sb.Append(c);
                }
            }
            return sb.ToString();
        }

        private static string DecodeEncodedNonAsciiCharacters(string value)
        {
            return Regex.Replace(
                value,
                @"\\u(?<Value>[a-zA-Z0-9]{4})",
                m => {
                    return ((char)int.Parse(m.Groups["Value"].Value, NumberStyles.HexNumber)).ToString();
                });
        }
    }
}
