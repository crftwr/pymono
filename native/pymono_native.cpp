#include "python.h"

//-----------------------------------------------------------------------------

#define MODULE_NAME "pymono_native"
static PyObject * Error;

//-----------------------------------------------------------------------------

//#define PRINTF printf
//#define PRINTF PySys_WriteStdout
#define PRINTF(x...)

//#define TRACE printf("%s(%d) : %s\n",__FILE__,__LINE__,__FUNCTION__)
//#define TRACE PySys_WriteStdout("%s(%d) : %s\n",__FILE__,__LINE__,__FUNCTION__)
#define TRACE

#if 0
	struct FuncTrace
	{
		FuncTrace( const char * _funcname, unsigned int _lineno )
		{
			funcname = _funcname;
			lineno   = _lineno;
		
			printf( "FuncTrace : Enter : %s(%)\n", funcname, lineno );
		}

		~FuncTrace()
		{
			printf( "FuncTrace : Leave : %s(%)\n", funcname, lineno );
		}
	
		const char * funcname;
		unsigned int lineno;
	};
	#define FUNC_TRACE FuncTrace functrace(__FUNCTION__,__LINE__)
#else
	#define FUNC_TRACE
#endif

// ----------------------------------------------------------------------------

static PyObject * _initMono( PyObject * self, PyObject * args )
{
	if( ! PyArg_ParseTuple(args,"") )
		return NULL;
		
	printf("initMono\n");	

	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef pymono_native_funcs[] =
{
    { "initMono", _initMono, METH_VARARGS, "" },
    {NULL, NULL, 0, NULL}
};

// ----------------------------------------------------------------------------

extern "C" void __stdcall initpymono_native(void)
{
    PyObject *m, *d;

    m = Py_InitModule3( MODULE_NAME, pymono_native_funcs, "pymono_native module." );

    d = PyModule_GetDict(m);

    Error = PyErr_NewException( MODULE_NAME".Error", NULL, NULL);
    PyDict_SetItemString( d, "Error", Error );

    if( PyErr_Occurred() )
    {
        Py_FatalError( "can't initialize module "MODULE_NAME );
    }
}
