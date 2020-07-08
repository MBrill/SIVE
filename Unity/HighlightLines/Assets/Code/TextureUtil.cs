using UnityEngine;
using System.Collections;

public static class TextureUtil {

	/*!
	 * \brief Color a texture black
	 * \param[in] width The desired width.
	 * \param[in] height The desired height.
	 * \param[out] texture The texture.
	 * \return void
	 */
	public static void ColorTextureBlack(int width, int height ,out Texture2D tex)
	{
		tex = new Texture2D(width,height);
		Color[] colors = new Color[width * height];
		for(int i = 0; i<colors.Length; i++)
		{
			colors[i] = Color.black;
		}
		tex.SetPixels(colors);
		tex.Apply();
	}

	/*!
	 * \brief Add a white line with a specific thickness in the middle of a texture.
	 * \param[in] thickness The thickness of the line
	 * \param[in] tex The texture.
	 * \return void
	 */
	public static void AddWhiteLineToTexture(float thickness, Texture2D tex)
	{
		var colors = tex.GetPixels();
		
		int middle = (tex.width / 2) - 1 ;
		var start = new int[] {middle, 0};
		var end  = new int[] {middle, tex.height - 1};
		
		LineDrawer.plotThickLineAA(start[0], start[1], end[0], end[1], thickness, delegate(int x, int y, float i) {
			colors[y * tex.width + (tex.width - 1 - x)] = new Color(i, i, i);
		});
		
		tex.SetPixels(colors);
		tex.Apply();
	}
}
