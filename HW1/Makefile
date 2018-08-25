CC		= g++
C		= cpp
H       = h

CFLAGS		= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/ -lglut -lGL -lGLU -lm
  endif
endif


HFILES = ToolKit.${H}
OFILES = ToolKit.o
PROJECT		= hw1

${PROJECT}:	${PROJECT}.o ${OFILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${PROJECT}.o ${OFILES} ${LDFLAGS}

${PROJECT}.o: ${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

ToolKit.o : ToolKit.${C} ToolKit.${H}
	${CC} ${CFLAGS} -c ToolKit.${C}

clean:
	rm -f core.* *.o *~ ${PROJECT}

