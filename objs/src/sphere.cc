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

// $Id: sphere.cc,v 1.1 2003/10/27 17:17:08 krmoule Exp $

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
  printf("Usage: sphere [options]\n");
  printf("Options:\n");
  printf("    -s <num>    Number of slices (default: 10)\n");
  printf("    -r <num>    Number of rings (default: 10)\n");
  printf("    -R <num>    Radius (default: 1)\n");
  printf("    -c          Generate cap (with singularities in tangents)\n");
  printf("    -C          Generate closed solid\n");
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
  double r = 1;
  int sides = 10;
  int rings = 10;
  bool cap = false;
  bool closed = false;
  bool normals = true;
  bool texcoords = true;
  bool tangents = true;

  int c;
  while((c = getopt(argc, argv, "s:r:R:cCntg")) != -1)
    {
    switch(c)
      {
      case 's':
	sides = atoi(optarg);
	break;
      case 'r':
	rings = atoi(optarg);
	break;
      case 'R':
	r = atof(optarg);
	break;
      case 'c':
	cap = true;
	break;
      case 'C':
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
  
  if (cap && closed)
    {
    printf("v 0 0 1\n");
    if (texcoords) printf("vt 0 0\n");
    if (tangents) printf("r 0 0 0\n");
    if (normals) printf("vn 0 0 1\n");
    printf("v 0 0 -1\n");
    if (texcoords) printf("vt 0 0\n");
    if (tangents) printf("r 0 0 0\n");
    if (normals) printf("vn 0 0 -1\n");
    }

  for(int i = 0; i <= rings; i++)
    {
    double theta = 2*M_PI*((double)i/rings);
    double cost = cos(theta);
    double sint = sin(theta);

    if (closed && i == rings) break;

    for(int j = 0; j <= sides; j++)
      {
      double phi = M_PI*((double)j/sides);

      if (cap)
	{
	if (closed)
	  {
	  if (j == 0) continue;
	  if (j == sides) continue;
	  }
	else
	  {
	  // this should really go back in, and
	  // the face construction code below changed
	  // to skip the appropriate numbers. I'm too
	  // lazy todo it right now. The only draw
	  // back is that extra unused vertices are
	  // being generated

	  //if (j == 0 && i%2 == 1) continue;
	  //if (j == sides && i%2 == 1) continue;
	  }
	}
      else
	{
	if (j == 0) phi += 0.01;
	if (j == sides) phi -= 0.01;
	}

      double cosp = cos(phi);
      double sinp = sin(phi);

      printf("v %f %f %f\n",
	     r*cost*sinp,
	     r*sint*sinp,
	     r*cosp);

      if (texcoords)
	{
	printf("vt %f %f\n",
	       ((double)i/rings),
	       ((double)j/sides));
	}

      if (tangents)
	{
	double dxdt = -sint*sinp;
	double dydt = cost*sinp;
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
	       cost*sinp,
	       sint*sinp,
	       cosp);
	}
      }
    }

  for(int i = 0; i < rings; i++)
    {
    if (cap)
      {
      if (closed)
	{
	print_face(1,
		   3+i*(sides-1)+0,
		   3+((i+1)%rings)*(sides-1)+0,
		   normals,
		   texcoords);

	for(int j = 0; j < sides-2; j++)
	  {
	  print_face(3+i*(sides-1)+j,
		     3+i*(sides-1)+(j+1),
		     3+((i+1)%rings)*(sides-1)+j,
		     normals,
		     texcoords);
	  print_face(3+i*(sides-1)+(j+1),
		     3+((i+1)%rings)*(sides-1)+(j+1),
		     3+((i+1)%rings)*(sides-1)+j,
		     normals,
		     texcoords);
	  }

	print_face(3+i*(sides-1)+sides-2,
		   2,
		   3+((i+1)%rings)*(sides-1)+sides-2,
		   normals,
		   texcoords);
	}
      else
	{
	print_face(1+i*(sides+1),
		   1+i*(sides+1)+1,
		   1+(i+1)*(sides+1)+1,
		   normals,
		   texcoords);

	for(int j = 1; j < sides-1; j++)
	  {
	  print_face(1+i*(sides+1)+j,
		     1+i*(sides+1)+(j+1),
		     1+(i+1)*(sides+1)+j,
		     normals,
		     texcoords);
	  print_face(1+i*(sides+1)+(j+1),
		     1+(i+1)*(sides+1)+(j+1),
		     1+(i+1)*(sides+1)+j,
		     normals,
		     texcoords);
	  }

	print_face(1+i*(sides+1)+sides-1,
		   1+i*(sides+1)+sides,
		   1+(i+1)*(sides+1)+sides-1,
		   normals,
		   texcoords);
	}
      }
    else
      {
      if (closed)
	{
	for(int j = 0; j < sides; j++)
	  {
	  print_face(1+i*(sides+1)+j,
		     1+i*(sides+1)+(j+1),
		     1+((i+1)%rings)*(sides+1)+j,
		     normals,
		     texcoords);
	  print_face(1+i*(sides+1)+(j+1),
		     1+((i+1)%rings)*(sides+1)+(j+1),
		     1+((i+1)%rings)*(sides+1)+j,
		     normals,
		     texcoords);
	  }
	}
      else
	{
	for(int j = 0; j < sides; j++)
	  {
	  print_face(1+i*(sides+1)+j,
		     1+i*(sides+1)+(j+1),
		     1+(i+1)*(sides+1)+j,
		     normals,
		     texcoords);
	  print_face(1+i*(sides+1)+(j+1),
		     1+(i+1)*(sides+1)+(j+1),
		     1+(i+1)*(sides+1)+j,
		     normals,
		     texcoords);
	  }
	}
      }
    }
  }

