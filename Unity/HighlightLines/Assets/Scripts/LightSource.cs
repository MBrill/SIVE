using UnityEngine;
using System.Collections;
/*! 
 * \brief This Script turns a plane into the Light Source
 * \ingroup HighlightLines
 * \author Sascha Hayton
 * \date 2014 2015
 */ 
public class LightSource : MonoBehaviour {

	[Header("Light Source")]
	public Vector3 LightPosition = new Vector3(0, 10, 0);
	public int LineCount = 1;
	public float DistanceBetweenLines = 2.5f;
	public float LineWidth = 0.1f;
	public float LineLenght = 10.0f;
	public Color LineColor = Color.white;


	[Header("Debug Options")]
	public bool DrawLines = true;

	GameObject[] lines;

	public GameObject[] Lines {
		get {
			return lines;
		}
	}

	void Start()
	{
		gameObject.transform.position = LightPosition;
		lines =  new GameObject[LineCount];
		for( int i = 0; i < lines.Length; i++)
		{

			lines[i] = GameObject.CreatePrimitive(PrimitiveType.Quad);
			lines[i].name = "Light_" + i;
			lines[i].transform.parent = gameObject.transform;
			lines[i].transform.Rotate( new Vector3( -90, 180  ,180) );
			MeshRenderer lr = lines[i].GetComponent<MeshRenderer>();
			lr.material.color = LineColor;
			lr.material = new Material(Shader.Find("Self-Illumin/Diffuse"));

			
				if( i == 0)
				{
					lr.transform.position = LightPosition;
					lr.transform.localScale = new Vector3(LineWidth , LineLenght ,1);

				}
				else
				{
					float newX;

        			if(i%2==1)
						newX = lines[i-1].transform.position.x + i * DistanceBetweenLines;
					else
						newX = lines[i-1].transform.position.x - i * DistanceBetweenLines;
        
          			lr.transform.position = new Vector3( newX, LightPosition.y, LightPosition.z);
					lr.transform.localScale = new Vector3(LineWidth ,LineLenght, 1);
      		}
         
    	}
	}

	//
	void Update()
	{
		if(Input.GetKeyDown(KeyCode.L))
			DrawLines = !DrawLines;
		for(int i = 0;i<lines.Length;i++)
			lines[i].renderer.enabled = DrawLines;
	}
}
