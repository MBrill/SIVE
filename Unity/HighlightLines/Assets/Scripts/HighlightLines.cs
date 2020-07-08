using UnityEngine;
using System.Collections;

/*!
 * \defgroup HighlightLines 
 * \brief HighlightLines implementation in Unity.
 * \author Sascha Hayton
 * \date 2014-2015
 */

/*! 
 * \brief This script draws highlight lines on the object the script is attached to.
 * 
 * \ingroup HighlightLines
 * \author Sascha Hayton
 * \date 2014 2015
 */ 

public class HighlightLines : MonoBehaviour 
{

// public Member variables accessible from the inspector in the unity editor.

	[Header("Debug Functions")]
	//! Draw the vertex normals of the object.
	public bool DrawNormals;
	//! Draw the lines that intersect with the "light source".
	public bool DrawIntersection;
	//! Print the information of lines which intersect with the "light source".
	public bool LogIntersection;
	//! Print the duration of the calculcation and assignment of the calcAndSetUV Method.
	public bool LogCalculationTime;
	//! ÜPrint the new and old uv-coordinates for each vertex.
	[Header("Expensive")]
	public bool LogUVs;

// private variable accessible only from within this script.

	//! The MeshFilter of the object the script is attached to.
	MeshFilter objMeshFilter;
	//! The Vertices of the object.
	Vector3[] objVerts;
	//! The Normals of the object.
	Vector3[] objNormals;
	//! The UV-coordinates of the object.
	Vector2[] objUv;

	Bounds lightsource;

	Plane p;

	Texture2D tex ;
	// Use this for initialization
	void Start () 
    {
		objMeshFilter = gameObject.GetComponent<MeshFilter>();
		lightsource = GameObject.Find("light").GetComponent<Collider>().bounds;

		p = new Plane(Vector3.down,new Vector3(0,5,0));

		// cache the mesh data.
		objVerts = objMeshFilter.mesh.vertices;
		objNormals = objMeshFilter.mesh.normals;
		objUv = new Vector2[objMeshFilter.mesh.vertices.Length];

		for(int i = 0; i < objMeshFilter.mesh.uv.Length; i++)
		{
			objUv[i] = new Vector3(0,0);
		}

		objMeshFilter.mesh.uv = objUv;
		tex = Resources.LoadAssetAtPath<Texture2D>("Assets/StaticAssets/Textures/Top.jpg" ) ;



		Material mat0 = new Material(Shader.Find("Self-Illumin/Diffuse"));
		mat0.mainTexture = tex;
		mat0.color = Color.white;
		gameObject.renderer.materials = new Material[1]{mat0};
	}
	
	/*! 
	 * \brief Function to calculate and set the uv-coordinates
	 * \return void
	 */
	void calcAndSetUV()
	{		
		// A Ray for the intersection with the bounds of the "Light Source".
		Ray r = new Ray();

  		 // A timer to stop the duration of this method.
		float timer = Time.realtimeSinceStartup;     

		// The distance of the intersection point.
		float intersectionDistance;

		// intersection Points
	    float IPx = 0.0f;
	    float IPz = 0.0f;

		// tmeps
		float x = 0.0f;
		float y = 0.0f;

		int intersections = 0;

		updateNormalsAndVerts();

		for(int i = 0;i<objVerts.Length;i++)
		{
			r.origin = objVerts[i];
			r.direction = objNormals[i];
	
				if( p.Raycast(r,out intersectionDistance) )
				//if( lightsource.IntersectRay(r, out intersectionDistance) )
				{
					Vector3 ip = r.GetPoint(intersectionDistance);
					// formular from p.120 M. Brill, Virtuelle Realität, Auflage: 2009. Berlin; Heidelberg: Springer, 2008. 
					// x = Px + ((IPx - Pz) / Nz) * Nx
					// y = Py + ((IPx - Pz) / Nz) * Ny
					// This formular uses a different coord system. So we need to remember y is z.

			

					if(LogIntersection)
					{
						Debug.Log(" Vertex["+i+"]:" + objVerts[i] +" intersects the Light Source at " + IPx+","+IPz );
					}
						
					x = r.origin.x + ((ip.x - r.origin.y)/r.direction.y) * r.direction.x;
					y = r.origin.z + ((ip.z - r.origin.y)/r.direction.y) * r.direction.z;
										
					//[] -> [0,1]

					x = (x - lightsource.min.x) / (lightsource.max.x - lightsource.min.x);
					y = (y - lightsource.min.z) / (lightsource.max.z - lightsource.min.z);

					if(x > 1.0f) x = 1.0f;
					if(y > 1.0f) y = 1.0f;
					if(x < 0.0f) x = 0.0f;
					if(y < 0.0f) y = 0.0f;

					objUv[i].x = x;
					objUv[i].y = -y;
				

					if(DrawIntersection)
						Debug.DrawLine(objVerts[i], r.GetPoint(intersectionDistance), Color.green, 5,true );


					intersections++;
				}
				else
				{
					objUv[i].x = 0.0f;
					objUv[i].y = 0.0f;
					objMeshFilter.mesh.uv = objUv;
				}

				if(LogUVs)
					Debug.Log("OldUV["+i+"]: "+ objMeshFilter.mesh.uv[i]+" newUVs: "+objUv[i]);

				objMeshFilter.mesh.uv = objUv;
		}

		gameObject.renderer.materials[0].mainTexture = tex;
		timer = Time.realtimeSinceStartup - timer;	
	    if(LogCalculationTime)
		{
			Debug.Log("Done. The calculation for " + gameObject.name + " was done in "+ timer.ToString("0.000") +" secs. "+ intersections +" intersections were counted.");
		}
	}

	/*!
     * \brief Helper Coroutine. It calls the calcAndSetUV Method after a short delay. To make sure to print a Debug Message first.
     * This was also a try to use paralism. But it failt.
     * \param[in] waitTime Time in secs to wait until calcAndSetUV will be called.
     * \return IEnumerator A datatype to implement a Coroutine.
     */
	IEnumerator Wait(float waitTime) 
	{
		if(LogCalculationTime)
		{
			Debug.Log("Calculating "+ objVerts.Length +" intenrsections. For "+ gameObject.name +" Please wait.");
			yield return new WaitForSeconds(waitTime);
		}
		calcAndSetUV();
  	}

	/*!
     * \brief Helper method to apply the tranformation of the unity Inspector to the mesh.
     * \return void
     */
	void updateNormalsAndVerts()
	{
		for(int i = 0; i<objNormals.Length;i++)
		{
			objVerts[i] = transform.localToWorldMatrix.MultiplyPoint3x4(objMeshFilter.mesh.vertices[i]);
			objNormals[i] = transform.localToWorldMatrix.inverse.transpose.MultiplyPoint3x4(objMeshFilter.mesh.normals[i]).normalized;
		}
	}

	//! Update is called once per frame
	void Update () 
	{
		// On 'V' rcalculate the highlight lines.
		if(Input.GetKeyUp(KeyCode.V))
		{
			StartCoroutine("Wait", 0.4);
		}

		if(Input.GetKeyDown(KeyCode.N))
		{
			DrawNormals = !DrawNormals;
		}

		if(DrawNormals)
		{
			updateNormalsAndVerts();
			for(int i = 0; i<objMeshFilter.mesh.normals.Length;i++){
				Debug.DrawRay(objVerts[i], objNormals[i], Color.red, 0, true);
			}
				
		}
	}
}


