using UnityEngine;

/*!
 * \defgroup DebugUtils 
 * \brief Various debug utilities that were created during this project.
 * \author Patrick Schwartz
 */

/*!
 * \brief Debug utility class
 * \ingroup DebugUtils
 * \author Patrick Schwartz
 */
public static class UnityDebug {
    /*!
     * \brief Assert function. Will pause the Editor playback if the condition is false.
     *
     * Will only be called if UNITY_EDITOR is defined.
     */

    [System.Diagnostics.Conditional("UNITY_EDITOR")]
    public static void assert(bool condition, string message) {
        if(!condition) {
            Debug.LogError("Assertion failed! : " + message + "\n" + new System.Diagnostics.StackTrace(1).ToString());
            Debug.Break();
        }
    }
}
