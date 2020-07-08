using UnityEditor;
using UnityEngine;

/*!
 * \brief Custom Inspector for ReflectionLineManager
 * 
 * Allows creating a new Cubemap should none be assigned.
 */
[CustomEditor(typeof(ReflectionLineManager))]
public class ReflectionLineManagerEditor : Editor {
    private const string CubemapText = "Cubemap";
    private const string CubemapTooltip = "The Cubemap that will be updated with this Manager.";
    private const string ReflectionLineRotationText = "Current rotation (degrees).";
    private const string ReflectionLineRotationTooltip = "Rotation of the Reflectionline in degrees.";
    private const string RotationStepText = "Rotation step";
    private const string RotationStepTooltip = "How much degrees a single rotation rotates. Controlled with LeftArrow and RightArrow.";
    private const string cubemapOverwriteWarning = "The assigned Cubemap will be overwritten by this Script!";
    private const string LineWidthText = "Line Width (Pixel)";
    private const string CubemapSizeHelpText = "Size needs to be a multiple of 2 and >= 16!";
    private const string GenerateButtonText = "Generate new Cubemap";

    private SerializedProperty cubeMapProperty_ = null;
    private SerializedProperty reflectionLineRotationProperty_ = null;
    private SerializedProperty rotationStepProperty_ = null;
    private SerializedProperty lineWidthProperty_ = null;

    public void OnEnable() {
        cubeMapProperty_ = serializedObject.FindProperty("cubeMap_");
        reflectionLineRotationProperty_ = serializedObject.FindProperty("reflectionLineRotation_");
        rotationStepProperty_ = serializedObject.FindProperty("rotationStep_");
        lineWidthProperty_ = serializedObject.FindProperty("lineWidth_");
    }

    public override void OnInspectorGUI() {
        serializedObject.Update();

        EditorGUILayout.PropertyField(cubeMapProperty_, new GUIContent(CubemapText, CubemapTooltip));
        EditorGUILayout.HelpBox(cubemapOverwriteWarning, MessageType.Warning);
        EditorGUILayout.Space();
        reflectionLineRotationProperty_.floatValue = EditorGUILayout.Slider(new GUIContent(ReflectionLineRotationText, ReflectionLineRotationTooltip), reflectionLineRotationProperty_.floatValue, 0.0f, 360.0f);
        EditorGUILayout.PropertyField(rotationStepProperty_, new GUIContent(RotationStepText, RotationStepTooltip));
        lineWidthProperty_.intValue = EditorGUILayout.IntSlider(new GUIContent(LineWidthText), lineWidthProperty_.intValue, 1, 100);

        // Allow creating a new Cubemap
        if(cubeMapProperty_.objectReferenceValue == null) {
            EditorGUILayout.Space();
            EditorGUILayout.LabelField(new GUIContent("Cubemap Creation"), EditorStyles.boldLabel);

            int textureSize = 512;
            textureSize = EditorGUILayout.IntField(new GUIContent("Size"), textureSize);
            EditorGUILayout.HelpBox(CubemapSizeHelpText, MessageType.Warning, true);

            float result = Mathf.Log(textureSize, 2.0f);
            if((result % 1.0f) > 0.0f) {
                Debug.LogWarning(target.name + ": Only exponents of 2 are supported for size.");
                textureSize = Mathf.ClosestPowerOfTwo((int)result);
            }
            if(textureSize < 16) {
                Debug.LogWarning(target.name + ": Texture size needs to be >= 16!");
                textureSize = 16;
            }

            var path = "Assets/Cubemaps";
            path = EditorGUILayout.TextField(new GUIContent("Path"), path);

            if(GUILayout.Button(new GUIContent(GenerateButtonText))) {
                var cubeMap = new Cubemap(textureSize, TextureFormat.ARGB32, false);
                CubemapUtil.initCubeMap(cubeMap);

                AssetDatabase.CreateFolder("Assets", "Cubemaps");
                AssetDatabase.CreateAsset(cubeMap, path + "/CubeMap" + cubeMap.GetInstanceID() + ".cubemap");

                cubeMapProperty_.objectReferenceValue = cubeMap;
            }
        }
        serializedObject.ApplyModifiedProperties();
    }
}