using UnityEngine;

/*!
 * \brief Script for rotating an object along its y axis.
 * 
 * \ingroup DebugUtils
 * \author Patrick Schwartz
 * \date 2014-2015
 */

public class ObjectRotation : MonoBehaviour {

    //! How much degrees a single rotation rotates
    public float rotationStep_ = 1;

    //! Rotation based on keys R (left) and T (right)
    private void Update() {
        if(Input.GetKey(KeyCode.R)) {
            transform.Rotate(Vector3.up, -rotationStep_, Space.World);
        } else if(Input.GetKey(KeyCode.T)) {
            transform.Rotate(Vector3.up, rotationStep_, Space.World);
        }
    }
}