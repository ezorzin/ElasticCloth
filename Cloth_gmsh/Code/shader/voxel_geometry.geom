/// @file
#version 410 core

layout (points) in;                                                             // Input points.
layout (points, max_vertices = 26) out;                                         // Output points.

in VS_OUT
{
  vec4 color;                                                                   // Color.
  vec4 center;                                                                  // Center.
  mat4 V_mat;
  mat4 P_mat;
} gs_in[];

out vec4 voxel_color;                                                           // Voxel color (for fragment shader).
out vec4 voxel_center;
out vec4 voxel_point;

const float size = 30;

void main()
{                           
  mat4 V_mat;                                                                   // View matrix.
  mat4 P_mat;                                                                   // Projection matrix.

  V_mat = gs_in[0].V_mat;                                                       // Getting view matrix...
  P_mat = gs_in[0].P_mat;                                                       // Getting projection matrix...
  voxel_color = gs_in[0].color;                                                 // Setting voxel color...
  voxel_center = gs_in[0].center;                                               // Setting voxel center...  
  voxel_point = P_mat*V_mat*voxel_center;
  gl_Position = voxel_point;                                                    // Setting voxel position...
  gl_PointSize = (1.0 - gl_Position.z / gl_Position.w) * size;                  // Computing voxel point size...
  EmitVertex();                                                                 // Emitting vertex...
  EndPrimitive();                                                               // Ending primitive...
}
