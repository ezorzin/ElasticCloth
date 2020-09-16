/// @file
#version 410 core

layout (points) in;                                                             // Input points.
layout (points, max_vertices = 26) out;                                 // Output points.

in VS_OUT
{
  vec4 color;                                                                   // Color.
  vec4 center;                                                                  // Center.
  mat4 V_mat;
  mat4 P_mat;
} gs_in[];

out vec4 voxel_color;                                                           // Voxel color (for fragment shader).
out vec4 voxel_center;                                                          // Voxel center (for fragment shader).

void main()
{
  mat4 V_mat = gs_in[0].V_mat;
  mat4 P_mat = gs_in[0].P_mat;

  voxel_color = gs_in[0].color;                                                 // Setting voxel color...
  voxel_center = P_mat*V_mat*gs_in[0].center;                                   // Setting voxel center...
  gl_Position = voxel_center;                                                   // Setting voxel position...
  EmitVertex();         
  EndPrimitive();                                                        
}
