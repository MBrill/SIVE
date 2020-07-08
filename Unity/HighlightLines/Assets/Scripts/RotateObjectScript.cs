using UnityEngine;
using System.Collections;

/*! 
 * \brief A simple script to rotate an object around its y-axis through C# script.
 * \ingroup HighlightLines
 * \author Sascha Hayton
 * \date 2014 2015
 */ 

public class RotateObjectScript : MonoBehaviour
{
		// Update is called once per frame
		void Update ()
		{
			// If e on the keyboard is pressed
			if(Input.GetKey(KeyCode.E))
			{
				transform.Rotate(Vector3.up * Time.deltaTime * 50);
			}
			// If q on the keyboard is pressed
			if(Input.GetKey(KeyCode.Q))
			{
				transform.Rotate(Vector3.up * -Time.deltaTime * 50);
			}
		}
}
