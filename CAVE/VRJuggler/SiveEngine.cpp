// --------------------------------------------------------------------
//  SiveEngine.cpp
// --------------------------------------------------------------------
#include "SiveEngine.h"
#include "vlgPerspective.h"
#include "vlgWASD.h"

#include "vtkPolyDataReader.h"

#include "TopParallelLightCage.h"
#include <iostream>
using namespace std;

//  Konstruktor
SiveEngine::SiveEngine(void) 
{
	object = new InterrogationObject;
	//object->verboseOn();
}

// OpenGL und Anwendungs-Initialisierung 
void SiveEngine::init(void) 
{
	int i;
        //camera->set(4.0f, 0.0f, 0.0f);
	setWindowTitle("SIVE/GL");
	//useLightedAssets();

	// Zentralprojektion erzeugen mit Öffnungswinkel von 120 Grad.
	// Ziel: möglichst nahe am menschlichen Auge, um Fish-Tank
	// und Immersion zu realisieren. Wichtig ist auch die Höhenangabe
	// für das Auge in der WASD-Instanz!
	vlgPerspective *eye = new vlgPerspective(60.0f);
	setViewProjection(eye);
        vlgWASD *wasd = new vlgWASD;
	// Setzen der Position mit Hilfe von kartesischen Koordinaten
	wasd->setPosition(0.0f, 1.8f, 5.5f);
	wasd->setDirection(0.0f, 0.0f);
	wasd->setSpeed(0.05f);
	//setViewControl(wasd);

	useLightedAssets();
	axis->show();
	grid->setColor(0.5f, 0.5f, 0.0f);
	//grid->noShow();
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

        /* Color Tracking anschalten               */
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        /* Ambiente Lichtfarbe setzen                  */
        GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
        /* Diffuse Lichtfarbe setzen                   */
        GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
        /* Spekulare Lichtfarbe setzen                 */
        GLfloat specularLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
    
        // Eine Lichtquelle; als Headlight realisieren.
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, specularLight);
        glEnable(GL_LIGHT0);
       
	about();
	cout << "Einlesen des Objekts und Übergabe an OpenGL" << endl;

	// Objekt einlesen
	object->readObject("Data/G1_transformed.vtk");
	//object->readObject("Data/G2.vtk");
	//object->readObject("Data/fohe.vtk");
	//object->readObject("Data/fineMesh.vtk");
	
	cout << "Das untersuchte Objekt ist eingelesen und vorbereitet" << endl;

	cout << "Setzen des Lichtkäfigs" << endl;
	float box[6] = {-2.0f, 2.0f, 0.5f, 4.0f, -2.0f, 2.0f};
	TopParallelLightCage localCage(box, 10);
    
	localCage.setColor(1.0f, 1.0f, 1.0f);
	cagelist = localCage.createList();

	LightVector *dir = new LightVector(0.0f, 0.0f, 1.0f);

	float *boundingbox = new float[6];
	boundingbox = object->getBoundingBox();
	float delta = 0.5f*(boundingbox[3]-boundingbox[2]);
        float w[3];
	w[0] = 0.5f*(boundingbox[0]+boundingbox[1]);
	w[1] = boundingbox[3] + 0.1f*delta;
	w[2] = 0.5f*(boundingbox[4]+boundingbox[5]);

	dir->setRenderOrigin(w);
	dir->setLength(2.0f*delta);
	dir->setColor(1.0f, 1.0f, 0.0f);
	vectorlist = dir->getLine();

	cout << "Der Lichtkäfig ist gesetzt" << endl;
	cout << endl;

	cout << "Die Isophoten werden initialisiert" << endl;
	isophotes = new Isophotes(object, dir);
	cout << "Die Isophoten werden berechnet" << endl;
			

	isophotes->doAttributes();
	isophotes->doPointData();
	isophotes->doLines();
	isophotes->processData();
	isophotes->setPointer();

	isophotes->compute();
	cout << "Initialisierung abgeschlossen" << endl;
}
	

// Funktion mit Applikationsanweisungen für die grafische Ausgabe
void SiveEngine::display(void)
{
	glEnable(GL_LIGHTING);
        GLfloat light0Pos[] = {2.0f, 6.0f, 2.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);

        glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
	    glColor3fv(object->getColor());
            object->setPointerAndDraw();
	    
 	    glDisable(GL_LIGHTING);
	    glLineWidth(4.0f);
	    // Für Isophoten wird der Lichtvektor dargestellt
	    glCallList(vectorlist);
            //glCallList(cagelist);

	    glLineWidth(5.0f);
	    glColor3f(1.0f, 1.0f, 1.0f);
	    isophotes->setPointerAndDraw();

	    glEnable(GL_LIGHTING);
	glPopMatrix();
}

// Tastatur-Shortcuts
void SiveEngine::keyboard(unsigned char key, int x, int y)
{
	/*Vector3 plusX(0.1f, 0.0f, 0.0f), minusX(-0.1f, 0.0f, 0.0f),
		plusY(0.0f, 0.1f, 0.0f), minusY(0.0f, -0.1f, 0.0f),
		plusZ(0.0f, 0.0f, 0.1f), minusZ(0.0f, 0.0f, -0.1f);*/

	switch (key) {
		case 'U': //rake->rotateX(-M_PI*0.05);
			  glutPostRedisplay();
			  break;
    }
}

void SiveEngine::about()
{
	cout << "-----------------------------------------" << endl;
	cout << "-  Flächenqualitätsuntersuchung         -" << endl;
	cout << "-----------------------------------------" << endl;
	cout << "                                         " << endl;
	cout << "-----------------------------------------" << endl;
	cout << " Kamerasteuerung: Examine                " << endl;
	cout << "-----------------------------------------" << endl;
}


SiveEngine* SiveEngine::instance = 0;
// Gibt die  Instanz der Klasse zurück
// Muss überschrieben werden, um den richtigen Pointer zurückzugeben.
// Alternativen finden man in Gamma et. al. ...
SiveEngine* SiveEngine::Instance(void)
{
	if (instance == 0) {
		SiveEngine::instance = new SiveEngine;
		vlgGraphicsEngine::instance = SiveEngine::instance;
	}
	return instance;
}
