#include "pix.h"

using namespace tcaxLib;

pix::pix() :
    pPoints(new TCAX_Points),
    pPix(new TCAX_Pix)
{}

pix::~pix()
{
    delete pPoints;
    delete pPix;
}

//PixPoints(PIX), where "PIX" is tuple
TCAX_PyPoints pix::pix_points(TCAX_PyPix &PIX)
{
    int i, w, h;
    if (py::len(PIX) < 1)
    {
        PyErr_SetString(PyExc_RuntimeError, "PixPoints error, too less parameters - `(PIX)'\n");
        return PIX; //no change
    }

    this->convert_py_pix(PIX);
    pPoints->count = 0;
    pPoints->capacity = 100;
    pPoints->xBuf = new double[pPoints->capacity]();
    pPoints->yBuf = new double[pPoints->capacity]();
    pPoints->aBuf = new unsigned char[pPoints->capacity]();
    for (h = 0; h < pPix->height; ++h)
    {
        for (w = 0; w < pPix->width; ++w)
        {
            unsigned char a = pPix->buf[((h * pPix->width + w) << 2) + 3];
            if (0 != a)
                points_append(w, h, a);
        }
    }

    delete[] pPix->buf;

    py::list pyPoints;
    for (i = 0; i < pPoints->count; i++)
    {
        pyPoints.append(py::make_tuple(pPoints->xBuf[i], pPoints->yBuf[i], pPoints->aBuf[i]));
    }

    delete[] pPoints->xBuf;
    delete[] pPoints->yBuf;
    delete[] pPoints->aBuf;

    return py::tuple(pyPoints);
}

//BlankPix(width, height, rgba)
TCAX_PyPix pix::create_blank_pix(int width, int height, uint32_t rgba)
{
    int i;

    pPix->initX = 0;
    pPix->initY = 0;
    pPix->width = width;
    pPix->height = height;
    pPix->size = pPix->height * (pPix->width << 2);
    pPix->buf = new unsigned char[pPix->size]();
    for (i = 0; i < pPix->size; i += 4)
        *((uint32_t *)&pPix->buf[i]) = rgba;
    return convert_pix(1);
}

//PixResize(PIX, width, height)
TCAX_PyPix pix::resample_py_pix(TCAX_PyPix &PIX, int width, int height)
{
    if (width < 0 || height < 0 || (0 == width && 0 == height))
    {
        PyErr_SetString(PyExc_RuntimeError, "PixResize error, invalid width or height value\n");
        return PIX;
    }

    TCAX_pPix pixDst = new TCAX_Pix;
    convert_py_pix(PIX);
    pixDst->width = width;
    pixDst->height = height;
    resample_pix(pPix, pixDst);

    TCAX_pPix tmp_ptr = pPix;
    pPix = pixDst;
    delete[] tmp_ptr->buf;
    delete tmp_ptr;

    return convert_pix(1);
}

//PixResizeF(PIX, width, height)
TCAX_PyPix pix::resample_py_pix_ex(TCAX_PyPix &PIX, double width, double height)
{
    if (width < 0 || height < 0 || (0 == width && 0 == height))
    {
        PyErr_SetString(PyExc_RuntimeError, "PixResizeF error, invalid width or height value\n");
        return PIX;
    }

    TCAX_pPix pixDst = new TCAX_Pix;
    convert_py_pix(PIX);
    resample_pix_ex(pPix, pixDst, width, height);

    TCAX_pPix tmp_ptr = pPix;
    pPix = pixDst;
    delete[] tmp_ptr->buf;
    delete tmp_ptr;

    return convert_pix(1);
}

/*private member function*/
unsigned char pix::CLIP_0_255(int a)
{
    if (a < 0)
    {
        return static_cast<unsigned char>(0);
    }
    else if (a > 255)
    {
        return static_cast<unsigned char>(255);
    }
    else
    {
        return static_cast<unsigned char>(a);
    }
}

void pix::points_append(double x, double y, unsigned char a)
{
    double *xBuf, *yBuf;
    unsigned char *aBuf;
    int count, capacity;
    xBuf = pPoints->xBuf;
    yBuf = pPoints->yBuf;
    aBuf = pPoints->aBuf;
    count = pPoints->count;

    capacity = pPoints->capacity;
    if (count + 1 > capacity)
    {  /* we need more space */
        capacity += capacity / 2 + 1;
        xBuf = (double *)realloc(xBuf, capacity * sizeof(double));
        yBuf = (double *)realloc(yBuf, capacity * sizeof(double));
        aBuf = (unsigned char *)realloc(aBuf, capacity * sizeof(unsigned char));
        pPoints->xBuf = xBuf;
        pPoints->yBuf = yBuf;
        pPoints->aBuf = aBuf;
        pPoints->capacity = capacity;
    }

    xBuf[count] = x;
    yBuf[count] = y;
    aBuf[count] = a;
    count++;
    pPoints->count = count;
}

TCAX_PyPix pix::convert_pix(int delPix)
{
    int i;
    py::tuple pyTemp = py::make_tuple(pPix->initX, pPix->initY);
    py::tuple pyTemp2 = py::make_tuple(pPix->width, pPix->height);
    py::list tmp_list;
    for (i = 0; i < pPix->size; ++i)
    {
        tmp_list.append(pPix->buf[i]);
    }
    py::tuple pyTemp3 = py::tuple(tmp_list);
    if (delPix) free(pPix->buf);
    return py::make_tuple(pyTemp, pyTemp2, pyTemp3);
}

void pix::convert_py_pix(const py::tuple &pyPix)
{
    int i;
    py::tuple pos, size, buf;
    pos = py::extract<py::tuple>(pyPix[0]);
    size = py::extract<py::tuple>(pyPix[1]);
    buf = py::extract<py::tuple>(pyPix[2]);

    pPix->initX = py::extract<double>(pos[0]);
    pPix->initY = py::extract<double>(pos[1]);
    pPix->width = py::extract<int>(size[0]);
    pPix->height = py::extract<int>(size[1]);

    pPix->size = pPix->height * (pPix->width << 2);
    pPix->buf = new unsigned char[pPix->size]();

    for (i = 0; i < pPix->size; ++i)
        pPix->buf[i] = py::extract<unsigned char>(buf[i]);
}

double pix::filter_MitchellNetravali(double x, double b, double c)
{
    double p0, p2, p3, q0, q1, q2, q3;
    p0 = (   6 -  2 * b          ) / 6.0;
    p2 = ( -18 + 12 * b +  6 * c ) / 6.0;
    p3 = (  12 -  9 * b -  6 * c ) / 6.0;
    q0 = (        8 * b + 24 * c ) / 6.0;
    q1 = (     - 12 * b - 48 * c ) / 6.0;
    q2 = (        6 * b + 30 * c ) / 6.0;
    q3 = (     -      b -  6 * c ) / 6.0;
    if (x < 0) x = -x;    /* x = fabs(x) */
    if (x < 1)
        return p0 + (p2 + p3 * x) * x * x;
    else if (x < 2)
        return q0 + (q1 + (q2 + q3 * x) * x) * x;
    else return 0;
}

double pix::filter_cubic(double x, double a)
{
    if (x < 0) x = -x;    /* x = fabs(x) */
    if (x < 1) return (a + 2) * x * x * x - (a + 3) * x * x + 1;
    else if (x < 2) return a * x * x * x - 5 * a * x * x + 8 * a * x - 4 * a;
    else return 0;
}

double pix::filter_BSpline(double x)
{
    if (x < -2) return 0;
    else if (x < -1) return (2 + x) * (2 + x) * (2 + x) / 6.0;
    else if (x < 0) return (4 + (-6 - 3 * x) * x * x) / 6.0;
    else if (x < 1) return (4 + (-6 + 3 * x) * x * x) / 6.0;
    else if (x < 2) return (2 - x) * (2 - x) * (2 - x) / 6.0;
    else return 0;
}

void pix::resample_rgba(const unsigned char *src, int width, int height,
                        unsigned char *dst, int targetWidth, int targetHeight)
{
    int h, w, m, n, index;
    double fx, fy;
    int ix, iy, xx, yy;
    double xScale, yScale, r1, r2;
    double rr, gg, bb, aa;
    if (targetWidth == width && targetHeight == height)
        memcpy(dst, src, height * (width << 2) * sizeof(unsigned char));
    else
    {
        xScale = targetWidth / (double)width;
        yScale = targetHeight / (double)height;
        for (h = 0; h < targetHeight; ++h)
        {
            fy = h / yScale;
            iy = (int)fy;
            for (w = 0; w < targetWidth; ++w)
            {
                fx = w / xScale;
                ix = (int)fx;
                rr = 0;
                gg = 0;
                bb = 0;
                aa = 0;
                for (m = 0; m < 4; ++m)
                {
                    yy = iy + m - 1;
                    r1 = filter_cubic(yy - fy, -0.5);    /* can use _tcaxlib_filter_MitchellNetravali (best quality but slowest) or _tcaxlib_filter_BSpline (smoothest) */
                    if (yy < 0) yy = 0;
                    if (yy >= height) yy = height - 1;
                    for (n = 0; n < 4; ++n)
                    {
                        xx = ix + n - 1;
                        r2 = r1 * filter_cubic(xx - fx, -0.5);    /* can use _tcaxlib_filter_MitchellNetravali (best quality but slowest) or _tcaxlib_filter_BSpline (smoothest) */
                        if (xx < 0) xx = 0;
                        if (xx >= width) xx = width - 1;
                        index = (yy * width + xx) << 2;
                        rr += src[index] * r2;
                        gg += src[index + 1] * r2;
                        bb += src[index + 2] * r2;
                        aa += src[index + 3] * r2;
                    }
                }
                index = (h * targetWidth + w) << 2;
                dst[index] = CLIP_0_255((int)rr);
                dst[index + 1] = CLIP_0_255((int)gg);
                dst[index + 2] = CLIP_0_255((int)bb);
                dst[index + 3] = CLIP_0_255((int)aa);
            }
        }
    }
}

TCAX_pPix pix::resample_pix(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst)
{
    if (0 == pPixDst->width)
        pPixDst->width = pPixDst->height * pPixSrc->width / pPixSrc->height;    /* keep aspect ratio */
    else if (0 == pPixDst->height)
        pPixDst->height = pPixDst->width * pPixSrc->height / pPixSrc->width;    /* keep aspect ratio */

    pPixDst->initX = pPixSrc->initX - (pPixDst->width - pPixSrc->width) / 2;
    pPixDst->initY = pPixSrc->initY - (pPixDst->height - pPixSrc->height) / 2;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    pPixDst->buf = new unsigned char[pPixDst->size]();

    resample_rgba(pPixSrc->buf, pPixSrc->width, pPixSrc->height, pPixDst->buf, pPixDst->width, pPixDst->height);
    return pPixDst;
}

TCAX_pPix pix::resample_pix_ex(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst, double width, double height)
{
    const int m = 4;
    double f_w, f_h, f_mw, f_mh;
    int i_mw, i_mh, pad_w, pad_h;
    int sf_mw, sf_mh, sf_msize;
    unsigned char *sBuf, *bufm;
    int i, index1, index2, size;
    f_w = width;
    f_h = height;
    if (0 == f_w)
        f_w = f_h * pPixSrc->width / static_cast<double>(pPixSrc->height);    /* keep aspect ratio */
    else if (0 == f_h)
        f_h = f_w * pPixSrc->height / static_cast<double>(pPixSrc->width);    /* keep aspect ratio */

    f_mw = m * f_w;
    f_mh = m * f_h;
    i_mw = static_cast<int>(f_mw);
    i_mh = static_cast<int>(f_mh);
    pad_w = m - i_mw % m;
    if (m == pad_w)
        pad_w = 0;
    pad_h = m - i_mh % m;
    if (m == pad_h)
        pad_h = 0;

    sf_mw = i_mw + pad_w;
    sf_mh = i_mh + pad_h;
    sf_msize = sf_mh * (sf_mw << 2);

    sBuf = new unsigned char[sf_msize](); /* now we've got the surface to draw */
    bufm = new unsigned char[(i_mh * (i_mw << 2))](); /* magnitude the source to m * m */

    resample_rgba(pPixSrc->buf, pPixSrc->width, pPixSrc->height, bufm, i_mw, i_mh);
    size = (i_mw << 2) * sizeof(unsigned char);

    for (i = 0; i < i_mh; i++)
    {
        index1 = i * (sf_mw << 2) * sizeof(unsigned char);
        index2 = i * (i_mw << 2) * sizeof(unsigned char);
        memcpy(sBuf + index1, bufm + index2, size);
    }

    delete[] bufm;
    pPixDst->initX = pPixSrc->initX - (f_w - pPixSrc->width) / 2;
    pPixDst->initY = pPixSrc->initY - (f_h - pPixSrc->height) / 2;
    pPixDst->width = sf_mw / m;
    pPixDst->height = sf_mh / m;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    pPixDst->buf = new unsigned char[pPixDst->size]();
    resample_rgba(sBuf, sf_mw, sf_mh, pPixDst->buf, pPixDst->width, pPixDst->height);
    delete[] sBuf;
    return pPixDst;
}