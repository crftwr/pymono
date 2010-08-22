import ctypes
import pymono_native

_pymono_native = ctypes.cdll.LoadLibrary("pymono_native.pyd")

cfunc_table = {}

def register( name, res_type, arg_types, func ):

    ctypes_func_type = []

    if res_type=="i":
        ctypes_func_type.append( ctypes.c_int )
    elif res_type=="f":
        ctypes_func_type.append( ctypes.c_float )
    elif res_type=="s":
        ctypes_func_type.append( ctypes.c_void_p )
    else:
        raise ValueError("unknown format charactor :",res_type)
    
    for arg_type in arg_types:
        if arg_type=="i":
            ctypes_func_type.append( ctypes.c_int )
        elif arg_type=="f":
            ctypes_func_type.append( ctypes.c_float )
        elif arg_type=="s":
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
            raise TypeError( "PyMonoString cannot be created from" + type(src) )
        
    def toPyString(self):
        return self.s

    def toMonoString(self):
        return pymono_native.pyStringToMonoString(self.s)

# TODO:
# 
# - str じゃなくて unicode を使って、変換の回数を減らす
# - もっといろんなタイプの引数をサポート ( 構造体、配列、signed/unsigned )
#
