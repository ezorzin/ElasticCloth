/// @file

// OPENGL:
#define INTEROP       true                                                                          // "true" = use OpenGL-OpenCL interoperability.
#define GUI_SIZE_X    800                                                                           // Window x-size [px].
#define GUI_SIZE_Y    600                                                                           // Window y-size [px].
#define GUI_NAME      "Neutrino - Cloth_gmsh"                                                       // Window name.

#ifdef __linux__
  #define SHADER_HOME "../Cloth_gmsh/Code/shader"                                                   // Linux OpenGL shaders directory.
  #define KERNEL_HOME "../Cloth_gmsh/Code/kernel"                                                   // Linux OpenCL kernels directory.
  #define GMSH_HOME   "../Cloth_gmsh/Code/mesh/"                                                    // Linux GMSH mesh directory.
#endif

#ifdef __APPLE__
  #define SHADER_HOME "../Cloth_gmsh/Code/shader"                                                   // Mac OpenGL shaders directory.
  #define KERNEL_HOME "../Cloth_gmsh/Code/kernel"                                                   // Mac OpenCL kernels directory.
  #define GMSH_HOME   "../Cloth_gmsh/Code/mesh/"                                                    // Linux GMSH mesh directory.
#endif

#ifdef WIN32
  #define SHADER_HOME "..\\..\\Cloth_gmsh\\Code\\shader"                                            // Windows OpenGL shaders directory.
  #define KERNEL_HOME "..\\..\\Cloth_gmsh\\Code\\kernel"                                            // Windows OpenCL kernels directory.
  #define GMSH_HOME   "..\\..\\Cloth_gmsh\\Code\\mesh\\"                                            // Linux GMSH mesh directory.
#endif

#define SHADER_VERT   "voxel_vertex.vert"                                                           // OpenGL vertex shader.
#define SHADER_GEOM   "voxel_geometry.geom"                                                         // OpenGL geometry shader.
#define SHADER_FRAG   "voxel_fragment.frag"                                                         // OpenGL fragment shader.
#define GMSH_MESH     "Square.msh"                                                                  // GMSH mesh.

// OPENCL:
#define QUEUE_NUM     1                                                                             // # of OpenCL queues [#].
#define KERNEL_NUM    2                                                                             // # of OpenCL kernel [#].

// INCLUDES:
#include "nu.hpp"                                                                                   // Neutrino's header file.

int main ()
{
  // KERNEL FILES:
  std::string              kernel_home;                                                             // Kernel home directory.
  std::vector<std::string> kernel_1;                                                                // Kernel_1 source files.
  std::vector<std::string> kernel_2;                                                                // Kernel_2 source files.

  // DATA:
  float                    x_min              = -1.0;                                               // "x_min" spatial boundary [m].
  float                    x_max              = +1.0;                                               // "x_max" spatial boundary [m].
  float                    y_min              = -1.0;                                               // "y_min" spatial boundary [m].
  float                    y_max              = +1.0;                                               // "y_max" spatial boundary [m].
  size_t                   nodes_x            = 100;                                                // # of nodes in "X" direction [#].
  size_t                   nodes_y            = 100;                                                // # of nodes in "Y" direction [#].
  size_t                   nodes              = nodes_x*nodes_y;                                    // Total # of nodes [#].
  float                    dx                 = (x_max - x_min)/(nodes_x - 1);                      // x-axis mesh spatial size [m].
  float                    dy                 = (y_max - y_min)/(nodes_y - 1);                      // y-axis mesh spatial size [m].
  float                    dz                 = dx;                                                 // z-axis mesh spatial size [m].
  size_t                   i;                                                                       // "x" direction index [#].
  size_t                   j;                                                                       // "y" direction index [#].

  // GUI PARAMETERS (orbit):
  float                    orbit_x_init       = 0.0f;                                               // x-axis orbit initial rotation.
  float                    orbit_y_init       = 0.0f;                                               // y-axis orbit initial rotation.

  // GUI PARAMETERS (pan):
  float                    pan_x_init         = 0.0f;                                               // x-axis pan initial translation.
  float                    pan_y_init         = 0.0f;                                               // y-axis pan initial translation.
  float                    pan_z_init         = -2.0f;                                              // z-axis pan initial translation.

  // GUI PARAMETERS (mouse):
  float                    mouse_orbit_rate   = 1.0;                                                // Orbit rotation rate [rev/s].
  float                    mouse_pan_rate     = 5.0;                                                // Pan translation rate [m/s].
  float                    mouse_decaytime    = 1.25;                                               // Pan LP filter decay time [s].

  // GUI PARAMETERS (gamepad):
  float                    gamepad_orbit_rate = 1.0;                                                // Orbit angular rate coefficient [rev/s].
  float                    gamepad_pan_rate   = 1.0;                                                // Pan translation rate [m/s].
  float                    gamepad_decaytime  = 1.25;                                               // Low pass filter decay time [s].
  float                    gamepad_deadzone   = 0.1;                                                // Gamepad joystick deadzone [0...1].

  // SIMULATION PARAMETERS:
  float                    h                  = 0.01;                                               // Cloth's thickness [m].
  float                    rho                = 1000.0;                                             // Cloth's mass density [kg/m^3].
  float                    E                  = 100000.0;                                           // Cloth's Young modulus [kg/(m*s^2)].
  float                    mu                 = 700.0;                                              // Cloth's viscosity [Pa*s].
  float                    m                  = rho*h*dx*dy;                                        // Cloth's mass [kg].
  float                    g                  = 9.81;                                               // External gravity field [m/s^2].
  float                    k                  = E*h*dy/dx;                                          // Cloth's elastic constant [kg/s^2].
  float                    C                  = mu*h*dx*dy;                                         // Cloth's damping [kg*s*m].
  float                    dt_critical        = sqrt (m/k);                                         // Critical time step [s].
  float                    dt_simulation      = 0.8* dt_critical;                                   // Simulation time step [s].

  // NEUTRINO:
  neutrino*                bas                = new neutrino ();                                    // Neutrino baseline.
  opengl*                  gui                = new opengl ();                                      // OpenGL context.
  opencl*                  ctx                = new opencl ();                                      // OpenCL context.
  shader*                  S                  = new shader ();                                      // OpenGL shader program.
  queue*                   Q                  = new queue ();                                       // OpenCL queue.
  kernel*                  K1                 = new kernel ();                                      // OpenCL kernel array.
  kernel*                  K2                 = new kernel ();                                      // OpenCL kernel array.
  size_t                   kernel_sx          = nodes;                                              // Kernel dimension "x" [#].
  size_t                   kernel_sy          = 0;                                                  // Kernel dimension "y" [#].
  size_t                   kernel_sz          = 0;                                                  // Kernel dimension "z" [#].

  // NODE COLOR:
  float4G*                 color              = new float4G ();                                     // Color [].

  // NODE KINEMATICS:
  float4G*                 position           = new float4G ();                                     // Position [m].
  float4*                  velocity           = new float4 ();                                      // Velocity [m/s].
  float4*                  acceleration       = new float4 ();                                      // Acceleration [m/s^2].

  // NODE KINEMATICS (INTERMEDIATE):
  float4*                  position_int       = new float4 ();                                      // Position (intermediate) [m].
  float4*                  velocity_int       = new float4 ();                                      // Velocity (intermediate) [m/s].
  float4*                  acceleration_int   = new float4 ();                                      // Acceleration (intermediate) [m/s^2].

  // NODE DYNAMICS:
  float4*                  gravity            = new float4 ();                                      // Gravity [m/s^2].
  float4*                  stiffness          = new float4 ();                                      // Stiffness.
  float4*                  resting            = new float4 ();                                      // Resting.
  float4*                  friction           = new float4 ();                                      // Friction.
  float4*                  mass               = new float4 ();                                      // Mass [kg].

  // MESH:
  mesh*                    object             = new mesh ();                                        // Mesh object.
  size_t                   nodes;                                                                   // Number of nodes.
  size_t                   elements;                                                                // Number of elements.
  size_t                   neighbours;                                                              // Number of neighbours.
  size_t                   border_nodes;                                                            // Number of border nodes.
  std::vector<size_t>      neighbour_cell;                                                          // Neighbours.
  std::vector<size_t>      border;                                                                  // Border nodes.

  // SIMULATION TIME:
  float1*                  dt                 = new float1 ();                                      // Time step [s].
  float                    simulation_time;                                                         // Simulation time [s].
  int                      time_step_index;                                                         // Time step index [#].

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////// DATA INITIALIZATION //////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  object->init (bas, std::string (GMSH_HOME) + std::string (GMSH_MESH));                            // Initializing object mesh...
  nodes    = object->node.size ();                                                                  // Getting number of nodes...
  elements = object->element.size ();                                                               // Getting number of elements...

  color->init (nodes);                                                                              // Initializing color data...

  position->init (nodes);                                                                           // Initializing position data...
  velocity->init (nodes);                                                                           // Initializing velocity data...
  acceleration->init (nodes);                                                                       // Initializing acceleration data...

  position_int->init (nodes);                                                                       // Initializing intermediate position data...
  velocity_int->init (nodes);                                                                       // Initializing intermediate position data...
  acceleration_int->init (nodes);                                                                   // Initializing intermediate position data...

  gravity->init (nodes);                                                                            // Initializing gravity data...
  stiffness->init (nodes);                                                                          // Initializing stiffness data...
  resting->init (nodes);                                                                            // Initializing resiting position data...
  friction->init (nodes);                                                                           // Initializing friction data...
  mass->init (nodes);                                                                               // Initializing mass data...

  dt->init (nodes);                                                                                 // Initializing time step data [s]...

  simulation_time = 0.0;                                                                            // Initializing simulation time [s]...
  time_step_index = 0;                                                                              // Initializing time step index [#]...

  std::cout << "Critical time step = " << dt_critical << "[s]" << std::endl;
  std::cout << "Simulation time step = " << dt_simulation << "[s]" << std::endl;

  neighbours      = 0;

  for(i = 0; i < nodes; i++)
  {
    neighbour_cell          = object->neighbours (i);                                               // Getting neighbour cell...
    neighbours             += neighbour_cell.size ();                                               // Accumulating number of neighbours...

    color->data[i].x        = 0.01f*(rand () % 100);                                                // Setting "r" color coordinate...
    color->data[i].y        = 0.01f*(rand () % 100);                                                // Setting "g" color coordinate...
    color->data[i].z        = 0.01f*(rand () % 100);                                                // Setting "b" color coordinate...
    color->data[i].w        = 1.0f;                                                                 // Setting "a" color coordinate...

    position->data[i].x     = object->node[i].x;                                                    // Setting "x" position...
    position->data[i].y     = object->node[i].y;                                                    // Setting "y" position...
    position->data[i].z     = object->node[i].z;                                                    // Setting "z" position...
    position->data[i].w     = object->node[i].w;                                                    // Setting "w" position...

    velocity->data[i].x     = 0.0f;                                                                 // Setting "x" velocity...
    velocity->data[i].y     = 0.0f;                                                                 // Setting "y" velocity...
    velocity->data[i].z     = 0.0f;                                                                 // Setting "z" velocity...
    velocity->data[i].w     = 1.0f;                                                                 // Setting "w" velocity...

    acceleration->data[i].x = 0.0f;                                                                 // Setting "x" acceleration...
    acceleration->data[i].y = 0.0f;                                                                 // Setting "y" acceleration...
    acceleration->data[i].z = 0.0f;                                                                 // Setting "z" acceleration...
    acceleration->data[i].w = 1.0f;                                                                 // Setting "w" acceleration...

    gravity->data[i].x      = 0.0f;                                                                 // Setting "x" gravity...
    gravity->data[i].y      = 0.0f;                                                                 // Setting "y" gravity...
    gravity->data[i].z      = -g;                                                                   // Setting "z" gravity...
    gravity->data[i].w      = 1.0f;                                                                 // Setting "w" gravity...

    stiffness->data[i].x    = k;                                                                    // Setting "x" stiffness...
    stiffness->data[i].y    = k;                                                                    // Setting "y" stiffness...
    stiffness->data[i].z    = k;                                                                    // Setting "z" stiffness...
    stiffness->data[i].w    = 1.0f;                                                                 // Setting "w" stiffness...

    resting->data[i].x      = dx;                                                                   // Setting "x" resting position...
    resting->data[i].y      = dy;                                                                   // Setting "y" resting position...
    resting->data[i].z      = dz;                                                                   // Setting "z" resting position...
    resting->data[i].w      = 1.0f;                                                                 // Setting "w" resting position...

    friction->data[i].x     = C;                                                                    // Setting "x" friction...
    friction->data[i].y     = C;                                                                    // Setting "y" friction...
    friction->data[i].z     = C;                                                                    // Setting "z" friction...
    friction->data[i].w     = 1.0f;                                                                 // Setting "w" friction...

    mass->data[i].x         = m;                                                                    // Setting "x" mass...
    mass->data[i].y         = m;                                                                    // Setting "y" mass...
    mass->data[i].z         = m;                                                                    // Setting "z" mass...
    mass->data[i].w         = 1.0f;                                                                 // Setting "w" mass...

    freedom->data[i].x      = 1.0f;                                                                 // Setting "x" freedom...
    freedom->data[i].y      = 1.0f;                                                                 // Setting "y" freedom...
    freedom->data[i].z      = 1.0f;                                                                 // Setting "z" freedom...
    freedom->data[i].w      = 1.0f;                                                                 // Setting "w" freedom...

    dt->data[i]             = dt_simulation;                                                        // Setting time step...
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////// NEUTRINO INITIALIZATION /////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  bas->init (QUEUE_NUM, KERNEL_NUM);                                                                // Initializing Neutrino baseline...
  gui->init
  (
   bas,                                                                                             // Neutrino baseline.
   GUI_SIZE_X,                                                                                      // GUI x-size [px].
   GUI_SIZE_Y,                                                                                      // GUI y-size [px.]
   GUI_NAME,                                                                                        // GUI name.
   orbit_x_init,                                                                                    // Initial x-orbit.
   orbit_y_init,                                                                                    // Initial y-orbit.
   pan_x_init,                                                                                      // Initial x-pan.
   pan_y_init,                                                                                      // Initial y-pan.
   pan_z_init                                                                                       // Initial z-pan.
  );
  ctx->init (bas, gui, NU_GPU);                                                                     // Initializing OpenCL context...
  S->init (bas, SHADER_HOME, SHADER_VERT, SHADER_GEOM, SHADER_FRAG);                                // Initializing OpenGL shader...
  Q->init (bas);                                                                                    // Initializing OpenCL queue...
  kernel_home = KERNEL_HOME;                                                                        // Setting kernel home directory...
  kernel_1.push_back ("utilities.cl");                                                              // Setting 1st source file...
  kernel_1.push_back ("thekernel1.cl");                                                             // Setting 2nd source file...
  K1->init (bas, kernel_home, kernel_1, kernel_sx, kernel_sy, kernel_sz);                           // Initializing OpenCL kernel K1...
  kernel_2.push_back ("utilities.cl");                                                              // Setting 1st source file...
  kernel_2.push_back ("thekernel2.cl");                                                             // Setting 2nd source file...
  K2->init (bas, kernel_home, kernel_2, kernel_sx, kernel_sy, kernel_sz);                           // Initializing OpenCL kernel K2...

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////// SETTING OPENCL KERNEL ARGUMENTS /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  K1->setarg (color, 0);                                                                            // Setting color kernel argument...
  K1->setarg (position, 1);                                                                         // Setting position kernel argument...
  K1->setarg (position_int, 2);                                                                     // Setting intermediate position kernel argument...
  K1->setarg (velocity, 3);                                                                         // Setting velocity kernel argument...
  K1->setarg (velocity_int, 4);                                                                     // Setting intermediate velocity kernel argument...
  K1->setarg (acceleration, 5);                                                                     // Setting acceleration kernel argument...
  K1->setarg (acceleration_int, 6);                                                                 // Setting intermediate acceleration kernel argument...
  K1->setarg (gravity, 7);                                                                          // Setting gravity kernel argument...
  K1->setarg (stiffness, 8);                                                                        // Setting stiffness kernel argument...
  K1->setarg (resting, 9);                                                                          // Setting resting position kernel argument...
  K1->setarg (friction, 10);                                                                        // Setting friction kernel argument...
  K1->setarg (mass, 11);                                                                            // Setting mass kernel argument...
  K1->setarg (freedom, 12);                                                                         // Setting freedom flag kernel argument...
  K1->setarg (dt, 13);                                                                              // Setting time step kernel argument...

  K2->setarg (color, 0);                                                                            // Setting color kernel argument...
  K2->setarg (position, 1);                                                                         // Setting position kernel argument...
  K2->setarg (position_int, 2);                                                                     // Setting intermediate position kernel argument...
  K2->setarg (velocity, 3);                                                                         // Setting velocity kernel argument...
  K2->setarg (velocity_int, 4);                                                                     // Setting intermediate velocity kernel argument...
  K2->setarg (acceleration, 5);                                                                     // Setting acceleration kernel argument...
  K2->setarg (acceleration_int, 6);                                                                 // Setting intermediate acceleration kernel argument...
  K2->setarg (gravity, 7);                                                                          // Setting gravity kernel argument...
  K2->setarg (stiffness, 8);                                                                        // Setting stiffness kernel argument...
  K2->setarg (resting, 9);                                                                          // Setting resting position kernel argument...
  K2->setarg (friction, 10);                                                                        // Setting friction kernel argument...
  K2->setarg (mass, 11);                                                                            // Setting mass kernel argument...
  K2->setarg (freedom, 12);                                                                         // Setting freedom flag kernel argument...
  K2->setarg (dt, 13);                                                                              // Setting time step kernel argument...

  color->name    = "voxel_color";                                                                   // Setting variable name for OpenGL shader...
  position->name = "voxel_center";                                                                  // Setting variable name for OpenGL shader...

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////// WRITING DATA ON OPENCL QUEUE //////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  Q->write (color, 0);                                                                              // Writing color data on queue...
  Q->write (position, 1);                                                                           // Writing position data on queue...
  Q->write (position_int, 2);                                                                       // Writing intermediate position data on queue...
  Q->write (velocity, 3);                                                                           // Writing velocity data on queue...
  Q->write (velocity_int, 4);                                                                       // Writing intermediate velocity data on queue...
  Q->write (acceleration, 5);                                                                       // Writing acceleration data on queue...
  Q->write (acceleration_int, 6);                                                                   // Writing intermediate acceleration data on queue...
  Q->write (gravity, 7);                                                                            // Writing gravity data on queue...
  Q->write (stiffness, 8);                                                                          // Writing stiffness data on queue...
  Q->write (resting, 9);                                                                            // Writing resting position data on queue...
  Q->write (friction, 10);                                                                          // Writing friction data on queue...
  Q->write (mass, 11);                                                                              // Writing mass data on queue...
  Q->write (index_R, 12);                                                                           // Writing right neighbour index on queue...
  Q->write (index_U, 13);                                                                           // Writing up neighbour index on queue...
  Q->write (index_L, 14);                                                                           // Writing left neighbour index on queue...
  Q->write (index_D, 15);                                                                           // Writing down neighbour index on queue...
  Q->write (freedom, 16);                                                                           // Writing freedom flag data on queue...
  Q->write (dt, 17);                                                                                // Writing time step data on queue...

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////// SETTING OPENGL SHADER ARGUMENTS ////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  S->setarg (color, 0);                                                                             // Setting shader argument "0"...
  S->setarg (position, 1);                                                                          // Setting shader argument "1"...
  S->build ();                                                                                      // Building shader program...

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////// APPLICATION LOOP ////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  while(!gui->closed ())                                                                            // Opening window...
  {
    bas->get_tic ();                                                                                // Getting "tic" [us]...

    gui->clear ();                                                                                  // Clearing gui...
    gui->poll_events ();                                                                            // Polling gui events...

    Q->acquire (color, 0);                                                                          // Acquiring OpenGL/CL shared argument...
    Q->acquire (position, 1);                                                                       // Acquiring OpenGL/CL shared argument...
    ctx->execute (K1, Q, NU_WAIT);                                                                  // Executing OpenCL kernel...
    ctx->execute (K2, Q, NU_WAIT);                                                                  // Executing OpenCL kernel...
    Q->release (color, 0);                                                                          // Releasing OpenGL/CL shared argument...
    Q->release (position, 1);                                                                       // Releasing OpenGL/CL shared argument...

    gui->mouse_navigation (
                           mouse_orbit_rate,                                                        // Orbit angular rate coefficient [rev/s].
                           mouse_pan_rate,                                                          // Pan translation rate [m/s].
                           mouse_decaytime                                                          // Orbit low pass decay time [s].
                          );

    gui->gamepad_navigation (
                             gamepad_orbit_rate,                                                    // Orbit angular rate coefficient [rev/s].
                             gamepad_pan_rate,                                                      // Pan translation rate [m/s].
                             gamepad_decaytime,                                                     // Low pass filter decay time [s].
                             gamepad_deadzone                                                       // Gamepad joystick deadzone [0...1].
                            );

    if(gui->button_CROSS)
    {
      gui->close ();                                                                                // Closing gui...
    }

    gui->plot (S);                                                                                  // Plotting shared arguments...

    gui->refresh ();                                                                                // Refreshing gui...

    simulation_time += dt_simulation;                                                               // Updating simulation time [s]...
    time_step_index++;                                                                              // Updating time step index [#]...

    bas->get_toc ();                                                                                // Getting "toc" [us]...
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////// CLEANUP ////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  delete bas;                                                                                       // Deleting Neutrino baseline...
  delete gui;                                                                                       // Deleting OpenGL gui...
  delete ctx;                                                                                       // Deleting OpenCL context...

  delete color;                                                                                     // Deleting color data...
  delete position;                                                                                  // Deleting position data...
  delete velocity;                                                                                  // Deleting velocity data...
  delete acceleration;                                                                              // Deleting acceleration data...

  delete position_int;                                                                              // Deleting intermediate position data...
  delete velocity_int;                                                                              // Deleting intermediate velocity data...
  delete acceleration_int;                                                                          // Deleting intermediate acceleration data...

  delete gravity;                                                                                   // Deleting gravity data...
  delete stiffness;                                                                                 // Deleting stiffness data...
  delete resting;                                                                                   // Deleting resting data...
  delete friction;                                                                                  // Deleting friction data...
  delete mass;                                                                                      // Deleting mass data...

  delete freedom;                                                                                   // Deleting freedom flag data...
  delete dt;                                                                                        // Deleting time step data...

  delete Q;                                                                                         // Deleting OpenCL queue...
  delete K1;                                                                                        // Deleting OpenCL kernel...
  delete K2;                                                                                        // Deleting OpenCL kernel...

  return 0;
}
