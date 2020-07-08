/*!
 * \brief Utility class for controlling the state of the Application.
 * \ingroup DebugUtils
 * \author Patrick Schwartz
 * \date 2014-2015
 */
public static class ApplicationControlUtil {

    //! Will quit the application regardless of being run as standalone or inside the editor.
    public static void Quit() {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }
}