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

// $Id: teapot.cc,v 1.2 2004/02/19 20:11:17 sjdutoit Exp $

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

//-------------------------------------------------------------------
// defines
//-------------------------------------------------------------------
#define MIRROR_NONE 0
#define MIRROR_X    1
#define MIRROR_Y    2
#define MIRROR_XY   3

//-------------------------------------------------------------------
// typedefs
//-------------------------------------------------------------------
typedef int patch_t[4][4];
typedef double point_t[3];

//-------------------------------------------------------------------
// patch_set_t
//-------------------------------------------------------------------
struct patch_set_t
  {
    int mirror;
    patch_t* p;
    bool u_flip;
    bool v_flip;
    double u_scale;
    double v_scale;
    double u_bias;
    double v_bias;
  };

//-------------------------------------------------------------------
// teapot control points
//-------------------------------------------------------------------
point_t cpts[] = {
  { 0.2000, 2.70000, 0.0000 },
  { 0.2000, 2.70000, -0.1120 },
  { 0.1120, 2.70000, -0.2000 },
  { 0.0000, 2.70000, -0.2000 },
  { 1.3375, 2.53125, 0.0000 },
  { 1.3375, 2.53125, -0.7490 },
  { 0.7490, 2.53125, -1.3375 },
  { 0.0000, 2.53125, -1.3375 },
  { 1.4375, 2.53125, 0.0000 },
  { 1.4375, 2.53125, -0.8050 },
  { 0.8050, 2.53125, -1.4375 },
  { 0.0000, 2.53125, -1.4375 },
  { 1.5000, 2.40000, 0.0000 },
  { 1.5000, 2.40000, -0.8400 },
  { 0.8400, 2.40000, -1.5000 },
  { 0.0000, 2.40000, -1.5000 },
  { 1.7500, 1.87500, 0.0000 },
  { 1.7500, 1.87500, -0.9800 },
  { 0.9800, 1.87500, -1.7500 },
  { 0.0000, 1.87500, -1.7500 },
  { 2.0000, 1.35000, 0.0000 },
  { 2.0000, 1.35000, -1.1200 },
  { 1.1200, 1.35000, -2.0000 },
  { 0.0000, 1.35000, -2.0000 },
  { 2.0000, 0.90000, 0.0000 },
  { 2.0000, 0.90000, -1.1200 },
  { 1.1200, 0.90000, -2.0000 },
  { 0.0000, 0.90000, -2.0000 },
  { -2.0000, 0.90000, 0.0000 },
  { 2.0000, 0.45000, 0.0000 },
  { 2.0000, 0.45000, -1.1200 },
  { 1.1200, 0.45000, -2.0000 },
  { 0.0000, 0.45000, -2.0000 },
  { 1.5000, 0.22500, 0.0000 },
  { 1.5000, 0.22500, -0.8400 },
  { 0.8400, 0.22500, -1.5000 },
  { 0.0000, 0.22500, -1.5000 },
  { 1.5000, 0.15000, 0.0000 },
  { 1.5000, 0.15000, -0.8400 },
  { 0.8400, 0.15000, -1.5000 },
  { 0.0000, 0.15000, -1.5000 },
  { -1.6000, 2.02500, 0.0000 },
  { -1.6000, 2.02500, -0.3000 },
  { -1.5000, 2.25000, -0.3000 },
  { -1.5000, 2.25000, 0.0000 },
  { -2.3000, 2.02500, 0.0000 },
  { -2.3000, 2.02500, -0.3000 },
  { -2.5000, 2.25000, -0.3000 },
  { -2.5000, 2.25000, 0.0000 },
  { -2.7000, 2.02500, 0.0000 },
  { -2.7000, 2.02500, -0.3000 },
  { -3.0000, 2.25000, -0.3000 },
  { -3.0000, 2.25000, 0.0000 },
  { -2.7000, 1.80000, 0.0000 },
  { -2.7000, 1.80000, -0.3000 },
  { -3.0000, 1.80000, -0.3000 },
  { -3.0000, 1.80000, 0.0000 },
  { -2.7000, 1.57500, 0.0000 },
  { -2.7000, 1.57500, -0.3000 },
  { -3.0000, 1.35000, -0.3000 },
  { -3.0000, 1.35000, 0.0000 },
  { -2.5000, 1.12500, 0.0000 },
  { -2.5000, 1.12500, -0.3000 },
  { -2.6500, 0.93750, -0.3000 },
  { -2.6500, 0.93750, 0.0000 },
  { -2.0000, 0.90000, -0.3000 },
  { -1.9000, 0.60000, -0.3000 },
  { -1.9000, 0.60000, 0.0000 },
  { 1.7000, 1.42500, 0.0000 },
  { 1.7000, 1.42500, -0.6600 },
  { 1.7000, 0.60000, -0.6600 },
  { 1.7000, 0.60000, 0.0000 },
  { 2.6000, 1.42500, 0.0000 },
  { 2.6000, 1.42500, -0.6600 },
  { 3.1000, 0.82500, -0.6600 },
  { 3.1000, 0.82500, 0.0000 },
  { 2.3000, 2.10000, 0.0000 },
  { 2.3000, 2.10000, -0.2500 },
  { 2.4000, 2.02500, -0.2500 },
  { 2.4000, 2.02500, 0.0000 },
  { 2.7000, 2.40000, 0.0000 },
  { 2.7000, 2.40000, -0.2500 },
  { 3.3000, 2.40000, -0.2500 },
  { 3.3000, 2.40000, 0.0000 },
  { 2.8000, 2.47500, 0.0000 },
  { 2.8000, 2.47500, -0.2500 },
  { 3.5250, 2.49375, -0.2500 },
  { 3.5250, 2.49375, 0.0000 },
  { 2.9000, 2.47500, 0.0000 },
  { 2.9000, 2.47500, -0.1500 },
  { 3.4500, 2.51250, -0.1500 },
  { 3.4500, 2.51250, 0.0000 },
  { 2.8000, 2.40000, 0.0000 },
  { 2.8000, 2.40000, -0.1500 },
  { 3.2000, 2.40000, -0.1500 },
  { 3.2000, 2.40000, 0.0000 },
  { 0.0000, 3.15000, 0.0000 },
  { 0.8000, 3.15000, 0.0000 },
  { 0.8000, 3.15000, -0.4500 },
  { 0.4500, 3.15000, -0.8000 },
  { 0.0000, 3.15000, -0.8000 },
  { 0.0000, 2.85000, 0.0000 },
  { 1.4000, 2.40000, 0.0000 },
  { 1.4000, 2.40000, -0.7840 },
  { 0.7840, 2.40000, -1.4000 },
  { 0.0000, 2.40000, -1.4000 },
  { 0.4000, 2.55000, 0.0000 },
  { 0.4000, 2.55000, -0.2240 },
  { 0.2240, 2.55000, -0.4000 },
  { 0.0000, 2.55000, -0.4000 },
  { 1.3000, 2.55000, 0.0000 },
  { 1.3000, 2.55000, -0.7280 },
  { 0.7280, 2.55000, -1.3000 },
  { 0.0000, 2.55000, -1.3000 },
  // The next 4 control points affect the outer edge
  // of the lid. They have been moved slightly to remove
  // the gap between the lid and the body. For the sake of
  // completeness, the original values are list in the
  // adjacent comments...
  { 1.4000, 2.40000, 0.0000 },  // 1.300, 2.4,  0.000
  { 1.4000, 2.40000, -0.7480 }, // 1.300, 2.4, -0.728
  { 0.7480, 2.40000, -1.4000 }, // 0.728, 2.4, -1.300
  { 0.0000, 2.40000, -1.4000 }, // 0.000, 2.4, -1.300
  { 0.0000, 0.00000, 0.0000 },
  { 1.4250, 0.00000, -0.7980 },
  { 1.5000, 0.07500, 0.0000 },
  { 1.4250, 0.00000, 0.0000 },
  { 0.7980, 0.00000, -1.4250 },
  { 0.0000, 0.07500, -1.5000 },
  { 0.0000, 0.00000, -1.4250 },
  { 1.5000, 0.07500, -0.8400 },
  { 0.8400, 0.07500, -1.5000 },
};

//-------------------------------------------------------------------
// individual teapot patchs
//-------------------------------------------------------------------
patch_t rim = {
  { 102, 103, 104, 105 },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11 },
  { 12, 13, 14, 15 },
};

patch_t body1 = {
  { 12, 13, 14, 15},
  { 16, 17, 18, 19},
  { 20, 21, 22, 23},
  { 24, 25, 26, 27 }
};

patch_t body2 = {
  { 24, 25, 26, 27},
  { 29, 30, 31, 32},
  { 33, 34, 35, 36},
  { 37, 38, 39, 40 }
};

patch_t lid1 = {
  { 96, 96, 96, 96},
  { 97, 98, 99, 100},
  { 101, 101, 101, 101},
  { 0, 1, 2, 3 }
};

patch_t lid2 = {
  { 0, 1, 2, 3},
  { 106, 107, 108, 109},
  { 110, 111, 112, 113},
  { 114, 115, 116, 117 }
};

patch_t handle1 = {
  { 41, 42, 43, 44},
  { 45, 46, 47, 48},
  { 49, 50, 51, 52},
  { 53, 54, 55, 56 }
};

patch_t handle2 = {
  { 53, 54, 55, 56},
  { 57, 58, 59, 60},
  { 61, 62, 63, 64},
  { 28, 65, 66, 67 }
};

patch_t spout1 = {
  { 68, 69, 70, 71},
  { 72, 73, 74, 75},
  { 76, 77, 78, 79},
  { 80, 81, 82, 83 }
};

patch_t spout2 = {
  { 80, 81, 82, 83},
  { 84, 85, 86, 87},
  { 88, 89, 90, 91},
  { 92, 93, 94, 95 }
};

patch_t bottom = {
  { 118, 118, 118, 118 },
  { 124, 122, 119, 121 },
  { 123, 126, 125, 120 },
  { 40, 39, 38, 37 }
};

//-------------------------------------------------------------------
// complete teapot patch sets
//-------------------------------------------------------------------

#define NUM_PATCHES 32

patch_set_t patches[3][NUM_PATCHES] = {
  {
    { MIRROR_NONE, &lid2,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &lid2,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_XY,   &lid2,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &lid2,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &lid1,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &lid1,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_XY,   &lid1,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &lid1,    false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &rim,     false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &rim,     false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_XY,   &rim,     false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &rim,     false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &body1,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &body1,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_XY,   &body1,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &body1,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &body2,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &body2,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_XY,   &body2,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &body2,   false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &bottom,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &bottom,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_XY,   &bottom,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &bottom,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &handle1, false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &handle1, false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &handle2, false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &handle2, false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &spout1,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &spout1,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &spout2,  false, false, 1.00, 1.00, 0.00, 0.00 },
    { MIRROR_Y,    &spout2,  false, false, 1.00, 1.00, 0.00, 0.00 },
  },
  {
    { MIRROR_NONE, &lid2,    false,  true, 0.25, 0.50, 0.00, 0.00 },
    { MIRROR_X,    &lid2,     true,  true, 0.25, 0.50, 0.25, 0.00 },
    { MIRROR_XY,   &lid2,    false,  true, 0.25, 0.50, 0.50, 0.00 },
    { MIRROR_Y,    &lid2,     true,  true, 0.25, 0.50, 0.75, 0.00 },
    { MIRROR_NONE, &lid1,    false,  true, 0.25, 0.50, 0.00, 0.50 },
    { MIRROR_X,    &lid1,     true,  true, 0.25, 0.50, 0.25, 0.50 },
    { MIRROR_XY,   &lid1,    false,  true, 0.25, 0.50, 0.50, 0.50 },
    { MIRROR_Y,    &lid1,     true,  true, 0.25, 0.50, 0.75, 0.50 },
    { MIRROR_NONE, &rim,     false, false, 0.25, 0.10, 0.00, 0.00 },
    { MIRROR_X,    &rim,      true, false, 0.25, 0.10, 0.25, 0.00 },
    { MIRROR_XY,   &rim,     false, false, 0.25, 0.10, 0.50, 0.00 },
    { MIRROR_Y,    &rim,      true, false, 0.25, 0.10, 0.75, 0.00 },
    { MIRROR_NONE, &body1,   false, false, 0.25, 0.55, 0.00, 0.10 },
    { MIRROR_X,    &body1,    true, false, 0.25, 0.55, 0.25, 0.10 },
    { MIRROR_XY,   &body1,   false, false, 0.25, 0.55, 0.50, 0.10 },
    { MIRROR_Y,    &body1,    true, false, 0.25, 0.55, 0.75, 0.10 },
    { MIRROR_NONE, &body2,   false, false, 0.25, 0.35, 0.00, 0.65 },
    { MIRROR_X,    &body2,    true, false, 0.25, 0.35, 0.25, 0.65 },
    { MIRROR_XY,   &body2,   false, false, 0.25, 0.35, 0.50, 0.65 },
    { MIRROR_Y,    &body2,    true, false, 0.25, 0.35, 0.75, 0.65 },
    { MIRROR_NONE, &bottom,   true, false, 0.25, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &bottom,  false, false, 0.25, 1.00, 0.25, 0.00 },
    { MIRROR_XY,   &bottom,   true, false, 0.25, 1.00, 0.50, 0.00 },
    { MIRROR_Y,    &bottom,  false, false, 0.25, 1.00, 0.75, 0.00 },
    { MIRROR_NONE, &handle1, false, false, 0.50, 0.50, 0.00, 0.00 },
    { MIRROR_Y,    &handle1,  true, false, 0.50, 0.50, 0.50, 0.00 },
    { MIRROR_NONE, &handle2, false, false, 0.50, 0.50, 0.00, 0.50 },
    { MIRROR_Y,    &handle2,  true, false, 0.50, 0.50, 0.50, 0.50 },
    { MIRROR_NONE, &spout1,  false, false, 0.50, 0.90, 0.00, 0.00 },
    { MIRROR_Y,    &spout1,   true, false, 0.50, 0.90, 0.50, 0.00 },
    { MIRROR_NONE, &spout2,  false, false, 0.50, 0.10, 0.00, 0.90 },
    { MIRROR_Y,    &spout2,   true, false, 0.50, 0.10, 0.50, 0.90 },
  },
  {
    { MIRROR_NONE, &lid2,    false,  true, 0.50, 0.50, 0.00, 0.00 },
    { MIRROR_X,    &lid2,     true,  true, 0.50, 0.50, 0.50, 0.00 },
    { MIRROR_XY,   &lid2,     true,  true, 0.50, 0.50, 0.50, 0.00 },
    { MIRROR_Y,    &lid2,    false,  true, 0.50, 0.50, 0.00, 0.00 },
    { MIRROR_NONE, &lid1,    false,  true, 0.50, 0.50, 0.00, 0.50 },
    { MIRROR_X,    &lid1,     true,  true, 0.50, 0.50, 0.50, 0.50 },
    { MIRROR_XY,   &lid1,     true,  true, 0.50, 0.50, 0.50, 0.50 },
    { MIRROR_Y,    &lid1,    false,  true, 0.50, 0.50, 0.00, 0.50 },
    { MIRROR_NONE, &rim,     false, false, 0.50, 0.10, 0.00, 0.00 },
    { MIRROR_X,    &rim,      true, false, 0.50, 0.10, 0.50, 0.00 },
    { MIRROR_XY,   &rim,      true, false, 0.50, 0.10, 0.50, 0.00 },
    { MIRROR_Y,    &rim,     false, false, 0.50, 0.10, 0.00, 0.00 },
    { MIRROR_NONE, &body1,   false, false, 0.50, 0.55, 0.00, 0.10 },
    { MIRROR_X,    &body1,    true, false, 0.50, 0.55, 0.50, 0.10 },
    { MIRROR_XY,   &body1,    true, false, 0.50, 0.55, 0.50, 0.10 },
    { MIRROR_Y,    &body1,   false, false, 0.50, 0.55, 0.00, 0.10 },
    { MIRROR_NONE, &body2,   false, false, 0.50, 0.35, 0.00, 0.65 },
    { MIRROR_X,    &body2,    true, false, 0.50, 0.35, 0.50, 0.65 },
    { MIRROR_XY,   &body2,    true, false, 0.50, 0.35, 0.50, 0.65 },
    { MIRROR_Y,    &body2,   false, false, 0.50, 0.35, 0.00, 0.65 },
    { MIRROR_NONE, &bottom,   true, false, 0.50, 1.00, 0.00, 0.00 },
    { MIRROR_X,    &bottom,  false, false, 0.50, 1.00, 0.50, 0.00 },
    { MIRROR_XY,   &bottom,  false, false, 0.50, 1.00, 0.50, 0.00 },
    { MIRROR_Y,    &bottom,   true, false, 0.50, 1.00, 0.00, 0.00 },
    { MIRROR_NONE, &handle1, false, false, 1.00, 0.50, 0.00, 0.00 },
    { MIRROR_Y,    &handle1, false, false, 1.00, 0.50, 0.00, 0.00 },
    { MIRROR_NONE, &handle2, false, false, 1.00, 0.50, 0.00, 0.50 },
    { MIRROR_Y,    &handle2, false, false, 1.00, 0.50, 0.00, 0.50 },
    { MIRROR_NONE, &spout1,  false, false, 1.00, 0.90, 0.00, 0.00 },
    { MIRROR_Y,    &spout1,  false, false, 1.00, 0.90, 0.00, 0.00 },
    { MIRROR_NONE, &spout2,  false, false, 1.00, 0.10, 0.00, 0.90 },
    { MIRROR_Y,    &spout2,  false, false, 1.00, 0.10, 0.00, 0.90 },
  },
};

//-------------------------------------------------------------------
// norm
//-------------------------------------------------------------------
void norm(point_t& p)
  {
  double l = 1/sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]); 
  p[0] *= l;
  p[1] *= l;
  p[2] *= l;
  }

//-------------------------------------------------------------------
// len
//-------------------------------------------------------------------
double len(point_t& p)
  {
  return sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]); 
  }

//-------------------------------------------------------------------
// usage
//-------------------------------------------------------------------
void usage(void)
  {
  printf("Usage: teapot [options]\n");
  printf("Options:\n");
  printf("    -u <num>    Sampling in u direction (default: 10)\n");
  printf("    -v <num>    Sampling in v direction (default: 10)\n");
  printf("    -n          No normals\n");
  printf("    -t          No texture coordinates\n");
  printf("    -g          No tangents\n");
  printf("    -1          Texture coordinates set #1\n");
  printf("    -2          Texture coordinates set #2\n");
  printf("    -3          Texture coordinates set #3\n");
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
// eval_curve
//-------------------------------------------------------------------
void eval_curve(const point_t& a,
		const point_t& b,
		const point_t& c,
		const point_t& d,
		double u,
		point_t* r,
		point_t* du)
  {
  for(int i = 0; i < 3; i++)
    {
    // evaluate curve
    double a0 = (1-u)*a[i] + u*b[i];
    double b0 = (1-u)*b[i] + u*c[i];
    double c0 = (1-u)*c[i] + u*d[i];
    double a1 = (1-u)*a0 + u*b0;
    double b1 = (1-u)*b0 + u*c0;
    double a2 = (1-u)*a1 + u*b1;
    if (r) (*r)[i] = a2;
    
    // evaluate derivative
    double da0 = (1-u)*(b[i] - a[i]) + u*(c[i] - b[i]);
    double db0 = (1-u)*(c[i] - b[i]) + u*(d[i] - c[i]);
    double da1 = (1-u)*da0 + u*db0;
    if (du) (*du)[i] = da1;
    }
  }

//-------------------------------------------------------------------
// eval_surface
//-------------------------------------------------------------------
void eval_surface(const patch_t& p,
		  double u,
		  double v,
		  point_t* r,
		  point_t* du,
		  point_t* dv)
  {
  point_t tmp[4];
  point_t dtmp[4];
  
  for(int i = 0; i < 4; i++)
    {
    // evaluate surface in u
    eval_curve(cpts[p[i][0]],
	       cpts[p[i][1]],
	       cpts[p[i][2]],
	       cpts[p[i][3]],
	       u,
	       &tmp[i],
	       &dtmp[i]);
    }
  
  // evaluate surface in v
  eval_curve(tmp[0], tmp[1], tmp[2], tmp[3], v, r, dv);
  eval_curve(dtmp[0], dtmp[1], dtmp[2], dtmp[3], v, du, NULL);
  }

//-------------------------------------------------------------------
// bisect
//-------------------------------------------------------------------
void bisect(const patch_t& p, 
	    double u0, double v0,
	    double u, double v,
	    point_t* du, point_t* dv)
  {
  // evaluate the starting end of the bisection
  // to get a initial, but crude, tangent vector
  point_t r;
  point_t tdu;
  point_t tdv;
  eval_surface(p, u0, v0, &r, &tdu, &tdv);

  // if the starting point of the bisection is bad
  // then we have real problems...
  if (len(tdu) < 1e-10 || len(tdv) < 1e-10)
    {
    fprintf(stderr, "bisection failed...\n");
    return;
    }

  int steps = 0;
  while(1)
    {
    if (steps == 40)
      {
      fprintf(stderr, "bisection aborted, too many iterations...\n");
      return;
      }

    double mu = (u0 + u)/2;
    double mv = (v0 + v)/2;
    point_t mdu;
    point_t mdv;
    eval_surface(p, mu, mv, &r, &mdu, &mdv);

    if (len(mdu) < 1e-10 || len(mdv) < 1e-10)
      {
      // the current iteration produced a bad set of tangents vector, we
      // are done, return the tangent vectors from the previous
      // iteration
      for(int i = 0; i < 3; i++) (*du)[i] = tdu[i];
      for(int i = 0; i < 3; i++) (*dv)[i] = tdv[i];
      return;
      }

    u0 = mu;
    v0 = mv;
    for(int i = 0; i < 3; i++) tdu[i] = mdu[i];
    for(int i = 0; i < 3; i++) tdv[i] = mdv[i];
    steps++;
    }
  }

//-------------------------------------------------------------------
// main
//-------------------------------------------------------------------
int main(int argc, char** argv)
  {
  int idx = 0;
  int nu = 10;
  int nv = 10;
  bool normals = true;
  bool texcoords = true;
  bool tangents = true;

  int c;
  while((c = getopt(argc, argv, "u:v:ntg123")) != -1)
    {
    switch(c)
      {
      case 'u':
	nu = atoi(optarg);
	break;
      case 'v':
	nv = atoi(optarg);
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
      case '1':
	idx = 0;
	break;
      case '2':
	idx = 1;
	break;
      case '3':
	idx = 2;
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
  
  // vertices 
  for(unsigned int p = 0; p < NUM_PATCHES; p++)
    {
    for(int i = 0; i <= nu; i++)
      {
      for(int j = 0; j <= nv; j++)
	{
	double u = ((double)i/nu);
	double v = ((double)j/nv);
	point_t r;
	point_t du;
	point_t dv;
	eval_surface(*(patches[idx][p].p), u, v, &r, &du, &dv);

	// evaluate length of tangent vectors, if they are close
	// to zero we need todo some additional work
	if (len(du) < 1e-10 || len(dv) < 1e-10)
	  {
	  double u0;
	  double v0;

	  if (i == nu)
	    {
	    u0 = ((double)(i-1)/nu);
	    }
	  else
	    {
	    u0 = ((double)(i+1)/nu);
	    }
	  
	  if (j == nv)
	    {
	    v0 = ((double)(j-1)/nv);
	    }
	  else
	    {
	    v0 = ((double)(j+1)/nv);
	    }

	  bisect(*(patches[idx][p].p), u0, v0, u, v, &du, &dv);
	  }

	// normalize du & dv
	norm(du);
	norm(dv);

	// cross two tangents to get normal
	point_t n;
	n[0] = dv[1]*du[2] - du[1]*dv[2];
	n[1] = dv[2]*du[0] - du[2]*dv[0];
	n[2] = dv[0]*du[1] - du[0]*dv[1];
	norm(n);

	double s = (patches[idx][p].u_scale*(patches[idx][p].u_flip ? (1-u) : u) +
		    patches[idx][p].u_bias);
	double t = (patches[idx][p].v_scale*(patches[idx][p].v_flip ? (1-v) : v) +
		    patches[idx][p].v_bias);

	if (patches[idx][p].u_flip)
	  {
	  du[0] = -du[0];
	  du[1] = -du[1];
	  du[2] = -du[2];
	  }

	switch(patches[idx][p].mirror)
	  {
	  case MIRROR_NONE:
	    {
	    printf("v %f %f %f\n", r[0], r[1], r[2]);

	    if (texcoords)
	      {
	      printf("vt %f %f\n", s, t);
	      }

	    if (tangents)
	      {
	      printf("r %f %f %f\n", du[0], du[1], du[2]);
	      }

	    if (normals)
	      {
	      printf("vn %f %f %f\n", n[0], n[1], n[2]);
	      }

	    break;
	    }
	  case MIRROR_X:
	    {
	    printf("v %f %f %f\n", -r[0], r[1], r[2]);

	    if (texcoords)
	      {
	      printf("vt %f %f\n", s, t);
	      }

	    if (tangents)
	      {
	      printf("r %f %f %f\n", -du[0], du[1], du[2]);
	      }

	    if (normals)
	      {
	      printf("vn %f %f %f\n", -n[0], n[1], n[2]);
	      }

	    break;
	    }
	  case MIRROR_Y:
	    {
	    printf("v %f %f %f\n", r[0], r[1], -r[2]);

	    if (texcoords)
	      {
	      printf("vt %f %f\n", s, t);
	      }

	    if (tangents)
	      {
	      printf("r %f %f %f\n", du[0], du[1], -du[2]);
	      }

	    if (normals)
	      {
	      printf("vn %f %f %f\n", n[0], n[1], -n[2]);
	      }

	    break;
	    }
	  case MIRROR_XY:
	    {
	    printf("v %f %f %f\n", -r[0], r[1], -r[2]);

	    if (texcoords)
	      {
	      printf("vt %f %f\n", s, t);
	      }

	    if (tangents)
	      {
	      printf("r %f %f %f\n", -du[0], du[1], -du[2]);
	      }

	    if (normals)
	      {
	      printf("vn %f %f %f\n", -n[0], n[1], -n[2]);
	      }

	    break;
	    }
	  }
	}
      }
    }

  // faces
  for(unsigned int p = 0; p < NUM_PATCHES; p++)
    {
    for(int i = 0; i < nu; i++)
      {
      for(int j = 0; j < nv; j++)
	{
	int p0 = 1 + (nu+1)*(nv+1)*p + (nv+1)*i + j;
	int p1 = 1 + (nu+1)*(nv+1)*p + (nv+1)*i + (j+1);
	int p2 = 1 + (nu+1)*(nv+1)*p + (nv+1)*(i+1) + j;
	int p3 = 1 + (nu+1)*(nv+1)*p + (nv+1)*(i+1) + (j+1);

	switch(patches[idx][p].mirror)
	  {
	  case MIRROR_NONE:
	    {
	    print_face(p0, p2, p1, normals, texcoords);
	    print_face(p1, p2, p3, normals, texcoords);
	    break;
	    }
	  case MIRROR_X:
	    {
	    print_face(p0, p1, p2, normals, texcoords);
	    print_face(p2, p1, p3, normals, texcoords);
	    break;
	    }
	  case MIRROR_Y:
	    {
	    print_face(p0, p1, p2, normals, texcoords);
	    print_face(p2, p1, p3, normals, texcoords);
	    break;
	    }
	  case MIRROR_XY:
	    {
	    print_face(p0, p2, p1, normals, texcoords);
	    print_face(p1, p2, p3, normals, texcoords);
	    break;
	    }
	  }
	}
      }
    }
  
  return 0;
  }
