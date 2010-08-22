
.PHONY: dist

all: cs_exe dist

cs_exe: test.exe

test.exe: test.cs
	gmcs $<

dist:
	cp pymono_icall.py    dist/pymono/
	cp pymono_native.pyd  dist/pymono/
	cp test.py	      dist/pymono/sample/
	cp test.cs	      dist/pymono/sample/

clean:
	rm -f test.exe

run: all
	/c/python27/python.exe -u test.py
