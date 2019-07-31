# Tracer
A class, in C++ or Python, to conditionally output a debugging message to stderr, depending on the settings of several environment variables, which the user can control at run time via the shell command line, prior to execution of the code

    // constructor and Print() member function, C++
    Tracer(bool condition, char *aGroup, int aLevel, char *format, ...);
    void Print(bool condition, char *format, ...);

    # constructor and Print() member function, Python
    def __init__(self, print_flag, group, level, message):
    def Print(self, print_flag, message):

Each instance of Tracer knows:
  - a string corresponding to which group this Tracer belongs to 
  - an integer corresponding to the level of detail at which this Tracer will print
  - a message to be printed
  
Generally, the intended use would be for Tracer instances to be scattered throughout the source code, with their group names set to logical values (perhaps a class names or module names), and with level values generally corresponding to their level of detail (higher values for higher levels of detail).

Tracer instances will print their message to stderr, if the following conditions are true:
  - the boolean 'condition' or 'print_flag' variable is true, and
  - the Tracer instance group name is contained in the TRACEGROUP environment variable (or if TRACEGROUP is set to ALL), and either
  - the Tracer instance level is less than or equal to the TRACELEVEL environment variable, or
  - the Tracer instance level is exactly equal to the TRACELEVEL environment variable, IF the TRACEONLY environment variable is also set to TRUE
  
In this way, the developer can easily turn on or off particular TRACER messages scattered throughout the source code at run time, by manipulating the TRACEGROUP, TRACELEVEL, and TRACEONLY environment variables prior to code execution, and watching the messages printed to stderr.
