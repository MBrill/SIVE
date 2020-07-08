using UnityEngine;

/*!
 * \brief Utility class for modifying Cubemaps.
 *
 * \ingroup ReflectionLine
 * \author Patrick Schwartz
 * \date 2014
 */

public static class CubemapUtil {
    /*!
     * Initialize a Cubemap with black on all sides;
     *
     * \param cubemap The Cubemap to initialize
     */

    public static void initCubeMap(Cubemap cubemap) {
        Color[] colors = new Color[cubemap.width * cubemap.height];

        for(uint i = 0; i < colors.Length; ++i) {
            colors[i] = Color.black;
        }

        cubemap.SetPixels(colors, CubemapFace.NegativeX);
        cubemap.SetPixels(colors, CubemapFace.NegativeY);
        cubemap.SetPixels(colors, CubemapFace.NegativeZ);
        cubemap.SetPixels(colors, CubemapFace.PositiveX);
        cubemap.SetPixels(colors, CubemapFace.PositiveY);
        cubemap.SetPixels(colors, CubemapFace.PositiveZ);

        cubemap.Apply();
    }

    /*!
     * Initialize a single Cubemap face to all black.
     *
     * \param cubemap The Cubemap.
     * \param face The face to init.
     */

    public static void initCubeMapFace(Cubemap cubemap, CubemapFace face) {
        Color[] colors = new Color[cubemap.width * cubemap.height];

        for(uint i = 0; i < colors.Length; ++i) {
            colors[i] = Color.black;
        }

        cubemap.SetPixels(colors, face);

        cubemap.Apply();
    }

    /*!
     * Add a reflection line with the specified Rotation on the specified face.
     *
     * \param cubemap The Cubemap to add to.
     * \param rotation Rotation of the reflection line
     * \param faces The Cubemap faces to add the line to.
     */

    public static void addReflectionlineToCubemap(Cubemap cubemap, float rotation, int lineWidth, params CubemapFace[] faces) {
        UnityDebug.assert(cubemap.width == cubemap.height, "Cubemap is not square!");

        foreach(CubemapFace face in faces) {
            var colors = cubemap.GetPixels(face);

            int middle = (cubemap.width / 2) - 1;

            var center = new int[] { middle, middle };

            var origStart = new int[] { middle, 0 };
            var newStart = rotatePointAroundPoint2D(origStart, center, rotation);

            var origEnd = new int[] { middle, cubemap.height - 2 };
            var newEnd = rotatePointAroundPoint2D(origEnd, center, rotation);

            LineDrawer.plotThickLineAA(newStart[0], newStart[1], newEnd[0], newEnd[1], lineWidth, delegate(int x, int y, float i) {
                colors[y * cubemap.width + (cubemap.width - 1 - x)] = new Color(i, i, i);
            });

            cubemap.SetPixels(colors, face);
        }

        if(faces.Length > 0)
            cubemap.Apply();
    }

    /*!
     * \brief Helper function for rotating a point around another point.
     *
     * \param orig Original point.
     * \param center The Point to rotate around.
     * \param rotationAngle The angle of rotation in degrees.
     * \returns Rotated point.
     */

    private static int[] rotatePointAroundPoint2D(int[] orig, int[] center, float rotationAngle) {
        float s = Mathf.Sin(rotationAngle * Mathf.Deg2Rad);
        float c = Mathf.Cos(rotationAngle * Mathf.Deg2Rad);

        var point = new int[] { orig[0] - center[0], orig[1] - center[1] };

        float newX = point[0] * c - point[1] * s;
        float newy = point[0] * s + point[1] * c;

        return new int[] { (int)newX + center[0], (int)newy + center[1] };
    }
}
