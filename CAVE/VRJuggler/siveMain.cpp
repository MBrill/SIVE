/* -------------------------------------------------------------------
 *    Dateiname: siveMain.cpp
 * -------------------------------------------------------------------*/
#include "SiveEngine.h"

int main(int argv, char **argc)
{
    vlgGraphicsEngine *app = SiveEngine::Instance();

    app->initApplication(argv, argc, 1280, 960, 50, 50, 
		         GLUT_DOUBLE | GLUT_RGBA);

    app->run();

    return 0;
}
