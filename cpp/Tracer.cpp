#include "Tracer.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


//
// init static variables
//
char*       Tracer::grpenv      = 0;
int         Tracer::tracelevel  = 0;
bool        Tracer::onlyflag    = false;
bool        Tracer::envchecked  = false;
unsigned    Tracer::tracecount  = 0;


//
// ctor
//
// print a message to stderr if the passed 'condition' variable
// is non-zero, and if this Tracer object SHOULD be printing, based
// on the TRACExx environment varialbes
//
Tracer::Tracer(bool condition, char *aGroup, int aLevel, char *format, ...)
    : group(0), level(aLevel), serial(0), usecount(0)
{
    // variable arg list
    va_list arg_list;

    // copy the group
    if (aGroup)
    {
        group = new char[strlen(aGroup) + 1];
        strcpy(group, aGroup);
    }

    // only do this once, since the call to getenv() is relatively expenensive
    if (!envchecked)
    {
        envchecked = true;
        CheckEnvironment();
    }

    // are all the pointers non-null
    if (group && grpenv)
    {
        // is our group contained in the list, or is the environment variable set to "ALL"?
        if (strstr(grpenv, group) || strstr(grpenv, "ALL"))
        {
            // is our level less or equal to the tracelevel environment variable?
            if ( (onlyflag && (level == tracelevel)) || (!onlyflag && (level <= tracelevel)) )
            {
                // a non-zero value for 'serial' not only uniquely identifies this tracer,
                // but also doubles as a flag to indicate that this Tracer SHOULD print
                // the tracecount variable is a static class value
                serial = ++tracecount;

                // is the 'condition' variable non-zero?
                if (condition)
                {
                    // print!
                    va_start(arg_list, format);
                    fprintf(stderr, "Tracer: [%d][%s, %d] ", serial, group, level);
                    vfprintf(stderr, format, arg_list);
                    fprintf(stderr, "\n");
                    va_end(arg_list);
                }
            }
        }
    }
}


//
// dtor
//
Tracer::~Tracer()
{
    // print a closing message, if the serial number is non-zero
    if ( serial && (usecount > 0) )
    {
        fprintf(stderr, "Tracer: [%d][%s, %d] -exit-", serial, group, level);
        fprintf(stderr, "\n");
    }

    // clean up
    delete[] group;
}


//
// because calls to getenv() are expensive, this function is only
// called once, and the results are saved in static class variables
//
void Tracer::CheckEnvironment()
{
    // get the TRACEGROUP's from the environment
    // get the TRACELEVEL from the environment
    // get the TRACEONLY from the environment
    grpenv = getenv("TRACEGROUP");
    char* levenv = getenv("TRACELEVEL");
    char* onlyenv = getenv("TRACEONLY");

    if (levenv)
        tracelevel = atoi(levenv);

    if (onlyenv)
    {
        if (0 == strcmp(onlyenv, "TRUE") || (0 == strcmp(onlyenv, "true")))
        {
            onlyflag = true;
        }
    }
}



//
// print a message to stderr if the passed 'condition' variable
// is non-zero, and if this Tracer object SHOULD be printing, based
// on the TRACExx environment varialbes
//
void Tracer::Print(bool condition, char *format, ...)
{
    // variable arg list
    va_list arg_list;

    // print message if 'condition' is true, and if this Tracer SHOULD print
    if (condition && serial)
    {
        // print!
        va_start(arg_list, format);
        fprintf(stderr, "Tracer: [%d][%s, %d] ", serial, group, level);
        vfprintf(stderr, format, arg_list);
        fprintf(stderr, "\n");
        va_end(arg_list);

        // increment use counter
        ++usecount;
    }
}

#define TESTTRACER
#ifdef TESTTRACER


class Foo
{
public:
    void FooFunction()
    {
        // Tracer for an entering message - set group to "Foo" and level to 5
        Tracer(true, "Foo", 5, "Entering FooFunction()");

        // ...do some Foo stuff.  Perhaps there could be an error condition involved
        // indicated with a boolean someErrorFlag.
        bool someErrorFlag = true;
        Tracer(someErrorFlag, "Foo", 5, "Encountered an error condition");

        // higher level of detail, group is still set to "Foo" but level is now 10
        Tracer tt(true, "Foo", 10, "Doing some detailed calculations");
        for (int i = 0; i < 10; i++)
            tt.Print(true, "Iteration %d", i);
    }
};

class Bar
{
public:
    void BarFunction()
    {
        // Tracer for an entering message - set group to "Bar" and level to 5
        Tracer(true, "Bar", 5, "Entering BarFunction()");

        // ...more Bar stuff 
    }
};


int main()
{
    //
    // to turn on tracer messages for these two instances, depending on what you wish to 
    // see at run time, do one of the following at the command line prior to execution
    //          export TRACEGROUP=Foo
    //          export TRACEGROUP=Bar
    //          export TRACEGROUP=Foo,Bar
    //          export TRACEGROUP=ALL
    //
    // also need to set the desired level of detail. Do something like this, depending on
    // how much detail you wish to see
    //          export TRACELEVEL=5
    //          export TRACELEVEL=10
    //
    // Setting TRACELEVEL to 10 will normally show all tracers with levels <= 10
    // if you ONLY want to see tracers of level 10, you would do
    //          export TRACELEVEL=10
    //          export TRACEONLY=TRUE
    //

    Foo f;
    Bar b;

    f.FooFunction();
    b.BarFunction();

    //
    // sample stderr output, with TRACEGROUP=Foo,Bar and TRACELEVEL=5
    //
    //          Tracer: [1][Foo, 5] Entering FooFunction()
    //          Tracer: [2][Foo, 5] Encountered an error condition
    //          Tracer: [3][Bar, 5] Entering BarFunction()
    //
    // sample stderr output, with TRACEGROUP=Foo,Bar and TRACELEVEL=10
    //
    //          Tracer: [1][Foo, 5] Entering FooFunction()
    //          Tracer: [2][Foo, 5] Encountered an error condition
    //          Tracer: [3][Foo, 10] Doing some detailed calculations
    //          Tracer: [3][Foo, 10] Iteration 0
    //          Tracer: [3][Foo, 10] Iteration 1
    //          Tracer: [3][Foo, 10] Iteration 2
    //          Tracer: [3][Foo, 10] Iteration 3
    //          Tracer: [3][Foo, 10] Iteration 4
    //          Tracer: [3][Foo, 10] Iteration 5
    //          Tracer: [3][Foo, 10] Iteration 6
    //          Tracer: [3][Foo, 10] Iteration 7
    //          Tracer: [3][Foo, 10] Iteration 8
    //          Tracer: [3][Foo, 10] Iteration 9
    //          Tracer: [3][Foo, 10] -exit-
    //          Tracer: [4][Bar, 5] Entering BarFunction()
    //
    // setting TRACEONLY=TRUE and re-running yields
    //
    //          Tracer: [1][Foo, 10] Doing some detailed calculations
    //          Tracer: [1][Foo, 10] Iteration 0
    //          Tracer: [1][Foo, 10] Iteration 1
    //          Tracer: [1][Foo, 10] Iteration 2
    //          Tracer: [1][Foo, 10] Iteration 3
    //          Tracer: [1][Foo, 10] Iteration 4
    //          Tracer: [1][Foo, 10] Iteration 5
    //          Tracer: [1][Foo, 10] Iteration 6
    //          Tracer: [1][Foo, 10] Iteration 7
    //          Tracer: [1][Foo, 10] Iteration 8
    //          Tracer: [1][Foo, 10] Iteration 9
    //          Tracer: [1][Foo, 10] -exit-
    //

    return 0;
}

#endif
