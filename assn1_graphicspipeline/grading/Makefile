CXX = g++
CXXFLAGS = -c -Wall -g -O3 -std=c++11
LIBS_NOGL  = -lpng
LIBS  = -lGL -lGLU -lglut -lX11 $(LIBS_NOGL)
OBJ := dump_png.o minigl.o
OBJ_GL := main.o parse.o
OBJ_NOGL := $(subst .o,-nogl.o,$(OBJ_GL))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	MAC_INCLUDE= -I/usr/local/Cellar/libpng/1.6.34/include/libpng16/ #-I<incpath1> -I<incpath2>
	MAC_LIBDIR= -L/usr/local/Cellar/libpng/1.6.34/lib/ #-L<libpath1> -L<libpath2>
	LIBS = -framework OpenGL -framework GLUT $(LIBS_NOGL)
	CXXFLAGS += -Wno-deprecated-declarations -Wno-macro-redefined $(MAC_INCLUDE)
endif

TARGET = minigl
TARGET_NOGL = $(TARGET)-nogl

.PHONY: all clean

all: $(TARGET) $(TARGET_NOGL)

$(TARGET): $(OBJ) $(OBJ_GL)
	$(CXX) -o $(TARGET) $(OBJ) $(OBJ_GL) $(MAC_LIBDIR) $(LIBS)
$(TARGET_NOGL): $(OBJ) $(OBJ_NOGL)
	$(CXX) -o $(TARGET_NOGL) $(OBJ) $(OBJ_NOGL) $(MAC_LIBDIR) $(LIBS_NOGL)

# Generate Objects
$(OBJ) $(OBJ_GL): %.o:%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
$(OBJ_NOGL): %-nogl.o: %.cpp
	$(CXX) $(CXXFLAGS) -DNO_OPENGL -o $@ $<

clean:
	rm -f *.o *~ *.bak $(TARGET) $(TARGET_NOGL)
