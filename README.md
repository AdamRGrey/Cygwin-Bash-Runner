Cygwin-Bash-Runner
==================

I hate windows command prompt. I like C#. I'd *like* to be able to execute a command to log in to bash and execute a file (with arguments!) just by doubleclicking an sh file.

So to get this to work, one should open up command prompt and do this:

```dos
ftype bash_script=<path to>Cygwin-Bash-Runner.exe %1 %*
assoc .sh=bash_script
```
that's not a typo. Do percent one, then percent asterisk.

That *should* associate sh files with the program and pass any additional arguments. I think. idk what else I need to do.
