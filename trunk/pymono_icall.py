import ctypes
import pymono_native

_pymono_native = ctypes.cdll.LoadLibrary("pymono_native.pyd")

cfunc_table = {}

def register( name, res_type, arg_type, func ):

    ctypes_func_type = []

    if res_type=="i":
        ctypes_func_type.append( ctypes.c_int )
    elif res_type=="f":
        ctypes_func_type.append( ctypes.c_float )
    elif res_type=="s":
        ctypes_func_type.append( ctypes.c_void_p )
    else:
        raise ValueError("unknown format charactor :",res_type)
    
    for a in arg_type:
        if a=="i":
            ctypes_func_type.append( ctypes.c_int )
        elif a=="f":
            ctypes_func_type.append( ctypes.c_float )
        elif a=="s":
            ctypes_func_type.append( ctypes.c_void_p )
        else:
            raise ValueError("unknown format charactor :",a)
        
    CFuncType = ctypes.CFUNCTYPE( *ctypes_func_type )
    cfunc = CFuncType(func)

    _pymono_native.registerInternalCall( name, cfunc )    

    cfunc_table[name] = cfunc
    

class PyMonoString:

    def __init__(self,src):
        if isinstance( src, ctypes.c_void_p ) or isinstance( src, int ):
            self.s = pymono_native.monoStringToPyString(src)
        elif isinstance( src, str ):
            self.s = src
        else:
            print type(src)
            assert(False)    
        
    def toPyString(self):
        return self.s

    def toMonoString(self):
        return pymono_native.pyStringToMonoString(self.s)
    
