﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cygwin_Bash_Runner
{
    class Program
    {
        static void Main(string[] args)
        {
            for (int i = 0; i < args.Length; i++)
            {
                Console.WriteLine(args[i]);   
            }
            Console.ReadLine();
        }
    }
}
