using System;
using System.Runtime.CompilerServices;

class PyMonoTest
{
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	extern static int Func1( int i );

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	extern static float Func2( float f );

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	extern static float Func3( int i, float f );

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	extern static string Func4( string s );

	static void Main()
	{
		Console.WriteLine( Func1(1234) );
		Console.WriteLine( Func2(12.34f) );
		Console.WriteLine( Func3(3,1.234f) );
		Console.WriteLine( Func4("Hello!") );
	}
}
