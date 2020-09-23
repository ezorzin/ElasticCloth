/// @file
#version 410 core

layout (location = 0) in vec4 voxel_color;                                      // Voxel color.
layout (location = 1) in vec4 voxel_center;                                     // Voxel center.

uniform mat4 V_mat;                                                             // View matrix.
uniform mat4 P_mat;                                                             // Projection matrix.

out VS_OUT
{
  vec4 color;                                                                   // Color.
  vec4 center;                                                                  // Center.
} vs_out;

/// @function
// Computing rendering point coordinates:
void main(void)
{
  vs_out.color = voxel_color;                    
  vs_out.center = voxel_center;        
}
