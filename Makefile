CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)

	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X12/include -DOSX -Wno-deprecated-declarations -Wall -Werror -std=c++11
	LDFLAGS = -framework GLUT -framework OpenGL \
    		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    		-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin -Wno-deprecated-declarations -Wall -Werror -std=c++11
	LDFLAGS = -lglut -lGLU
endif

AFLAGS = -I ./armadillo-5.000.1/include -DARMA_DONT_USE_WRAPPER -framework Accelerate
	
RM = /bin/rm -f 
all: main 
main: clean main.cpp
	$(CC) $(CFLAGS) $(AFLAGS) -O0  main.cpp -o main $(LDFLAGS)
clean: 
	$(RM) *.o main
