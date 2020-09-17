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
out vec2 pointPos;

void main()
{
  vec4 voxel_center;
  vec2 ndcPos;
  vec2 screen;
  mat4 V_mat;
  mat4 P_mat;

  V_mat = gs_in[0].V_mat;
  P_mat = gs_in[0].P_mat;
  voxel_color = gs_in[0].color;                                                 // Setting voxel color...
  voxel_center = gs_in[0].center;                                               // Setting voxel center...  
  gl_Position = P_mat*V_mat*voxel_center;
  gl_PointSize = 10.0;
  EmitVertex();
  EndPrimitive();                

  screen = vec2(800.0, 600.0);
  ndcPos = gl_Position.xy/gl_Position.w;
  pointPos = screen*(0.5*ndcPos + 0.5);                      
}
