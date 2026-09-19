#include "semaphore.h"
#include <cstddef>

enum GRAPH_VERTEX_COLOR
{
  RED = 0,
  BLUE =1,
  GREEN = 2
};

struct GRAPH_VERTEX
{
  uint name;
  enum GRAPH_VERTEX_COLOR color;
};

struct GRAPH_EDGE
{
  struct GRAPH_VERTEX from;
  struct GRAPH_VERTEX to;
};

#define EDGES 1028
struct GRAPH_EDGE_LIST
{
  GRAPH_EDGE edge_list[EDGES];
  size_t length;
};



