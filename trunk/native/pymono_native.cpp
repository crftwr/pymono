#include <stdlib.h>

#include <python.h>

#include <mono/jit/jit.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/mono-config.h>


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

static MonoString*
gimme () {
	return mono_string_new (mono_domain_get (), "Hello InternalCall!");
}

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

static PyObject * _initMono( PyObject * self, PyObject * args )
{
	if( ! PyArg_ParseTuple(args,"") )
		return NULL;
		
	{
		MonoDomain *domain;
		const char *file;
		int retval;
	
		file = "test.exe";

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
		/*
		 * We add our special internal call, so that C# code
		 * can call us back.
		 */
		mono_add_internal_call ("MonoEmbed::gimme", (const void*)gimme);

		int argc = 1;
		const char * argv[] = { file };
		main_function ( domain, file, argc, const_cast<char**>(argv) );
	
		retval = mono_environment_exitcode_get ();
	
		mono_jit_cleanup(domain);
	}



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
