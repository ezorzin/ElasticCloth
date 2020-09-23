/// @file

#version 410 core

uniform mat4 V_mat;                                                             // View matrix.
uniform mat4 P_mat;                                                             // Projection matrix.
uniform float AR;                                                               // Viewport aspect ratio.
uniform int size_x;
uniform int size_y;

in  vec4 voxel_color;                                                           // Voxel color.
in  vec4 voxel_center;                                                          // Voxel center.
out vec4 fragment_color;                                                        // Fragment color.

void main(void)
{
  vec4 P;                                                                       // 2D fragment coordinates of the voxel.
  vec2 Q;
  float R;                                                                      // Radius of the voxel.
  float k1;                                                                     // Smoothness coefficient.
  float k2;
  float k3;

  P = voxel_center;                                                 // Getting fragment coordinates...
  Q = vec2(gl_FragCoord.x/size_x, gl_FragCoord.y/size_y);
  R = distance(P.xy, Q);                                                        // Computing voxel radius...
  k1 = 1.0 - smoothstep(0.0, 0.5, R);                                           // Computing smoothness coefficient...
  k2 = 1.0 - smoothstep(0.0, 0.1, R);
  k3 = 1.0 - smoothstep(0.2, 0.3, R);

  if (k1 == 0.0)
  {
    discard;                                                                    // Discarding fragment point...
  }

  fragment_color = vec4(0.8*vec3(k2, 1.2*k3, k1) + voxel_color.rgb, 0.2 + k1);
  //fragment_color = voxel_color;
}
