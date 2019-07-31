# Tracer
A class, in C++ or Python, to conditionally output a debugging message to stderr, depending on the settings of several environment variables, which the user can control at run time via the shell command line, prior to execution of the code

Each instance of Tracer knows:
  - a string corresponding to which group this Tracer belongs to 
  - an integer corresponding to the level of detail at which this Tracer will print
  - a message to be printed
  
Generally, the intended use would be for Tracer instances to be scattered throughout the source code, with their group names set to logical values (perhaps a class names or module names), and with level values generally corresponding to their level of detail (higher values for higher levels of detail).

Tracer instances will print their message to stderr, if the following conditions are true:
  - the Tracer instance group name is contained in the TRACEGROUP environment variable (or if TRACEGROUP is set to ALL), AND
  - the Tracer instance level is less than or equal to the TRACELEVEL environment variable, OR
  - the Tracer instance level is exactly equal to the TRACELEVEL environment variable, IF the TRACEONLY environment variable is also set to TRUE
  
In this way, the develop can easily turn on or off particular TRACER messages scattered throughout the source code at run time, by manipulating the TRACEGROUP, TRACELEVEL, and TRACEONLY environment variables.
