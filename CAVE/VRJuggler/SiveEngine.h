// --------------------------------------------------------------------
//  SiveEngine.h
// --------------------------------------------------------------------
#ifndef SIVEENGINE
#define SIVEENGINE

#include "vlgGraphicsEngine.h"
#include "vlgGetVTKPolyData.h"
#include "InterrogationObject.h"
#include "Isophotes.h"

//! SiveEngine - Anwendungsklasse f�r das SIVE-Projekt
class SiveEngine : public vlgGraphicsEngine
{
////
// public
////
public:
	//! Gibt die  Instanz der Klasse zur�ck
	// Muss �berschrieben werden, um den richtigen Pointer zur�ckzugeben.
	// Alternativen finden man in Gamma et. al. ...
	static SiveEngine* Instance(void);

	//! OpenGL und Anwendungs-Initialisierung 
	void init(void);
	
	//! Funktion mit Applikationsanweisungen f�r die grafische Ausgabe
	void display(void);
	
        //! Tastatur-Shortcuts
	void keyboard(unsigned char key, int x, int y);

	//! About
	void about(void);
////
// private
////
private:
	//! Lichtvektor f�r die Isophoten
	LightVector *dir;
	//! Display-List ID f�r den Lichtk�fig
	GLuint cagelist, vectorlist;
	//! Instanz des untersuchten Objekts
	InterrogationObject *object;
	//! Isophoten
	Isophotes *isophotes;
	//! Instanzvariable
	static SiveEngine* instance;
	//! Konstruktor
	SiveEngine(void);
};
#endif /* SIVEENGINE */
