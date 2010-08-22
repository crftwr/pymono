#include <stdlib.h>

#include <python.h>

#include <mono/jit/jit.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/mono-config.h>
#include <mono/utils/mono-embed.h>


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

MonoDomain * domain;
const char * file = "test.exe";

static void main_function (MonoDomain *domain, const char *file, int argc, char** argv)
{
	MonoAssembly *assembly;

	assembly = mono_domain_assembly_open (domain, file);
	if (!assembly)
		exit (2);

	/*
	 * mono_jit_exec() will run the Main() method in the assembly.
	 * The return value needs to be looked up from
	 * System.Environment.ExitCode.
	 */
	mono_jit_exec (domain, assembly, argc, argv);
}

static PyObject * _initialize( PyObject * self, PyObject * args )
{
	if( ! PyArg_ParseTuple(args,"") )
		return NULL;
		
	{
		/*
		 * Load the default Mono configuration file, this is needed
		 * if you are planning on using the dllmaps defined on the
		 * system configuration
		 */
		mono_config_parse (NULL);
		/*
		 * mono_jit_init() creates a domain: each assembly is
		 * loaded and run in a MonoDomain.
		 */
		domain = mono_jit_init (file);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * _terminate( PyObject * self, PyObject * args )
{
	if( ! PyArg_ParseTuple(args,"") )
		return NULL;
		
	{
		mono_jit_cleanup(domain);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * _execute( PyObject * self, PyObject * args )
{
	if( ! PyArg_ParseTuple(args,"") )
		return NULL;
		
	int argc = 1;
	const char * argv[] = { file };
	main_function ( domain, file, argc, const_cast<char**>(argv) );
	int result = mono_environment_exitcode_get();

	PyObject * pyret = Py_BuildValue( "i", result );
	return pyret;
}

static PyObject * _pyStringToMonoString( PyObject * self, PyObject * args )
{
	const char * str;
	unsigned int len;

	if( ! PyArg_ParseTuple( args, "s", &str, &len ) )
		return NULL;
		
	MonoString * mono_string = mono_string_new_len(mono_domain_get(),str,len);	
		
	PyObject * pyret = Py_BuildValue( "i", mono_string );
	return pyret;
}

static PyObject * _monoStringToPyString( PyObject * self, PyObject * args )
{
	int mono_string;

	if( ! PyArg_ParseTuple( args, "i", &mono_string ) )
		return NULL;
		
	char * utf8 = mono_string_to_utf8( (MonoString*)mono_string );
		
	PyObject * pyret = Py_BuildValue( "s", utf8 );

	g_free(utf8);

	return pyret;
}

static PyMethodDef pymono_native_funcs[] =
{
    { "initialize", _initialize, METH_VARARGS, "" },
    { "terminate", _terminate, METH_VARARGS, "" },
    { "execute", _execute, METH_VARARGS, "" },

    { "pyStringToMonoString", _pyStringToMonoString, METH_VARARGS, "" },
    { "monoStringToPyString", _monoStringToPyString, METH_VARARGS, "" },

    {NULL, NULL, 0, NULL}
};

// ----------------------------------------------------------------------------

extern "C" int __stdcall registerInternalCall( const char * name, const void * func )
{
	mono_add_internal_call( name, (const void*)func );
	return 0;
}

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
