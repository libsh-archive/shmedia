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

// $Id: torus.cc,v 1.1 2003/10/27 17:17:08 krmoule Exp $

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
  printf("Usage: torus [options]\n");
  printf("Options:\n");
  printf("    -s <num>    Number of slices (default: 10)\n");
  printf("    -r <num>    Number of rings (default: 10)\n");
  printf("    -i <num>    Inner radius (default .25)\n");
  printf("    -o <num>    Outer radius (default .75)\n");
  printf("    -c          Generate closed solid\n");
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
  double rin = .25;
  double rout = .75;
  int sides = 10;
  int rings = 10;
  bool closed = false;
  bool normals = true;
  bool texcoords = true;
  bool tangents = true;

  int c;
  while((c = getopt(argc, argv, "s:r:i:o:cntg")) != -1)
    {
    switch(c)
      {
      case 's':
	sides = atoi(optarg);
	break;
      case 'r':
	rings = atoi(optarg);
	break;
      case 'i':
	rin = atof(optarg);
	break;
      case 'o':
	rout = atof(optarg);
	break;
      case 'c':
	closed = true;
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
  
  for(int i = 0; i <= rings; i++)
    {
    if (closed && i == rings) continue;

    double theta = 2*M_PI*((double)i/rings);
    double cost = cos(theta);
    double sint = sin(theta);

    for(int j = 0; j <= sides; j++)
      {
      if (closed && j == sides) continue;

      double phi = 2*M_PI*((double)j/sides);
      double cosp = cos(phi);
      double sinp = sin(phi);

      printf("v %f %f %f\n",
	     cost*(rout + rin*cosp),
	     sint*(rout + rin*cosp),
	     rin*sinp);

      if (texcoords)
	{
	printf("vt %f %f\n",
	       ((double)i/rings),
	       ((double)j/sides));
	}

      if (tangents)
	{
	double dxdt = -sint*(rout + rin*cosp);
	double dydt = cost*(rout + rin*cosp);
	double dzdt = 0;
	double d = 1.0/sqrt(dxdt*dxdt + dydt*dydt + dzdt*dzdt);
	
	printf("r %f %f %f\n",
	       d*dxdt,
	       d*dydt,
	       d*dzdt);
	}

      if (normals)
	{
	printf("vn %f %f %f\n",
	       cost*cosp,
	       sint*cosp,
	       sinp);
	}
      }
    }
  
  for(int i = 0; i < rings; i++)
    {
    for(int j = 0; j < sides; j++)
      {
      if (closed)
	{
	print_face(1+i*(sides)+j,
		   1+((i+1)%rings)*(sides)+j,
		   1+i*(sides)+(j+1)%sides,
		   normals,
		   texcoords);
	print_face(1+i*(sides)+(j+1)%sides,
		   1+((i+1)%rings)*(sides)+j,
		   1+((i+1)%rings)*(sides)+(j+1)%sides,
		   normals,
		   texcoords);
	}
      else
	{
	print_face(1+i*(sides+1)+j,
		   1+((i+1))*(sides+1)+j,
		   1+i*(sides+1)+(j+1),
		   normals,
		   texcoords);
	print_face(1+i*(sides+1)+(j+1),
		   1+((i+1))*(sides+1)+j,
		   1+((i+1))*(sides+1)+(j+1),
		   normals,
		   texcoords);
	}
      }
    }
  }

