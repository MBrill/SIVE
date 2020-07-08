using System;

/*!
 * \brief Utility class for Drawing lines according to a modified Bresenham Algorithm.
 * 
 * The actual drawing is done inside a delegate function. This allows the use of this class for drawing the line to any surface.
 * 
 * \ingroup ReflectionLine
 * \author Patrick Schwartz
 * \date 2014-2015
 */
public static class LineDrawer {

    //! Delegate for the Function which actually sets the pixel. This way the plotLine Functions are not dependent on the specific Layout of a Cubemap texture.
    public delegate void SetPixelColorFunction(int x, int y);

    //! Same as SetPixelColorFunction with intensity of a pixel color.
    public delegate void SetPixelColorAAFunction(int x, int y, float intensity);

    /*!
     * \brief Plot a line from Point (x0, y0) to (x1, y1).
     *
     * Line is 1 pixel wide and is NOT anti-aliased.
     *
     * Based on: A. Zingl, “A Rasterizing Algorithm for Drawing Curves,” Technikum-Wien, Wien, Studentische Arbeit, 2012. http://members.chello.at/~easyfilter/bresenham.html
     * and: J. E. Bresenham, “Algorithm for computer control of a digital plotter,” IBM Systems Journal, vol. 4, no. 1, pp. 25–30, 1965.
     *
     * Original by  A. Zingl was refactored for readability.
     * 
     * \param x0 x component of the startpoint. In pixel.
     * \param y0 y component of the startpoint. In pixel.
     * \param x1 x component of the endpoint. In pixel.
     * \param y1 y component of the endpoint. In pixel.
     * \param setPixel Function for actually setting a Pixel. Does not matter how that works.
     */

    public static void plotLine(int x0, int y0, int x1, int y1, SetPixelColorFunction setPixel) {
        int dX = System.Math.Abs(x0 - x1);
        int dY = -System.Math.Abs(y0 - y1);

        // For negative gradients or reversed lines
        int stepX = x0 < x1 ? 1 : -1;
        int stepY = y0 < y1 ? 1 : -1;

        int error = dX + dY;

        int x = x0;
        int y = y0;

        int error2 = error * 2;

        for(; ; ) {
            setPixel(x, y);
            if(error2 >= dY) {
                if(x == x1) break;
                error += dY;
                x += stepX;
            }
            if(error2 <= dX) {
                if(y == y1) break;
                error += dX;
                y += stepY;
            }
        }
    }

    /*!
     * \brief Plot a anti aliased thick line from Point (x0, y0) to (x1, y1).
     *
     * Line is wd pixel wide and is anti-aliased.
     *
     * Based on: A. Zingl, “A Rasterizing Algorithm for Drawing Curves,” Technikum-Wien, Wien, Studentische Arbeit, 2012. http://members.chello.at/~easyfilter/bresenham.html
     * and: J. E. Bresenham, “Algorithm for computer control of a digital plotter,” IBM Systems Journal, vol. 4, no. 1, pp. 25–30, 1965.
     *
     * Original function from A. Zingl was modified to output the intensity of a pixel instead of a color value. Also some refactoring for better readability was done.
     * 
     * \param x0 x component of the startpoint. In pixel.
     * \param y0 y component of the startpoint. In pixel.
     * \param x1 x component of the endpoint. In pixel.
     * \param y1 y component of the endpoint. In pixel.
     * \param wd Width of the Line. In Pixel
     * \param setPixel Function for actually setting a Pixel. Does not matter how that works.
     */

    public static void plotThickLineAA(int x0, int y0, int x1, int y1, float wd, SetPixelColorAAFunction setPixel) {
        int dx = Math.Abs(x1 - x0);
        int dy = Math.Abs(y1 - y0);

        // For negative gradients or reversed lines
        int stepX = x0 < x1 ? 1 : -1;  
        int stepY = y0 < y1 ? 1 : -1;

        // error value e_xy
        int err = dx - dy;
        int e2 = 0;
        int x2 = 0;
        int y2 = 0;

        double ed = dx + dy == 0 ? 1 : Math.Sqrt(dx * dx + dy * dy);

        for(wd = (wd + 1) / 2; ; ) { /* pixel loop */
            var intensity = 1.0 - (Math.Abs(err - dx + dy) / ed - wd + 1);
            setPixel(x0, y0, intensity < 0 ? 1.0f : (float)intensity);
            e2 = err;
            x2 = x0;
            if(2 * e2 >= -dx) { /* x step */
                for(e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx) {
                    intensity = 1.0 - (Math.Abs(e2) / ed - wd + 1);
                    setPixel(x0, y2 += stepY, intensity < 0 ? 1.0f : (float)intensity);
                }

                if(x0 == x1) break;
                e2 = err;
                err -= dy;
                x0 += stepX;
            }
            if(2 * e2 <= dy) { /* y step */
                for(e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy) {
                    intensity = 1.0 - (Math.Abs(e2) / ed - wd + 1);
                    setPixel(x2 += stepX, y0, intensity < 0 ? 1.0f : (float)intensity);
                }

                if(y0 == y1) break;
                err += dx;
                y0 += stepY;
            }
        }
    }
}