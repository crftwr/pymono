all: cs_exe

cs_exe: test.exe

test.exe: test.cs
	gmcs $<

clean:
	rm -f test.exe

run:
	/c/python27/python.exe -u pymono_main.py
