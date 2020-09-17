/// @file

#version 410 core

in  vec4 voxel_color;                                                           // Voxel color.
out vec4 fragment_color;                                                        // Fragment color.

void main(void)
{
  vec2 temp;
  float k;

  temp = gl_PointCoord - vec2(0.5);
  float f = dot(temp, temp);

  if (f > 0.25)
  {
    discard;
  }
  else
  {
    k = 1.0 - smoothstep(0.0, 0.25, f);
  }

  fragment_color = k*voxel_color;
}
