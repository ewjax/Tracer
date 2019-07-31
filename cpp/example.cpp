#include "Tracer.h"

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
        const char* someErrorMessage = "Too Close for Comfort!";
        Tracer(someErrorFlag, "Foo", 5, "Encountered an error condition: %s", someErrorMessage);

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
    //          Tracer: [2][Foo, 5] Encountered an error condition: Too Close for Comfort!
    //          Tracer: [3][Bar, 5] Entering BarFunction()
    //
    // sample stderr output, with TRACEGROUP=Foo,Bar and TRACELEVEL=10
    //
    //          Tracer: [1][Foo, 5] Entering FooFunction()
    //          Tracer: [2][Foo, 5] Encountered an error condition: Too Close for Comfort!
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
