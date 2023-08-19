#ifndef FRACTURED_LANDS_PERLIN_NOISE_HPP_
#define FRACTURED_LANDS_PERLIN_NOISE_HPP_

class PerlinNoise
{
public:
  float noise(float x, float y, float z);
  PerlinNoise();

private:
  static float fade(float t);
  static float lerp(float t, float a, float b);
  static float grad(int hash, float x, float y, float z);
  static const int permutation[];
  int p[512];
};

#endif
