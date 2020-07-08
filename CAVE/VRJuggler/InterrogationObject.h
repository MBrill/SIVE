// --------------------------------------------------------------------
//  Klasse für das Darstellen und Handeln des untersuchten
//  Objekts.
// --------------------------------------------------------------------
//  $RCSfile$
//  $Revision$
//  $Date$
// --------------------------------------------------------------------
#ifndef INTERROGATIONOBJECT
#define INTERROGATIONOBJECT

#include "vlgGetVTKPolyData.h"
#include "vlgTextureMap2D.h"
#include <vtkPolyData.h>

//! Klasse für das Darstellen und Handeln des untersuchten geometrischen Objekts
class InterrogationObject : public vlgGetVTKPolyData
{
public:
     //! Default-Konstructor
     InterrogationObject(void);
     //! Kopierkonstructor
     InterrogationObject(const InterrogationObject&);
     //! Konstruktor, der ein VTK-File einliest
     /*!
       Der Konstruktor verwendet ein polygonales Netz, das aus
       einem VTK-File eingelesen wird. Falls die bool'sche
       Variable auf true gesetzt ist werden die
       Linine als Textur auf dem Objekt dargestellt.
     
       Die Farbe für das Objekt wird auf rot gesetzt.
     */
     InterrogationObject(char*, bool);
     //! Constructor based on a vtk file
     /*!
       The Contructor reads the polygonal data in VTK format. No texture map
       is used for the rendering of the interrogation lines.
       
       The render color of the object is set to red.
     */
     InterrogationObject(char*);
     
     //! Read the polygonal data from a file in VTK format 
     void readObject(char*);
     
     //! Das Objekt als Instanz von vtkPolyData zurückgeben
     vtkPolyData* getVTKData(void);

     //! Query the bounding box of the interrogation object
     /*!
            The bounding box is given as (xmin, xmax, ymin, ymax, zmin, zmax).
     */
     float* getBoundingBox(void);
     //! Query the bounding box of the interrogation object
     /*!
       The bounding box is given as (xmin, xmax, ymin, ymax, zmin, zmax).
     */
     void   getBoundingBox(float b[6]);
     
     //! Query the center point of the bounding box
     float* getCenter(void);
     //! Query the center point of the bounding box
     void   getCenter(float c[3]);
     
     //! Determine, if the interrogation lines should be rendered with texture mapping
     inline void setTextureState(bool t) {textured = t;};
     
     //! Set the color of the interrogated object
     void setColor(float c[3]);
     //! Set the color of the interrogated object
     void setColor(float r, float g, float b);
     
     //! Query the render color 
     float* getColor(void);
     //! Query the render color 
     void getColor(float c[3]);
     
     //! set the polygonal data to vtkPolyData
     void setObject(vtkPolyData *o);
     
     
private:
     //! Eine achsen-orientierte Bounding-Box
     float* bbox;
     //! Die Farbe des Objekts (Default: rot)
     GLfloat color[3];           
     //! Textured or not textured - Hamlet?
     bool textured;
     //! Instanz einer 2D Textur für die Ausgabe als Textur
     vlgTextureMap2D *texture;
};
#endif
