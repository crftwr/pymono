
DIST_FILES = \
	pymono/__init__.py \
	pymono/pymono_icall.py \
	pymono/pymono_native.pyd \
	pymono/src.zip \
	pymono/sample/test.py \
	pymono/sample/test.cs \

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
	cd dist/src && svn export --force ../../../pymono && zip -r ../pymono/src.zip pymono
	cd dist && zip pymono.zip $(DIST_FILES)
	@echo ""
	md5sum dist/pymono.zip


clean:
	rm -f test.exe

run: all
	/c/python27/python.exe -u test.py
