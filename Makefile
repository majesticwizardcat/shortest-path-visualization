NAME = rendergraph
SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system
SOURCE = source/*.cpp

default: compile

clean: 
	rm -rf ${NAME}

compile: clean
	g++ ${SOURCE} ${SFMLLIBS} -o ${NAME}

