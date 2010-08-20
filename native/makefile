TARGET = pymono_native.pyd

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)
MDS  = $(SRCS:%.cpp=%.d)

ifeq ($(BUILD_MODE),debug)
BUILD_MODE_CFLAGS = \
    -g -DDEBUG
else
BUILD_MODE_CFLAGS = \
    -O2 -DNDEBUG
endif

CFLAGS = \
    $(BUILD_MODE_CFLAGS) \
    -mno-cygwin -mwindows \
    -MD \
    -fno-common \
    -fno-exceptions \
    -fno-rtti \
    -I/c/python27/include \
    -DWINVER=0x0500 \
    -D_WIN32_WINNT=0x0500 \
    -D_UNICODE=1 \
    -DUNICODE=1 \

LIBS = \
    -L/usr/lib/mingw \
    -L/c/python27/libs \
    -lpython27 \
    -lcomdlg32 \
    -lole32 \
    -lnetapi32 \
    -lmpr \
    -luuid \

#-------------------------------------------------------------------

all: $(TARGET)

clean:
	rm -f $(OBJS) $(MDS) $(TARGET) *~

install: $(TARGET)
	@cp -upv pymono_native.pyd ..

#-------------------------------------------------------------------

.SUFFIXES: .exe .pyd .o .cpp

.cpp.o:
	g++-3 $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	dllwrap --target=i386-mingw32 -mno-cygwin -k -def pymono_native.def --driver-name g++-3 -o $@ $(OBJS) $(LIBS)
	strip $@

#-------------------------------------------------------------------

-include $(MDS)
