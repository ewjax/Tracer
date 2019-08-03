
#ifndef __TRACE_H
#define __TRACE_H


#ifdef __cplusplus
extern "C" {
#endif



/*
    trace()

    Outputs a message to stderr if 

        - the passed parameter 'condition' is true, and
        - the passed parameter 'group' is contained in the TRACEGROUP
          environment variable, and
        - the passed parameter 'level' compares "favorably" to the
          value contained in the TRACELEVEL environment variable

    The phrase "favorably" in the above discussion is in quotes because it
    can be controlled at runtime.  The default behavior is 'less than or
    equal to', but it can be changed to 'is exactly equal to' if the 
    environment variable TRACEONLY is set to the string "TRUE".  This is 
    useful when it is desired to use the trace function as a debugging
    tool inside a specific function, without seeing any other trace output.

    Output is similar to printf, using the format string and a variable
    number of additional parameters.

    Environment variable summary:

        The TRACEGROUP variable will be able to contain more than 1 group,
        with desired groups separated with a comma.  

        The TRACELEVEL variable will represent the maximum (or exact) trace
        level which should be printed.

        The TRACEONLY variable will control whether the TRACELEVEL is used
        as a maximum trace level or an exact trace level.

    Example of environment variable use in the C shell:

        setenv TRACEGROUP CXMT,AWB
        setenv TRACELEVEL 20
        setenv TRACEONLY TRUE
*/
void trace(int condition, char *group, int level, char *format, ...);


#ifdef __cplusplus
}
#endif

#endif /* __TRACE_H */
