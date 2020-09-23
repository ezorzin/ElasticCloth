/// @file

#version 410 core

uniform mat4 V_mat;                                                             // View matrix.
uniform mat4 P_mat;                                                             // Projection matrix.

in  vec4 voxel_color;                                                           // Voxel color.
in  vec4 voxel_center;                                                          // Voxel center.
in  vec2 quad;
out vec4 fragment_color;                                                        // Fragment color.

void main(void)
{
  vec2 P;                                                                       // 2D fragment coordinates of the voxel.
  vec2 Q;
  float R;                                                                      // Radius of the voxel.
  float k1;                                                                     // Smoothness coefficient.
  float k2;
  float k3;

  P = 2.0*(quad - vec2(0.5, 0.5));

  if (length(P) > 1.0)
  {
    discard;                                                                    // Discarding fragment point...
  }

  fragment_color = voxel_color;
}
