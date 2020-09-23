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
  float k1;                                                                     // Smoothness coefficient.
  float k2;
  float k3;
  float R;

  R = length(quad);

  k1 = 1.0 - smoothstep(0.0, 0.5, R);                                           // Computing smoothness coefficient...
  k2 = 1.0 - smoothstep(0.0, 0.1, R);
  k3 = 1.0 - smoothstep(0.2, 0.3, R);

  if (k1 == 0.0)
  {
    discard;                                                                    // Discarding fragment point...
  }

  fragment_color = vec4(0.8*vec3(k2, 1.2*k3, k1) + voxel_color.rgb, 0.2 + k1);
}
