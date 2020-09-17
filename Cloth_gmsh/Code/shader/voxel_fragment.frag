/// @file

#version 410 core

in  vec4 voxel_color;                                                           // Voxel color.
in  vec2 pointPos;
out vec4 fragment_color;                                                        // Fragment color.

const float T = 0.3;
const float R = 0.5;

void main(void)
{
  float dist;
  float d;
  vec3 color;
  
  /*
  dist = distance(pointPos, gl_FragCoord.xy);

  if (dist > R)
  {
    discard;
  }
   
  d = dist/R;
  color = mix(voxel_color.rgb, vec3(0.0), step(1.0 - T, d));
*/
  //fragment_color = vec4(color, 1.0);
  fragment_color = voxel_color;
}
