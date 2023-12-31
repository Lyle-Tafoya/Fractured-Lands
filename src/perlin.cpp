/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include <cmath>
#include "perlin.hpp"

PerlinNoise::PerlinNoise()
{
  for(int i = 0; i < 256; ++i)
  {
    p[i] = permutation[i];
    p[256+i] = permutation[i];
  }
}

const int PerlinNoise::permutation[] = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

float PerlinNoise::noise(float x, float y, float z)
{
  int xi = static_cast<int>(std::floor(x)) & 255;
  int yi = static_cast<int>(std::floor(y)) & 255;
  int zi = static_cast<int>(std::floor(z)) & 255;
  x -= std::floor(x);
  y -= std::floor(y);
  z -= std::floor(z);
  float u = fade(x);
  float v = fade(y);
  float w = fade(z);
  int a = p[xi] + yi;
  int aa = p[a] + zi;
  int ab = p[a+1]+zi;
  int b = p[xi+1]+yi;
  int ba = p[b]+zi;
  int bb = p[b+1]+zi;

  return lerp(w, lerp(v, lerp(u, grad(p[aa], x, y, z), grad(p[ba], x-1, y, z)),
                         lerp(u, grad(p[ab], x, y-1, z), grad(p[bb], x-1, y-1, z))),
                 lerp(v, lerp(u, grad(p[aa+1], x, y, z-1), grad(p[ba+1], x-1, y, z-1)),
                         lerp(u, grad(p[ab+1], x, y-1, z-1), grad(p[bb+1], x-1, y-1, z-1))));
}

float PerlinNoise::fade(float t)
{
  return t * t * t * (t * (t*6 - 15) + 10);
}

float PerlinNoise::lerp(float t, float a, float b)
{
  return a + t * (b-a);
}

float PerlinNoise::grad(int hash, float x, float y, float z)
{
  int h = hash & 15;
  float u = h < 8 ? x : y;
  float v = h < 4 ? y : h == 12 || h ==  14 ? x : z;
  return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}
