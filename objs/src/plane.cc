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

// $Id: plane.cc,v 1.1 2003/10/27 17:17:08 krmoule Exp $

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

//-------------------------------------------------------------------
// usage
//-------------------------------------------------------------------
void usage(void)
  {
  printf("Usage: plane [options]\n");
  printf("Options:\n");
  printf("    -w <num>    Number of width divisions (default: 10)\n");
  printf("    -h <num>    Number of height divisions (default: 10)\n");
  printf("    -n          No normals\n");
  printf("    -t          No texture coordinates\n");
  printf("    -g          No tangents\n");
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
// main
//-------------------------------------------------------------------
int main(int argc, char** argv)
  {
  int w = 10;
  int h = 10;
  bool normals = true;
  bool texcoords = true;
  bool tangents = true;

  int c;
  while((c = getopt(argc, argv, "w:h:ntg")) != -1)
    {
    switch(c)
      {
      case 'w':
	w = atoi(optarg);
	break;
      case 'h':
	h = atoi(optarg);
	break;
      case 'n':
	normals = false;
	break;
      case 't':
	texcoords = false;
	break;
      case 'g':
	tangents = false;
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

  for(int i = 0; i <= h; i++)
    {
    for(int j = 0; j <= w; j++) 
      {
      double x = ((double)j/w) - .5;
      double y = ((double)i/h) - .5;
      double u = ((double)j/w);
      double v = 1 - ((double)i/h);

      printf("v %f %f 0\n", x, y);

      if (texcoords)
	{
	printf("vt %f %f\n", u, v);
	}

      if (tangents)
	{
	printf("r 1 0 0\n");
	}

      if (normals)
	{
	printf("vn 0 0 1\n");
	}
      }
    }

  for(int i = 0; i < h; i++)
    {
    for(int j = 0; j < w; j++)
      {
      print_face(1+(i*(w+1))+j,
		 1+(i*(w+1))+j+1,
		 1+((i+1)*(w+1))+j+1,
		 normals,
		 texcoords);
      print_face(1+(i*(w+1))+j,
		 1+((i+1)*(w+1))+j+1,
		 1+((i+1)*(w+1))+j,
		 normals,
		 texcoords);
      }
    }
  }
