/// @file
#include "utilities.cl"

__kernel void thekernel(__global float4*    position,                                               // Position [m].
                        __global float4*    color                                                   // Depth color [#]
                        )
{
        unsigned long gid = get_global_id(0);                                                       // Global index [#].
        float4 P = position[gid];                                                                   // Getting point coordinates [m]...
        float4 C = color[gid];                                                                      // Getting color coordinates [#]...

        position[gid] = P;                                                                          // Updating position [m]...
        color[gid] = C;                                                                             // Updating color...
}
