import os
import sys
import subprocess

sys.path.append("./dist")

os.environ["PATH"] += r";c:\Program Files (x86)\Mono-2.6.7\bin"
os.environ["MONO_PATH"] = r"c:\Program Files (x86)\Mono-2.6.7\lib\mono\2.0"

import pymono

subprocess.call( [ "gmcs.bat", "test.cs" ] )

pymono.initialize( "test.exe" )

#----------------------------------------------------------

def Func1(i):
    return i*2

pymono.register( "PyMonoTest::Func1", "i", "i", Func1 )


def Func2(f):
    return f*2

pymono.register( "PyMonoTest::Func2", "f", "f", Func2 )


def Func3(i,f):
    return i*f

pymono.register( "PyMonoTest::Func3", "f", "if", Func3 )


def Func4(s):
    s = pymono.PyMonoString(s)
    return pymono.PyMonoString(">>>" + s.toPyString() + "<<<").toMonoString()

pymono.register( "PyMonoTest::Func4", "s", "s", Func4 )

#----------------------------------------------------------

print ""
print "--"

pymono.execute()

print "--"

pymono.terminate()
