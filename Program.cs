using System.Diagnostics;
using System.IO;

namespace Cygwin_Bash_Runner
{
    // ReSharper disable once ClassNeverInstantiated.Global
    // ReSharper disable once InternalMembersMustHaveComments
    class Program
    {
        /// <summary>
        /// where is cygpath
        /// </summary>
        private const string CygpathLocation = "D:\\cygwin\\bin\\cygpath.exe";
        /// <summary>
        /// where is the bash
        /// </summary>
        private const string ShellLocation = "D:\\cygwin\\bin\\bash.exe";

        /// <summary>
        /// program entry point
        /// </summary>
        /// <param name="args">0 should be the script to run, 1+ will just be passed along</param>
        static void Main(string[] args)
        {
            var cygpathd = GetOutput(CygpathLocation, args[0]);
            
            var commandargs =" --login -c \"cd \"" + GetOutput(CygpathLocation, Directory.GetCurrentDirectory()) + "\"; '" + cygpathd + "' ";
            for (int i = 1; i < args.Length; i++)
            {
                commandargs += args[i];
            }

            new Process { StartInfo = { FileName = ShellLocation, Arguments = commandargs } }.Start();
        }

        /// <summary>
        /// run <paramref name="program"/> with argument string <paramref name="arg"/>.
        /// </summary>
        /// <returns>the output of <paramref name="program"/></returns>
        public static string GetOutput(string program, string arg)
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
            proc.ErrorDataReceived += (sender, e) => { toReturn += e.Data + " "; };
            proc.OutputDataReceived += (sender, e) => { toReturn += e.Data + " "; };

            proc.Start();
            proc.BeginOutputReadLine();
            proc.BeginErrorReadLine();
            proc.WaitForExit();

            return toReturn.TrimEnd(' ');
        }
    }

}
