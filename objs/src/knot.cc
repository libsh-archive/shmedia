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

// $Id: knot.cc,v 1.1 2003/10/27 17:17:08 krmoule Exp $

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
  printf("Usage: knot [options]\n");
  printf("Options:\n");
  printf("    -u <num>    Sampling in u direction (default: 10)\n");
  printf("    -v <num>    Sampling in v direction (default: 10)\n");
  printf("    -a <num>    A coefficient (default: 1.0)\n");
  printf("    -b <num>    B coefficient (default: 0.5)\n");
  printf("    -c <num>    C coefficient (default: 0.5)\n");
  printf("    -d <num>    D coefficient (default: 0.2)\n");
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
  int u = 10;
  int v = 10;
  double a = 1; 
  double b = 0.50;
  double c = 0.50;
  double d = 0.20;
  bool closed = false;
  bool normals = true;
  bool texcoords = true;
  bool tangents = true;

  int ch;
  while((ch = getopt(argc, argv, "u:v:a:b:c:d:Cntg")) != -1)
    {
    switch(ch)
      {
      case 'u':
	u = atoi(optarg);
	break;
      case 'v':
	v = atoi(optarg);
	break;
      case 'a':
	a = atof(optarg);
	break;
      case 'b':
	b = atof(optarg);
	break;
      case 'c':
	c = atof(optarg);
	break;
      case 'd':
	d = atof(optarg);
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

  for(int i = 0; i <= u; i++)
    {
    if (closed && i == u) break;

    double s = 4*M_PI*((double)i/u);

    double x = cos(s)*(a + b*cos(1.5*s));
    double y = sin(s)*(a + b*cos(1.5*s));
    double z = c*sin(1.5*s);      

    double dxds = -sin(s)*(a + b*cos(1.5*s)) - 1.5*b*sin(1.5*s)*cos(s);
    double dyds = cos(s)*(a + b*cos(1.5*s)) - 1.5*b*sin(1.5*s)*sin(s);
    double dzds = 1.5*c*cos(1.5*s);
    double dlen = sqrt(dxds*dxds + dyds*dyds + dzds*dzds);
    
    double qn[3];
    qn[0] = dxds/dlen;
    qn[1] = dyds/dlen;
    qn[2] = dzds/dlen;
    
    double px = -qn[1];
    double py =  qn[0];
    double pz =  0.0;
    double plen = sqrt(px*px + py*py + pz*pz);
    
    double qvn[3];
    qvn[0] = px/plen;
    qvn[1] = py/plen;
    qvn[2] = pz/plen;
    
    double ww[3];
    ww[0] = qn[1]*qvn[2] - qvn[1]*qn[2];
    ww[1] = qn[2]*qvn[0] - qvn[2]*qn[0];
    ww[2] = qn[0]*qvn[1] - qvn[0]*qn[1];
      
    for(int j = 0; j <= v; j++)
      {
      if (closed && j == v) break;
      
      double t = 2*M_PI*((double)j/v);

      printf("v %f %f %f\n", 
	     x + d*(qvn[0]*cos(t)+ww[0]*sin(t)),
	     y + d*(qvn[1]*cos(t)+ww[1]*sin(t)),
	     z + d*ww[2]*sin(t));

      px = d*(qvn[0]*cos(t)+ww[0]*sin(t));
      py = d*(qvn[1]*cos(t)+ww[1]*sin(t));
      pz = d*ww[2]*sin(t);
      plen = sqrt(px*px + py*py + pz*pz);
      
      if (texcoords)
	{
	printf("vt %f %f\n",
	       ((double)i/u),
	       ((double)j/v));
	}

      if (tangents)
	{
	printf("r %f %f %f\n",
	       qn[0],
	       qn[1],
	       qn[2]);
	}

      if (normals)
	{
	printf("vn %f %f %f\n",
	       px/plen,
	       py/plen,
	       pz/plen);
	}
      }
    }

  for(int i = 0; i < u; i++)
    {
    for(int j = 0; j < v; j++)
      {
      if (closed)
	{
	print_face(1 + i*(v)+j,
		   1 + ((i+1)%u)*(v)+((j+1)%v),
		   1 + ((i+1)%u)*(v)+j,
		   normals,
		   texcoords);
	
	print_face(1 + i*(v)+j,
		   1 + i*(v)+((j+1)%v),
		   1 + ((i+1)%u)*(v)+((j+1)%v),
		   normals,
		   texcoords);
	}
      else
	{
	print_face(1 + i*(v+1)+j,
		   1 + (i+1)*(v+1)+(j+1),
		   1 + (i+1)*(v+1)+j,
		   normals,
		   texcoords);
	
	print_face(1 + i*(v+1)+j,
		   1 + i*(v+1)+(j+1),
		   1 + (i+1)*(v+1)+(j+1),
		   normals,
		   texcoords);
	}
      }
    }
  }
