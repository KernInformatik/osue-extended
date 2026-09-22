# I think we got it

I had severe issues on how to process if the given solution is actually a solution.

The core idea is this:

- generator.c parses the input and writes it into our buffer
- it colorizes the vertices of our graph
- it parses through the graph with a solutionCounter incremented with illegal solutions: solution.vertex.u.color == solution.vertex.v.color
  - if the length of the solution is 0 the graph is colorable

But what was hard to implement?
To be honest, the structure on how to save the data. It really isn't hard to implement the graph basic structure, like vertexes or the edges. Even the thought to implent the GRAPH_EDGE_LIST makes also sense.

However, the difficult part was the shared memory, and what it should save.

After reading other's solutions (see github repo) I came to the conclusion that the shared memory should save edgelists with max buff size of 32 or even 64 I think it doen't have that big impact.

After copy pasting and changing the shm data of libsem, 100% handwritten, see on github, I think I got the general structure of it.

Genuinely, this was by far the hardest exercise, it took me 2 weeks, I even implemented a library for it and read like schizo the man pages, which actually helped a lot.
