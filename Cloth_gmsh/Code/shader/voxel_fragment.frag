/// @file

#version 410 core

in  vec4 voxel_color;                                                           // Voxel color.
in  vec2 pointPos;
out vec4 fragment_color;                                                        // Fragment color.

const float T = 0.3;
const float R = 5;

void main(void)
{
  vec2 temp = gl_PointCoord - vec2(0.5);
  float f = dot(temp, temp);
  if (f>0.25) discard;

  //fragment_color = vec4(color, 1.0);
  fragment_color = voxel_color;
}
