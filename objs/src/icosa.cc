// -*- C++ -*-
// Copyright (C) 2003, Kevin Moule (krmoule@cgl.uwaterloo.ca)
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.

// $Id: icosa.cc,v 1.1 2003/10/27 17:17:08 krmoule Exp $

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

#include <map>
#include <vector>

//-------------------------------------------------------------------
// defines
//-------------------------------------------------------------------
#define PHI .61803398874989484820

//-------------------------------------------------------------------
// vertex_t
//-------------------------------------------------------------------
class vertex_t
  {
  public:
    vertex_t(int id, double x, double y, double z) :
      id(id)
      {
      double len = sqrt(x*x + y*y + z*z);
      p[0] = x/len;
      p[1] = y/len;
      p[2] = z/len;
      }

  public:
    int id;
    double p[3];

  public:
    std::map<int, int> splits;
  };

//-------------------------------------------------------------------
// triangle_t
//-------------------------------------------------------------------
class triangle_t
  {
  public:
    triangle_t(int a, int b, int c)
      {
      f[0] = a;
      f[1] = b;
      f[2] = c;
      }

  public:
    int f[3];
  };

//-------------------------------------------------------------------
// icosahedron points
//-------------------------------------------------------------------
double pts[][3] = {
    {-PHI, -1, 0 }, // 0
    { PHI, -1, 0 }, // 1
    {-PHI,  1, 0 }, // 2
    { PHI,  1, 0 }, // 3

    { 0, -PHI, -1 }, // 4
    { 0,  PHI, -1 }, // 5
    { 0, -PHI,  1 }, // 6
    { 0,  PHI,  1 }, // 7

    {-1, 0, -PHI }, // 8
    {-1, 0,  PHI }, // 9
    { 1, 0, -PHI }, // 10
    { 1, 0,  PHI }, // 11
  };

//-------------------------------------------------------------------
// icosahedron faces
//-------------------------------------------------------------------
int faces[][3] = {
    { 0, 1, 6 },
    { 0, 4, 1 },

    { 2, 7, 3 },
    { 2, 3, 5 },

    { 4, 8, 5 },
    { 4, 5, 10 },

    { 6, 7, 9 },
    { 6, 11, 7 },

    { 8, 0, 9 },
    { 8, 9, 2 },

    { 10, 11, 1 },
    { 10, 3, 11 },

    { 1, 11, 6 },
    { 1, 4, 10 },

    { 0, 6, 9 },
    { 0, 8, 4 },

    { 3, 7, 11 },
    { 3, 10, 5 },

    { 2, 9, 7 },
    { 2, 5, 8 },
  };

//-------------------------------------------------------------------
// usage
//-------------------------------------------------------------------
void usage(void)
  {
  printf("Usage: icosa [options]\n");
  printf("Options:\n");
  printf("    -d <num>    Recursion depth (default: 2)\n");
  printf("    -r <num>    Radius (default: 1)\n");
  printf("    -n          No normals\n");
  exit(0);
  }

//-------------------------------------------------------------------
// print_face
//-------------------------------------------------------------------
void print_face(int a, int b, int c, bool normals, bool texcoords)
  {
  if (normals)
    {
    if (texcoords)
      {
      printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
	     a, a, a, b, b, b, c, c, c);
      }
    else
      {
      printf("f %d//%d %d//%d %d//%d\n",
	     a, a, b, b, c, c);
      }
    }
  else if (texcoords)
    {
    printf("f %d/%d %d/%d %d/%d\n",
	     a, a, b, b, c, c);
    }
  else
    {
    printf("f %d %d %d\n",
	   a, b, c);
    }
  }

//-------------------------------------------------------------------
// split
//-------------------------------------------------------------------
int split(std::vector<vertex_t>& vtxs, int a, int b)
  {
  if (vtxs[a].splits.find(b) != vtxs[a].splits.end())
    {
    return vtxs[a].splits[b];
    }
  else
    {
    vertex_t c(vtxs.size(),
	       vtxs[a].p[0]+vtxs[b].p[0],
	       vtxs[a].p[1]+vtxs[b].p[1],
	       vtxs[a].p[2]+vtxs[b].p[2]);
    vtxs.push_back(c);

    vtxs[a].splits[b] = c.id;
    vtxs[b].splits[a] = c.id;

    return c.id;
    }
  }

//-------------------------------------------------------------------
// subdivide
//-------------------------------------------------------------------
void subdivide(int depth,
  	       const triangle_t& t,
  	       std::vector<vertex_t>& vtxs,
  	       std::vector<triangle_t>& tris)
  {
  if (depth == 0)
    {
    tris.push_back(t);
    }
  else
    {
    int a = split(vtxs, t.f[0], t.f[1]);
    int b = split(vtxs, t.f[1], t.f[2]);
    int c = split(vtxs, t.f[2], t.f[0]);

    triangle_t t0(t.f[0], a, c);
    triangle_t t1(a, t.f[1], b);
    triangle_t t2(c, b, t.f[2]);
    triangle_t t3(a, b, c);

    subdivide(depth-1, t0, vtxs, tris);
    subdivide(depth-1, t1, vtxs, tris);
    subdivide(depth-1, t2, vtxs, tris);
    subdivide(depth-1, t3, vtxs, tris);
    }
  }

//-------------------------------------------------------------------
// main
//-------------------------------------------------------------------
int main(int argc, char** argv)
  {
  double r = 1;
  int depth = 2;
  bool normals = true;

  int c;
  while((c = getopt(argc, argv, "d:r:n")) != -1)
    {
    switch(c)
      {
      case 'd':
	depth = atoi(optarg);
	break;
      case 'r':
	r = atof(optarg);
	break;
      case 'n':
	normals = false;
	break;
      default:
	usage();
	break;
      }
    }

  // header
  time_t tm = time(NULL);
  printf("# Generated: %s", ctime(&tm));
  printf("# Command: ");
  for(int i = 0; i < argc; i++) printf("%s ", argv[i]);
  printf("\n");

  // vertex and triangle arrays
  std::vector<vertex_t> vtxs;
  std::vector<triangle_t> tris;

  // allocate a vertex for all the points
  for(unsigned int i = 0; i < sizeof(pts)/sizeof(pts[0]); i++)
    {
    vertex_t v(vtxs.size(), pts[i][0], pts[i][1], pts[i][2]);
    vtxs.push_back(v);
    }

  // allocate a triangle for all the faces, and subdivide
  for(unsigned int i = 0; i < sizeof(faces)/sizeof(faces[0]); i++)
    {
    triangle_t t(faces[i][0], faces[i][1], faces[i][2]);
    subdivide(depth, t, vtxs, tris);
    }
 
  // spit out the vertex data
  for(unsigned int i = 0; i < vtxs.size(); i++)
    {
    printf("v %f %f %f\n",
	   r*vtxs[i].p[0],
	   r*vtxs[i].p[1],
	   r*vtxs[i].p[2]);

    if (normals)
      {
      printf("vn %f %f %f\n",
	     vtxs[i].p[0],
	     vtxs[i].p[1],
	     vtxs[i].p[2]);
      }
    }

  // spit out the triangle data
  for(unsigned int i = 0; i < tris.size(); i++)
    {
    print_face(1 + tris[i].f[0],
	       1 + tris[i].f[1],
	       1 + tris[i].f[2],
	       normals, false);
    }

  return 0;
  }
