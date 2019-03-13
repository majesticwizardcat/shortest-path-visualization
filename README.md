# Shortest path algorithms visualization

Real time visualization of shortest path algorithms implemented
in C++ using the SFML library.

## Compiling and running

To compile, first install the SFML library:
```
	sudo apt install libsfml-dev
```
After that use `make` to compile the source.
You need to have the GNU `g++` compiler and `make`.

To run:
```
	./rendergraph [nodesFile] [edgesFile] [windowWidth] [windowHeight] [nodePixelSize] [startNode] [endNode]
	where [nodesFile]: a file with the graph nodes
	      [edgesFile]: a file with the graph edges
	      [windowWidth]: the window width in pixels
	      [windowHeight]: the window height in pixels
	      [nodePixelSize]: a float for the radius of the node
	      [startNode]: the id of the node to start
              [endNode]: the id of the node to end
```

The nodes file has the format:
```
	v [id] [x] [y]
	where [id]: the node id
	      [x]: the x coordinate
	      [y]: the y coordinate
```

The edges file has the format:
```
	a [idFrom] [idTo] [weight]
	where [idFrom]: the node that the edge starts from
	      [idTo]: the node the edge goes to
	      [weight]: the weight between these nodes
```

