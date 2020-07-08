// ------------------------------------------------------------------
//  filename:  sive.C
// ------------------------------------------------------------------
//  $Revision: 1.3 $
//  $Date: 2002/06/11 22:07:03 $
// ------------------------------------------------------------------

// Doxygen comments for the main page and this file
/*! \mainpage SIVE - Surface Interrogation in Virtual Environments
  
  \section intro Introduction
  This toolkit implements classes for interactive surface interrogation
  in a virtual environment.

  \section software Software
  The toolkit was developed on IRIX 6.5. The SIVE toolkit depends on:
     - OpenGL Performer 2.4
     - VTK 3.1g
     - CAVELib 2.7
  \section contact Contact
  brill@erc.msstate.edu.
*/


/*! \file
  Main program for SIVE

  This file is the main program for the SIVE toolkit. Objects can be read,
  a light cage is build and a event loop is defined for the CAVElib.
*/
// get system headers
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Performer headers
#include <Performer/pf.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfSCS.h>
#include <Performer/pf/pfDCS.h>
// cave headers
#include "pfcave.h"
// my stuff
#include "HighlightLines.h"
#include "ReflectionLines.h"
#include "Isophotes.h"
#include "LightLine.h"
#include "LightCage.h"
#include "GeometryRoom.h"
#include "TexturedRoom.h"

// Prototypes of local functions
void doCmd(int argc, char *argv[],
           char carFile[], bool &geo, bool &texture,
           bool &horizontal, bool &vertical, bool &criss, 
           float &radius, LightLine::Attenuation &lform, 
           int &bmSize, bool &preFilter, int &numberOfLines, int &speed, 
           bool &carToggle, bool &rl, bool &hl, bool &il);

void myEventLoop(Room *room, int speed);

// main
//! main program
/*!
  The main application called in the COVE. 

  If called without command options, sive will start a session with the
  following default values:
    - no verbose,
    - load the vtk file fohe.vtk,
    - use a light cage with 1 vertical, centered light line on the
      top plane of the object, radius=0,
    - use highlight lines,
    - render the interrogation lines as geometry,
    - render the object,
    - trigger the re-display of interrogation lines manually after a change
      in the light cage or the object.

  In general, the call is
    sive [-v] [-h|-r|-i|-c|-p] [-X] [-g|-t] [-P] [-V|-H] [-n:#] [-I] [-b:#.#] [l:c] [s:####] [-f:file] [-O] [-o:file]

  The options are:
    - -v: verbose mode on; the settings are displayed before the interactive
      system begins to run;
    - -o:filename: Use the vtk file filename.vtk in the directory ../data
      as input geometry.
    - -h|r|i: mutually exclusive options to choose the kind of interrogation
      lines. h is highlight lines, r is reflection lines, i is isophotes,
    - -g|t: mutually exclusive. g renders geometry for the interrogation lines,
      t uses textures. The default size of a texture is 256x256 pixels for 2D
      texture maps and 256 pixels for 1D texture maps. This size can be changed
      by the option -s (see below). 

      If no radius is given in case of -t, a radius of 0.01 is used. If no radius
      is given in case of -g, a radius of 0.0 is used.
    - -P: for texturing, turn the prefiltering of the texture map on. Default is      no prefiltering.
    - -O: do not display the object. The display of the object can be toggled
      in the COVE using Wand button 3. Default is object show.
    - -I: turns on immediate redisplay of interrogation lines in case of a changed
      light cage or object. Can not be toggled interactively and should be
      used carefully. If the object contains more than 100.000 polygons do not
      use this option!
    - -n:#: number of lines in the cage. The default ist 1.
    - -H|V: use horizontal (parallel to the x-axis) or vertical (parallel
      to the y-axis) lines. Default is vertical.
    - -X: use two sets of light lines, one set parallel to the x-, one
      parallel to the y-axis. In that case, the number given with the -n option
      defines the number of lines in every direction. 
      For example, -X -n2 results in total 4 lines, 2 parallel to x-, 2 parallel to y.
    - -b:f: Radius of the light cylinders created. Should be a float value. 
      Default is 0.0. If texturing is turned on, the default is 0.01.

    - -l:a: Attenuation of the light cylinders. Four values for
      the letter a are implemented:
         -# c for Constant (the default)
         -# l for Linear 
         -# q for Quadratic
         -# p for Polynomial
      So, call it like sive -ll to get linear attenuation of light.
      See the class \link LightCage \endlink for details.
    - -s:i: Set the size of the bitmaps used for the texture maps. The default
      is i=256. Should be an integer value and a power of 2 (a restriction put
      by OpenGL Performer).

    Examples

    sive -v: Show all the settings on the console window. Use highlight lines,
             render these lines as geometry. You have to toggle the re-display of
             the highlight lines manually using wand button 2. Radius of the one
             centered vertical light line is 0.0.
    sive -r -t -I -P -X -n5 -b0.5:
             Use reflection lines and render them as a texture. If the object or
             the light cage is changed the new interrogation lines are displayed
             immediately. Use a light cage with 5 light cylinders parallel to the
             The texture map is prefiltered.
             x-axis and 5 parallel to the y-axis; set the radius of the
             cylinders to 0.5.

*/
int main( int argc, char *argv[] )
{
  //
  // filenames
  // only to load the right Performer readers!
  char carFile[] = "./fohe.vtk";
  int  speed, numberOfLines, bmSize;
  bool horizontal, vertical, criss, tex, geo,
       reflect, highlights, 
       isophotes, preFilter, carToggle;
  LightLine::Attenuation lform;
  float radius;

  // Set up the cave and Performer
  //
  pfInit();
  pfCAVEConfig(&argc, argv, NULL);
  pfConfig();
  pfCAVEInitChannels();

  //
  // Declare the CAVE and Performer objects
  //   Has to be done after pfInit() !
  //
  
  doCmd(argc, argv, carFile, geo, tex,
        horizontal, vertical, criss, radius, lform,
        bmSize, preFilter, numberOfLines, speed,
        carToggle, reflect, highlights, isophotes);
  // 
  // Ok, now we now, what to do.
  //
  InterrogationLines *interLines;
  if (highlights) 
        {
          interLines = new HighlightLines;
        }
  if (isophotes) 
        {
          interLines = new Isophotes;
          // we need special interaction functions in the Room
          // classes to make sure we transform the LightVector
          // and not the LightCage like for the others.
          if (speed==0) // normal interaction wanted
             speed = 3;
          else if (speed==1) // fast interaction wanted
             speed = 4;
        }
  if (reflect) 
        {
          interLines = new ReflectionLines;
          speed = 2;
        }
  if (preFilter) interLines->preFilterOn();

  Room *room;

  if (geo) room = new GeometryRoom(pfCAVEMasterChan(), carFile, interLines);

  // In case of textures, we set light band radius and bitmap size. Default values
  // are r=0.01f and size = 256. size has to be a power of 2.
  if (tex) 
     room = new TexturedRoom(pfCAVEMasterChan(), carFile, interLines, bmSize);

  if (!isophotes) {
     // Add a vertical, centered lightline, no computation.
     if (criss) 
        // crisscross ...
        room->addLightObjects(true, numberOfLines, radius, lform);
     else
        room->addLightObjects(true, vertical, numberOfLines, radius, lform); 
  }
  else 
     // we have to build no light cage, but a light vector, we are
     // using isophotes.
     room->addLightVector(numberOfLines);

  if (!carToggle) room->toggleInterrogationObject();

  // the event loop
  myEventLoop(room, speed);
  // the event loop

  CAVEHalt();
  pfExit();
  return 0;
}

//! Function to process the command line options
/*!
  There are severeal options to change the behaviour of the main
  program. The options and the default values are described
  in the main program.
*/
void doCmd(int argc, char *argv[], 
           char carFile[], bool &geo, bool &texture, 
           bool &horizontal, bool &vertical, bool &criss, 
           float &radius, LightLine::Attenuation &lform,
           int &bmsize, bool &preFilter, int &numberOfLines, int &speed, 
           bool &carToggle, 
           bool &rl, bool &hl, bool &il)
{
  // ---------------------------------------------------------------------
  // process the commandline arguments argc, argv
  // linesFile: filename, where the lightcage and interogation lines
  //            are stored.
  //            The suffix <<pfb>> has to be provided!
  //            files will be looked for in ../data
  //            -h and -r, -t and -g, -rl and -hl are mutually exclusive!
  //
  // command line options:
  //   -v        == verbose mode
  //   -h        == highlight lines (default)
  //   -r        == reflection lines
  //   -i        == isophotes
  //   -g        == render the interrogation lines using geometry
  //   -t        == render the interrogation lines using textures
  //                (default color is white).
  //   -P        == turn the prefiltering of the texture map on. Default is off.
  //   -O        == don't show the car 
  //   -I        == fast interaction, that means transforming
  //                and recomputing lines at once. Should only
  //                be used with small to medium datasets!
  //   -n:#      == number of lines in the cage (default:1)
  //   -H        == horizontal lines in the cage
  //   -V        == vertical lines in the cage (default)
  //   -X        == crisscross lines, that means vertical and horizontal lines.
  //   -b:radius == Radius of the lightband, important for texture mapping,
  //                default value is 0.0f, and 0.01f for the textured case.
  //   -l:f      == Attenuation of the lightbands. Four lightforms are 
  //                implemented: 
  //                        f=c  == LightLine::Constant
  //                        f=l  == LightLine::Linear
  //                        f=q  == LightLine::Quadratic
  //                        f=p  == LightLine::Polynomial
  //                The default is constant light.
  //   -s:#      == Size of the bitmap used for texture mapping. Should
  //                be a power of 2. Default is 256.
  //   -o:'file' == set input file for the car geometry.
  //                default is fohe.vtk.
  // ---------------------------------------------------------------------

  int  s;
  int form;
  // Variables containing the default values
  int  fast = 0, linesNumber = 1, size = 256;
  bool reflect=false, highl=true, 
       isophotes = false, vert=true, hori = false, pre = false;
  bool rflag = false, hflag = false, xflag = false,  
       iflag = false, errflg=false, 
       horiflag = false, vertflag = false, 
       texflag = false, geoflag = true, 
       texsetflag = false, geosetflag = false, 
       carflag = true, verboseflag = false;
  LightLine::Attenuation att = LightLine::Linear;
  float rad = 0.0f;
  char  *carname= "./fohe.vtk";
  // Variables containing the default values

  extern char *optarg;
  extern int optind;

  // process the cmdline with getopt
  while ((s = getopt(argc, argv, "POIvhcriptgo:n:HVXb:s:l:")) != -1)
      switch (s) {
        case 'v': verboseflag = true;
                  break;
        case 'I': fast = 1;
                  break;
        case 'n': linesNumber = atoi(optarg);
                  break;
        case 's': size = atoi(optarg);
                  break;
        case 'b': rad = atof(optarg);
                  break;
        case 'O': carflag = false;
                  break;
        case 'l': form = optarg[0];
                  switch (form) {
                     case 'c':     att = LightLine::Constant;
                              break;
                     case 'l':     att = LightLine::Linear;
                              break;
                     case 'q':     att = LightLine::Quadratic;
                              break;
                     case 'p':     att = LightLine::Polynomial;
                              break;
                  }
                  break;
        case 'h':
             if (iflag || rflag ) errflg = true;
             else
                 {
                  hflag = true;
                  highl = true;
                  reflect = false;
                  isophotes = false;
                 }
             break;
        case 'i':
             if (hflag || rflag ) errflg = true;
             else
                 {
                  iflag = true;
                  isophotes = true;
                  highl = false;
                  reflect = false;
                 }
             break;
        case 'r':
             if (iflag || hflag) errflg = true;
             else {
                   rflag = true;
                   reflect = true;
                   isophotes = false;
                   highl = false;
                  }
             break;
        case 't':
             if (geosetflag) errflg = true;
             else
                 {
                  texsetflag = true;
                  texflag = true;
                  geoflag = false;
                 }
             break;
        case 'g':
             if (texsetflag) errflg = true;
             else {
                   geosetflag = true;
                   geoflag = true;
                   texflag = false;
                  }
             break;
        case 'H':
             if (vertflag) errflg = true;
             else
                 {
                  hori = true;
                  vert = false;
                  horiflag = true;
                 }
             break;
        case 'V':
             if (horiflag) errflg = true;
             else
                 {
                  vertflag = true;
                  vert = true;
                  hori = false;
                 }
             break;
        case 'P': pre= true;
                  break;
        case 'X': xflag = true;
                  break;
        case 'o':
             carname  = strcat("./", optarg);
             break;
        case '?':
             errflg = true; // getopt returns ?, if the options
                            // are not registered above.
      }

  if (!errflg) {
     strcpy(carFile, carname);
     speed = fast;
     numberOfLines = linesNumber;
     radius = rad;
     bmsize = size;
     horizontal = hori;
     vertical = vert;
     criss = xflag;
     carToggle = carflag;
     hl  = highl;
     rl  = reflect;
     il  = isophotes;
     geo = geoflag;
     lform = att;
     texture = texflag;
     preFilter = pre;

     // If textured and radius is still 0.0f, change it to the default 0.01f
     if (texture && (radius == 0.0f)) radius = 0.01;

     if (verboseflag) {
          cout << endl;
          cout << "---------------------------------------------------------------" << endl;
          cout << "Options in use: " << endl;
          cout << "---------------------------------------------------------------" << endl;
          cout << "Interrogated geometry:        " << carFile << endl;
          switch (fast) {
             case 0: cout << "Trigger manually with button 2" << endl;
                     break;
             case 1: cout << "Interactive recomputation of lines" << endl;
                     break;
          }
          if (carToggle)
             cout << "Object is displayed" << endl;
          else 
             cout << "Use Wand Button 3 to toggle the object display" << endl;
          if (hl)
             cout << "Visualizing Highlight Lines ..." << endl;
          if (il)
             cout << "Visualizing Isophotes ..." << endl;
          if (rl)
             cout << "Visualizing Reflection Lines ..." << endl;
          if (geo)
             cout << " ... using geometry." << endl;
          if (texture) {
             cout << " ... using textures." << endl;
             if (preFilter)
                cout << "The texture map is prefiltered." << endl;
          }
          if (!il) {
            if (criss) 
             cout << "CrissCross Light Cage with " << 2*numberOfLines << " lines" 
                  << endl;
            else {
            if (vertical)
             cout << "Vertical Light Cage with " << numberOfLines << " lines" 
                  << endl;
            else if (horizontal)
             cout << "Horizontal Light Cage with " << numberOfLines << " lines" 
                  << endl;
            }
          }
          else
            cout << "Parallel light and isophotes with " << numberOfLines <<
                 " isophotes on the object" << endl;
          
          if (radius != 0.0f && !il)
          cout << "We use a light band with radius = " << radius << "." <<endl;

          if (!il)
          switch (lform) {
             case LightLine::Constant:   cout << "The attenuation of light is constant" << endl;
                                    break;
             case LightLine::Linear:     cout << "The attenuation of light is linear" << endl;
                                    break;
             case LightLine::Quadratic:  cout << "The attenuation of light is quadratic" << endl;
                                    break;
             case LightLine::Polynomial: cout << "The attenuation of light is polynomial" << endl;
                                    break;
          }
          if (texture)
          cout << "We use a texture map of size " << bmsize << "x" << bmsize << "." << endl;
          cout << "---------------------------------------------------------------" << endl;
          cout << "Wand Buttons" << endl;
          cout << "---------------------------------------------------------------" << endl;
          cout << "Use Wand Button 1 to toggle navigation and cage movement" << endl;
          cout << "Use Wand Button 2 to recompute the interrogation lines" << endl;
          cout << "Use Wand Button 3 to toggle the object display" << endl;
     }
  }
  else {
      cerr << "Usage: sive [-v] [-h|-r|-i|-c|-p] [-X] [-g|-t] [-V|-H] [-n:#] [-I] [-b:#.#] [l:c] [s:####] [-f:file] [-O] [-o:file]" 
           << endl;
      exit(2);
  }
}

//! The event loop for interactions using the CAVElib
/*!
  The Room::interact(), Room::fastInteract or
  the Room::reflectInteract() member function is used,
  based on the type of interrogation lines and user feedback 
  selected.

  For Isophotes we call Room::isophoteInteract() and 
  Romm::isophoteFastInteract(), because there we have to transform
  not the light cage, but a light vector.
*/
void myEventLoop(Room *room, int speed)
{
  switch (speed) {
     case 0: 
            while(!CAVEgetbutton(CAVE_ESCKEY)) {
               pfSync();
               pfCAVEPreFrame();
               pfFrame();
               pfCAVEPostFrame();
               // interact with the data
               room->interact();
            }
         break;
     case 1: 
            while(!CAVEgetbutton(CAVE_ESCKEY)) {
               pfSync();
               pfCAVEPreFrame();
               pfFrame();
               pfCAVEPostFrame();
               // interact with the data
               room->fastInteract();
            }
         break;
     case 2: 
            while(!CAVEgetbutton(CAVE_ESCKEY)) {
               pfSync();
               pfCAVEPreFrame();
               pfFrame();
               pfCAVEPostFrame();
               // interact with the data
               room->reflectInteract();
            }
         break;
     case 3: 
            while(!CAVEgetbutton(CAVE_ESCKEY)) {
               pfSync();
               pfCAVEPreFrame();
               pfFrame();
               pfCAVEPostFrame();
               // interact with the data
               room->isophoteInteract();
            }
         break;
     case 4: 
            while(!CAVEgetbutton(CAVE_ESCKEY)) {
               pfSync();
               pfCAVEPreFrame();
               pfFrame();
               pfCAVEPostFrame();
               // interact with the data
               room->isophoteFastInteract();
            }
         break;
   }
}
