/// @file

#version 410 core

in  vec4 voxel_color;                                                           // Voxel color.
in  vec4 voxel_center; 
in  vec4 voxel_point;
out vec4 fragment_color;                                                        // Fragment color.

void main(void)
{
  vec2 P;                                                                       // 2D fragment coordinates of the voxel.
  float R;                                                                      // Radius of the voxel.
  float k;                                                                      // Smoothness coefficient.
  float alpha;

  P = gl_PointCoord;                                                            // Getting fragment coordinates...
  R = distance(P, vec2(0.5, 0.5));                                              // Computing voxel radius...
  k = 1.0 - smoothstep(0.0, 0.5, R);                                            // Computing smoothness coefficient...
  alpha = 1.0 - smoothstep(0.1, 1.0, R);

  if (k == 0.0)
  {
    discard;                                                                    // Discarding fragment point...
  }

  fragment_color = vec4((0.5 + k)*voxel_color.rgb, 0.5);
}
