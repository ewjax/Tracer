/*
 * Copyright (c) 1996 Westinghouse Electric Corporation
 * All Rights Reserved
 * Westinghouse Proprietary - Class 2
 * Description: LPWB Floating Menu Definition
 * History: 5/96 Elliott W. Jackson Initial version
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "trace.h"

void trace(int condition, char *group, int level, char *format, ...)
{
    static char *grpenv = NULL;
    static char *levenv = NULL;

    char *onlyenv = NULL;
    static int onlyflag = 0;

    static int checked = 0;

    int tracelevel;
    va_list arg_list;


    /* get the TRACEGROUP's from the environment */
    /* get the TRACELEVEL from the environment */
    /* get the TRACEONLY from the environment */
    /* only do this once, since the call to getenv() is relatively expenensive */
    if (!checked) 
    {
        checked = 1;

        grpenv = getenv("TRACEGROUP");
        levenv = getenv("TRACELEVEL");
        onlyenv = getenv("TRACEONLY");

        if (onlyenv) 
        {
            if (0 == strcmp(onlyenv, "TRUE")) 
            {
                onlyflag = 1;
            }
        }
    }

    /* are all the pointers non-null? */
    if (group && grpenv && levenv) 
    {
        /* only print if condition is true */
        if (condition) 
        {
            /* is our group contained in the list? */
            if (strstr(grpenv, group) || strstr(grpenv, "ALL")) 
            {
                /* is our level less or equal to the tracelevel? */
                tracelevel = atoi(levenv);
                if ( (onlyflag && (level == tracelevel)) || (!onlyflag && (level <= tracelevel)) )
                {
                    /* print! */
                    va_start(arg_list, format);
                    fprintf(stderr, "Trace: [%s, %d] ", group, level);
                    vfprintf(stderr, format, arg_list);
                    fprintf(stderr, "\n");
                    va_end(arg_list);
                }
            }
        }
    }
}



#define TESTTRACE
#ifdef TESTTRACE


int main()
{
    int i;


    for (i = 0; i < 10; ++i) 
    {
        trace(1, "Us", i, "Got Here, level %d!", i);

    }


    trace(1, "Us", 3, "Got Here, level 3!");
    trace(1, "Us", 4, "Got Here, level 4!");

    trace((1 < 2), "Us", 4, "Conditional, should print");
    trace((3 < 2), "Us", 4, "Conditional, shouldn't print");


    trace(1, "Them", 4, "Got Here, level %d", 4);







    return 0;
}



#endif



