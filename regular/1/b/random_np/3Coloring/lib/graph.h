#ifndef GRAPH_H
#define GRAPH_H
#include "common.h"

enum GRAPH_VERTEX_COLOR { RED = 0, BLUE = 1, GREEN = 2 };

struct GRAPH_VERTEX
{
  int name;
  enum GRAPH_VERTEX_COLOR color;
};

struct GRAPH_EDGE
{
  struct GRAPH_VERTEX from;
  struct GRAPH_VERTEX to;
};

struct GRAPH_EDGE_LIST
{
  struct GRAPH_EDGE edgeList[1028];
  size_t length;
};
#endif
