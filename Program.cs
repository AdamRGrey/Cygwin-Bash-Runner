using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cygwin_Bash_Runner
{
    class Program
    {
        public const string CygpathLocation = "D:\\cygwin\\bin\\cygpath.exe";
        public const string ShellLocation = "D:\\cygwin\\bin\\bash.exe";
        static void Main(string[] args)
        {
            var cygpathd = getOutput(CygpathLocation, args[0]);
            
            var commandargs =" --login -c \"cd \"" + getOutput(CygpathLocation, Directory.GetCurrentDirectory()) + "\"; '" + cygpathd + "' ";
            for (int i = 1; i < args.Length; i++)
            {
                commandargs += args[i];
            }

            new Process { StartInfo = { FileName = ShellLocation, Arguments = commandargs } }.Start();
        }
        public static string getOutput(string program, string arg)
        {
            var toReturn = "";
            var proc = new Process
            {
                StartInfo =
                {
                    UseShellExecute = false, 
                    RedirectStandardOutput = true, 
                    RedirectStandardError = true,
                    FileName = program,
                    Arguments = arg
                },
                EnableRaisingEvents = true
            };
            proc.ErrorDataReceived += (object sender, DataReceivedEventArgs e) => { toReturn += e.Data + " "; };
            proc.OutputDataReceived += (object sender, DataReceivedEventArgs e) => { toReturn += e.Data + " "; };

            proc.Start();
            proc.BeginOutputReadLine();
            proc.BeginErrorReadLine();
            proc.WaitForExit();

            return toReturn.TrimEnd(' ');
        }
    }
}
