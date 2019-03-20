#ifndef TCAXLIB_PIX_H
#define TCAXLIB_PIX_H

#include "common.h"

namespace tcaxLib
{
    /**
     * TCAX_Pix structure used to store pixel info of text(s) / image(s).
     */
    typedef struct _tcax_pix
    {
        double initX;       /**< left most */
        double initY;       /**< upper most */
        int width;          /**< width of the PIX */
        int height;         /**< height of the PIX */
        int size;           /**< size = height * (width * 4) */
        unsigned char *buf; /**< pixel values of the PIX */
    } TCAX_Pix, *TCAX_pPix;

    /**
     * TCAX_Points structure is an auto increase list.
     */
    typedef struct _tcax_points
    {
        double *xBuf;
        double *yBuf;
        unsigned char *aBuf;
        int count;       /**< number of points */
        int capacity;    /**< maximun points that the buffer can contain */
    } TCAX_Points, *TCAX_pPoints;

    /**
     * TCAX_PyPix structure that has the following structure
     * ((initX, initY), (width, height), (a1, a2, a3, ...)).
     */
    typedef py::tuple TCAX_PyPix;

    /**
     * TCAX_PyBigPix structure that has the following structure
     * [(PIX, offset_x, offset_y, layer), (PIX, offset_x, offset_y, layer), ...].
     */
    typedef py::list TCAX_PyBigPix;

    /**
     * TCAX_PyPoints structure a tuple of tuples (points).
     * ((x1, y1, a1), (x2, y2, a2), ...)
     */
    typedef py::tuple TCAX_PyPoints;

    class pix {
    public:

        pix();

        ~pix();

        /**
         * Get points from TCAX PY PIX.
         * @param PIX
         * @return TCAX_PyPoints
         */
        //PixPoints(PIX), where "PIX" is tuple
        TCAX_PyPoints pix_points(TCAX_PyPix &PIX);

        /**
         * Create a blank PIX.
         * @param width
         * @param height
         * @param rgba
         * @return TCAX_PyPix
         */
        //BlankPix(width, height, rgba)
        TCAX_PyPix create_blank_pix(int width, int height, uint32_t rgba);

        /**
         * Resample TCAX PY PIX.
         * @param PIX
         * @param width
         * @param height
         * @return TCAX_PyPix
         */
        //PixResize(PIX, width, height)
        TCAX_PyPix resample_py_pix(TCAX_PyPix &PIX, int width, int height);

        /**
         * supports fractional width and height
         */
        //PixResizeF(PIX, width, height)
        TCAX_PyPix resample_py_pix_ex(TCAX_PyPix &PIX, double width, double height);

    protected:

        TCAX_pPoints pPoints;

        TCAX_pPix pPix;

        /**
         * Clip a number into 0 to 255 inclusive
         */
        unsigned char CLIP_0_255(int a);

        /**
         * pPoints->xBuf and pPoints->yBuf need to be allocated before being passed to the function,
         * and it will be reallocated if more space is needed
         * pPoints->count and pPoints->capacity should also be specified before being passed to the function,
         * and their value may be modified
         */
        void points_append(double x, double y, unsigned char a);

        /**
         * Convert TCAX PIX to TCAX PY PIX.
         * @param pPix pointer to TCAX_Pix structure
         * @param delPix indicates whether to delete PIX before returning of the function
         * @return TCAX_PyPix
         */
        TCAX_PyPix convert_pix(int delPix = 0);

        /**
         * Convert TCAX PY PIX to TCAX PIX.
         * Remark: in the following functions we will just use PIX,
         * it may stand for TCAX PIX or TCAX PY PIX, in case that there is no ambiguity.
         *
         * @param pyPix the TCAX_PyPix structure
         */
        void convert_py_pix(const TCAX_PyPix &pyPix);

        /* resample image */
        double filter_MitchellNetravali(double x, double b, double c);

        double filter_cubic(double x, double a);

        double filter_BSpline(double x);

        void resample_rgba(const unsigned char *src, int width, int height,
                           unsigned char *dst, int targetWidth, int targetHeight);

        /**
         * Resample TCAX PIX.
         * Remark: the target width and height should be specified in the target TCAX_Pix structure before it is sent to the function.
         * Other variables will be ignored. Note that the `buf' of the target TCAX_Pix structure should not be allocated before it is
         * sent to the function, otherwise will cause memory leak.
         *
         * @param pPixSrc pointer to source TCAX_Pix structure
         * @param pPixDst pointer to target TCAX_Pix structure
         * @return TCAX_pPix
         */
        TCAX_pPix resample_pix(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst);

        /**
         * supports fractional width and height
         */
        TCAX_pPix resample_pix_ex(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst, double width, double height);
    };
};

#endif //TCAXLIB_PIX_H
