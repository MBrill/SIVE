// --------------------------------------------------------------------
//  Implementierung
// --------------------------------------------------------------------
//  $RCSfile$
//  $Revision$
//  $Date$
// --------------------------------------------------------------------
//#include <GL/gl.h>

#include <vtkPolyDataReader.h>
#include <vtkPolyData.h>
//#include <vtkDataArray.h> // für die TCoords

#include "InterrogationObject.h"
#include "vlgTexturemap2D.h"
#include "Vector3.h"

// Constructors
InterrogationObject::InterrogationObject(void) : vlgGetVTKPolyData()
{
	//verboseOn();

	color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
	textured = false;
	bbox = new float[6];
}

InterrogationObject::InterrogationObject(const InterrogationObject& copy)
{
	color[0] = copy.color[0];
	color[1] = copy.color[1];
	color[2] = copy.color[2];
	textured = copy.textured;
	bbox = new float[6];
}

InterrogationObject::InterrogationObject(char *fileName) : vlgGetVTKPolyData()
{
	bbox = new float[6];
	data = vtkPolyData::New();
	color[0] = 1.0f; 
	color[1]= 0.0f; 
	color[2] = 0.0f;
	textured = false;
	readObject(fileName);
}

InterrogationObject::InterrogationObject(char *fileName, bool tex) : vlgGetVTKPolyData()
{
	bbox = new float[6];
	data = vtkPolyData::New();
	color[0] = 1.0f; 
	color[1]= 0.0f; 
	color[2] = 0.0f;
	textured = tex;
	readObject(fileName);
}

void InterrogationObject::readObject(char *fileName)
{
        // ----- Die VTK-Pipeline  --------------------------
	vtkPolyDataReader *reader = vtkPolyDataReader::New();
	reader->SetFileName(fileName);
        // ----- Die VTK-Pipeline  --------------------------
	doAttributes();
	doPointData();
	noLines();
	doTriangles();
	doTriangleStrips();
	doQuads();
	doPolygons();
	//verboseOn();

	setData(reader->GetOutput());
	processData();

	// Die Bounding-Box von VTK berechnen lassen
	reader->GetOutput()->ComputeBounds();
	double b[6]; 
	reader->GetOutput()->GetBounds(b);
	for (int i=0; i<6; i++)
		bbox[i] = static_cast<float>(b[i]);
}

// Das Objekt als Instanz von vtkPolyData zurückgeben
vtkPolyData* InterrogationObject::getVTKData(void)
{
	return data;
}

float* InterrogationObject::getBoundingBox(void)
{
       return bbox;
}

void   InterrogationObject::getBoundingBox(float box[6])
{
   int i;
   for (i=0; i<6; i++)
	   box[i] = bbox[i];
}

float* InterrogationObject::getCenter(void)
{
   float *c = new float[3]; 
   c[0] = 0.5f * (bbox[1]+bbox[0]);
   c[1] = 0.5f * (bbox[3]+bbox[2]);
   c[2] = 0.5f * (bbox[5]+bbox[4]);

   return c;
}

void InterrogationObject::getCenter(float c[3])
{
   c[0] = 0.5f * (bbox[1]+bbox[0]);
   c[1] = 0.5f * (bbox[3]+bbox[2]);
   c[2] = 0.5f * (bbox[5]+bbox[4]);
}

void InterrogationObject::setColor(float c[3])
{
    color[0] = c[0];
    color[1] = c[1];
    color[2] = c[2];
}

void InterrogationObject::setColor(float r, float g, float b)
{
    color[0]=r;
    color[1]=g;
    color[2]=b;
}

float* InterrogationObject::getColor(void)
{
    float *c = new float[3];
    c[0] = color[0];
    c[1] = color[1];
    c[2] = color[2];
    return c;
}

void InterrogationObject::getColor(float c[3])
{
    c[0] = color[0];
    c[1] = color[1];
    c[2] = color[2];
}

void InterrogationObject::setObject(vtkPolyData *o)
{
	setData(o);
	processData();
	setPointer();
	// Die Bounding-Box von VTK berechnen lassen
	double b[6]; 
	o->ComputeBounds();
	o->GetBounds(b);
	for (int i=0; i<6; i++)
		bbox[i] = static_cast<float>(b[i]);
}
