import os
import sys

#
#    Tracer
#
#    A class to conditionally output a debugging message to stderr, depending on the 
#    settings of several environment variables, which the user can control
#    at run time via the shell command line, prior to execution of the code
# 
#    Outputs a message to stderr if
#
#        - the passed parameter 'condition' is true, and
#        - the passed parameter 'group' is contained in the TRACEGROUP
#          environment variable, and
#        - the passed parameter 'level' compares "favorably" to the
#          value contained in the TRACELEVEL environment variable
# 
#    The phrase "favorably" in the above discussion is in quotes because it
#    can be controlled at runtime.  The default behavior is 'less than or
#    equal to', but it can be changed to 'is exactly equal to' if the
#    environment variable TRACEONLY is set to the string "TRUE".  This is
#    useful when it is desired to use the trace function as a debugging
#    tool inside a specific function, without seeing any other trace output.
#
#    Environment variable summary:
#
#        The TRACEGROUP variable indicates which group(s) of Tracer messages
#        will be printed.  To turn on more than 1 group, separate the desired 
#        groups with a comma, or alternatively, set TRACEGROUP to ALL
#
#        The TRACELEVEL variable will represent the maximum (or exact) trace
#        level which should be printed.
#
#        The TRACEONLY variable will control whether the TRACELEVEL is used
#        as a maximum trace level or an exact trace level.
#
#    Example of environment variable use in the C shell:
#
#        setenv TRACEGROUP CXMT,AWB
#        setenv TRACELEVEL 20
#        setenv TRACEONLY TRUE
#
#    Examples for Raspbian bash shell
# 
#        export TRACEGROUP=CXMT,AWB
#        export TRACEGROUP=ALL
# 
#        export TRACELEVEL=20
# 
#        export TRACEONLY=TRUE
#        unset TRACEONLY
# 
#    The intended use, would be to set up Tracer instances throughout the code,
#    with Tracer "groups" set up by module or class, and Tracer "levels" set up
#    with higher level values corresponding to higher amounts of detail
# 
#    Example:
# 
#            class Foo:
#
#                def FooFunction(self):
#
#                    # Tracer for an entering message - set group to "Foo" and level to 5
#                    Tracer.Tracer(True, 'Foo', 5, 'Entering FooFunction()');
#
#
#                    # ...do some Foo stuff.  Perhaps there could be an error condition involved
#                    # indicated with a boolean someErrorFlag.
#                    someErrorFlag = True
#                    Tracer.Tracer(someErrorFlag, 'Foo', 5, 'Encountered an error condition');
#
#                    # higher level of detail, group is still set to "Foo" but level is now 10
#                    tt = Tracer.Tracer(True, 'Foo', 10, 'Doing some detailed calculations');
#                    for i in range(10):
#                        tt.Print(True, 'Iteration {}'.format(i))
#
#
#
#            class Bar:
#
#                def BarFunction(self):
#
#                    # Tracer for an entering message - set group to "Bar" and level to 5
#                    Tracer.Tracer(True, 'Bar', 5, 'Entering BarFunction()');
#
#                    # ...more Bar stuff 
#
#

class Tracer:

    # define a bunch of Class (static) variables
    trace_group    = ''        # TRACEGROUP environment variable
    trace_level    = 0         # equal to TRACELEVEL environment variable
    only_flag      = False     # cooresponds to TRACEONLY environment variable
    env_checked    = False     # boolean.  Necessary for python??? 
    tracer_counter = 0         # used to generate unique serial numbers for each Tracer instances


    #
    # ctor
    #
    def __init__(self, print_flag, group, level, message):

        # group this Tracer belongs to
        self.group = group

        # trace level of this Tracer
        self.level = level

        # which number is this Tracer
        # a non-zero value for 'serial' not only uniquely identifies this tracer,
        # but also doubles as a flag to indicate that this Tracer SHOULD print
        self.serial = 0

        # simple integer to keep track of whether an exit message is printed
        # at destruction.  If usecount == 0, no message is printed
        # set to 0 in ctor, incremented at every call to Print().
        self.use_count = 0

        # only do this once, since the call to getenv() is relatively expenensive
        # is this flag check necessary in python???  Unsure.  Docs indicate that os.environ is 
        # populated once, when os is imported, so this variable check for performance may be
        # unnecessary
        if not Tracer.env_checked:
            Tracer.env_checked = True
            self._CheckEnvironment()

        if ('' != Tracer.trace_group):
            if ( ('ALL' == Tracer.trace_group) or (self.group in Tracer.trace_group) ):
                if ( ((Tracer.only_flag == True) and (self.level == Tracer.trace_level)) or ((Tracer.only_flag == False) and (self.level <= Tracer.trace_level)) ):

                    # is the print_flag true?
                    if print_flag:

                    # a non-zero value for 'serial' not only uniquely identifies this tracer,
                    # but also doubles as a flag to indicate that this Tracer SHOULD print
                    # the tracecount variable is a static class value
                    Tracer.tracer_counter += 1
                    self.serial = Tracer.tracer_counter

                        print('Tracer: [{}][{}, {}] {}'.format(self.serial, self.group, self.level, message), file=sys.stderr)


    #
    # dtor
    #
    def __del__(self):

        # print a closing message, if 
        #   - the serial number is non-zero, indicating this instance SHOULD print, and
        #   - the use_count > 0, indicating this instance has printed message more than once via the Print member function
        if ( (self.serial > 0) and (self.use_count > 0) ):
            print('Tracer: [{}][{}, {}] -exit-'.format(self.serial, self.group, self.level), file=sys.stderr)



    # 
    # check the TRACExx environment variables
    #
    def _CheckEnvironment(self):

        if 'TRACEGROUP' in os.environ:
            Tracer.trace_group = os.environ['TRACEGROUP']

        if 'TRACELEVEL' in os.environ:
            Tracer.trace_level = int(os.environ['TRACELEVEL'])

        if 'TRACEONLY' in os.environ:
            if ( (os.environ['TRACEONLY'] == 'True') or (os.environ['TRACEONLY'] == 'TRUE') or (os.environ['TRACEONLY'] == 'true') ):
                Tracer.only_flag = True

    #
    # print a message to stderr if the passed 'condition' variable
    # is non-zero, and if this Tracer object SHOULD be printing, based
    # on the TRACExx environment varialbes
    #
    def Print(self, print_flag, message):

        # print message if 'print_flag' is true, and if this Tracer SHOULD print
        if ( (print_flag) and (self.serial > 0) ):
            print('Tracer: [{}][{}, {}] {}'.format(self.serial, self.group, self.level, message), file=sys.stderr)
            self.use_count += 1



def main():

    import Tracer

    Tracer.Tracer(True, 'Main', 5, 'Entering main() function')


    tt = Tracer.Tracer(True, 'Main', 10, 'Doing some detailed calculations')
    for i in range(10):
        tt.Print(True, 'Iteration {}'.format(i))



if __name__ == '__main__':
    main()





