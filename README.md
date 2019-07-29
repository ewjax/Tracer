# Tracer
A class to conditionally output a debugging message to stderr, depending on the settings of several environment variables, which the user can control at run time via the shell command line, prior to execution of the code


//
//    Tracer
//
//    A class to conditionally output a debugging message to stderr, depending on the 
//    settings of several environment variables, which the user can control
//    at run time via the shell command line, prior to execution of the code
// 
//    Outputs a message to stderr if
//
//        - the passed parameter 'condition' is true, and
//        - the passed parameter 'group' is contained in the TRACEGROUP
//          environment variable, and
//        - the passed parameter 'level' compares "favorably" to the
//          value contained in the TRACELEVEL environment variable
// 
//    The phrase "favorably" in the above discussion is in quotes because it
//    can be controlled at runtime.  The default behavior is 'less than or
//    equal to', but it can be changed to 'is exactly equal to' if the
//    environment variable TRACEONLY is set to the string "TRUE".  This is
//    useful when it is desired to use the trace function as a debugging
//    tool inside a specific function, without seeing any other trace output.
//
//    Output is similar to printf, using the format string and a variable
//    number of additional parameters.
//
//    Environment variable summary:
//
//        The TRACEGROUP variable indicates which group(s) of Tracer messages
//        will be printed.  To turn on more than 1 group, separate the desired 
//        groups with a comma, or alternatively, set TRACEGROUP to ALL
//
//        The TRACELEVEL variable will represent the maximum (or exact) trace
//        level which should be printed.
//
//        The TRACEONLY variable will control whether the TRACELEVEL is used
//        as a maximum trace level or an exact trace level.
//
//    Example of environment variable use in the C shell:
//
//        setenv TRACEGROUP CXMT,AWB
//        setenv TRACELEVEL 20
//        setenv TRACEONLY TRUE
//
//    Examples for Raspbian bash shell
// 
//        export TRACEGROUP=CXMT,AWB
//        export TRACEGROUP=ALL
// 
//        export TRACELEVEL=20
// 
//        export TRACEONLY=TRUE
//        unset TRACEONLY
// 
//    The intended use, would be to set up Tracer instances throughout the code,
//    with Tracer "groups" set up by module or class, and Tracer "levels" set up
//    with higher level values corresponding to higher amounts of detail
// 
//    Example:
// 
//        class Foo
//        {
//        public:
//            void FooFunction()
//            {
//                // Tracer for an entering message - set group to "Foo" and level to 5
//                Tracer(true, "Foo", 5, "Entering FooFunction()");
//
//                // ...do some Foo stuff.  Perhaps there could be an error condition involved
//                // indicated with a boolean someErrorFlag.
//                bool someErrorFlag = true;
//                Tracer(someErrorFlag, "Foo", 5, "Encountered an error condition");
//
//                // higher level of detail, group is still set to "Foo" but level is now 10
//                Tracer tt(true, "Foo", 10, "Doing some detailed calculations");
//                for (int i = 0; i < 10; i++)
//                    tt.Print(true, "Iteration %d", i);
// 
//            }
//        };
//
//        class Bar
//        {
//        public:
//            void BarFunction()
//            {
//                // Tracer for an entering message - set group to "Bar" and level to 5
//                Tracer(true, "Bar", 5, "Entering BarFunction()");
//
//                // ...more Bar stuff 
//            }
//        };
//
