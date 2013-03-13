CXXFLAGS	= -fpermissive -g $(DEBUG_FLAG)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS = -framework OpenGL -framework GLUT
else
	LDFLAGS = -lGL -lGLU -lglut
endif

SRCS		= \
	openGl3.cpp

OBJS		= $(SRCS:.cpp=.o)
EXEC		= openGl3

default : $(EXEC)

$(EXEC) : $(OBJS)
	g++ -o $(EXEC) $(OBJS) $(LDFLAGS)

zip	: openGl3.zip

openGl3.zip :
	zip -r openGl3.zip ./ -x *.git*

clean	:
	rm -f *.o $(EXEC)

depend	: $(SRCS)
	makedepend -- $(CXXFLAGS) -- $(SRCS)
