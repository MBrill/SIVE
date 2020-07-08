using UnityEngine;

/*! 
 * \defgroup ReflectionLine
 * \brief The ReflectionLine Project. Implements reflection lines in Unity.
 * \author Patrick Schwartz
 * \date 2014-2015
 */

/*!
 * \brief This Script handles the rotation of the reflection line. It triggers an update of the corresponding Cubemap.
 *
 * \ingroup ReflectionLine
 * \author Patrick Schwartz
 * \date 2014-2015
 */

public class ReflectionLineManager : MonoBehaviour {

    //! The Cubemap that will be updated with this Manager
    public Cubemap cubeMap_ = null;

    //! Rotation of the Reflection line
    public float reflectionLineRotation_ = 0.0f;

    //! How much degrees a single rotation rotates
    public float rotationStep_ = 1.0f;

    //! Width of the line in pixel
    public int lineWidth_ = 5;

    // Use this for initialization
    public void Start() {
        if(cubeMap_ == null) {
            Debug.LogError(name + ": Missing Cubemap. This Script cannot work without a Cubemap.");
            enabled = false;
            //ApplicationControlUtil.Quit();
            return;
        }
        updateLineInCubemap();
    }

    //! Rotate the line based on keys Q and E.
    public void Update() {
        // Controls
        if(Input.GetKey(KeyCode.Q)) {
            rotateLeft();
        } else if(Input.GetKey(KeyCode.E)) {
            rotateRight();
        }
    }

    //! Fill Cubemap with black again.
    public void OnDisable() {
        if(cubeMap_ != null)
            CubemapUtil.initCubeMap(cubeMap_);
    }

    //! Rotate the reflection line right.
    private void rotateRight() {
        reflectionLineRotation_ = (reflectionLineRotation_ + rotationStep_) % 360.0f;
        updateLineInCubemap();
    }

    //! Rotate the reflection line left.
    private void rotateLeft() {
        var value = (reflectionLineRotation_ - rotationStep_);

        reflectionLineRotation_ = value >= 0 ? value : (360.0f - Mathf.Abs(value));
        updateLineInCubemap();
    }

    //! Update the cubemap texture. Adds the Line in the specified rotation.
    private void updateLineInCubemap() {
        CubemapUtil.initCubeMapFace(cubeMap_, CubemapFace.PositiveY);
        CubemapUtil.addReflectionlineToCubemap(cubeMap_, reflectionLineRotation_, lineWidth_, CubemapFace.PositiveY);
    }
}