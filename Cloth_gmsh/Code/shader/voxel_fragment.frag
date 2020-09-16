/// @file

#version 410 core

in  vec4 voxel_color;                                                           // Voxel color.
in  vec4 voxel_center;                                                          // Voxel center.
out vec4 fragment_color;                                                        // Fragment color.

void main(void)
{
  //float alpha = step(0.45, 0.7);
  //fragment_color = mix(vec4(1.0, 0.0, 0.0, 1.0), voxel_color, alpha);
  fragment_color = voxel_color;
}
