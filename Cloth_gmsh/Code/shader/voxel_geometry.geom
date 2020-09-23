/// @file
#version 410 core

#define s 0.005

layout (points) in;                                                             // Input points.
layout (triangle_strip, max_vertices = 4) out;                                  // Output points.

uniform mat4 V_mat;                                                             // View matrix.
uniform mat4 P_mat;                                                             // Projection matrix.
uniform float AR;                                                               // Viewport aspect ratio.

in VS_OUT
{
  vec4 color;                                                                   // Color.
  vec4 center;                                                                  // Center.
} gs_in[];

out vec4 voxel_color;                                                           // Voxel color (for fragment shader).
out vec4 voxel_center;                                                          // Voxel center (for fragment shader).

void main()
{              
  vec4 A = vec4(-s, -s*AR, 0.0, 0.0);
  vec4 B = vec4(+s, -s*AR, 0.0, 0.0);
  vec4 C = vec4(-s, +s*AR, 0.0, 0.0);
  vec4 D = vec4(+s, +s*AR, 0.0, 0.0);            
  
  voxel_center = gs_in[0].center;                                               // Setting voxel center...              

  voxel_color = gs_in[0].color;                                                 // Setting voxel color...  
  gl_Position = P_mat*V_mat*(gs_in[0].center) + A;                              // Setting voxel position...
  EmitVertex();                                                                 // Emitting vertex...

  voxel_color = gs_in[0].color;                                                 // Setting voxel color...
  gl_Position = P_mat*V_mat*(gs_in[0].center) + B;                              // Setting voxel position...
  EmitVertex();                                                                 // Emitting vertex...

  voxel_color = gs_in[0].color;                                                 // Setting voxel color...
  gl_Position = P_mat*V_mat*(gs_in[0].center) + C;                              // Setting voxel position...
  EmitVertex();                                                                 // Emitting vertex...

  voxel_color = gs_in[0].color;                                                 // Setting voxel color...
  gl_Position = P_mat*V_mat*(gs_in[0].center) + D;                              // Setting voxel position...
  EmitVertex();                                                                 // Emitting vertex...

  EndPrimitive();                                                               // Ending primitive...
}
