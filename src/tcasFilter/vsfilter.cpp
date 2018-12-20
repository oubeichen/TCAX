#include "vsfilter.h"

static void VS_CC invertInit(VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi) {
	VSFilterData *d = (VSFilterData *) * instanceData;
    vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef *VS_CC vsfilterGetFrame(int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
	VSFilterData *d = (VSFilterData *) * instanceData;

	if (activationReason == arInitial) {
		vsapi->requestFrameFilter(n, d->node, frameCtx);
	}
	else if (activationReason == arAllFramesReady) {
		const VSFrameRef *src = vsapi->getFrameFilter(n, d->node, frameCtx);
		VSFrameRef *dst = vsapi->copyFrame(src, core);
		//vsapi->getFrameFormat(src);
		//VSFrameRef *dst = vsapi->newVideoFrame(vsapi->getFrameFormat(src), vsapi->getFrameWidth(src, 0), vsapi->getFrameHeight(src, 0), nullptr, core);

		vsapi->copyFrameProps(src, dst, core);

		if (d->vi->format->id == pfYUV420P8) {
			vsfilterRenderYV12(src, dst, n, d, vsapi);
		}
		else if (d->vi->format->id == pfCompatYUY2) {
			vsfilterRenderYUV2(src, dst, n, d, vsapi); /// deprecated
		}
		else if (d->vi->format->id == pfRGB24) {
			vsfilterRenderRGB24(src, dst, n, d, vsapi); /// deprecated
		}
		else {
			return nullptr;
		}

		vsapi->freeFrame(src);
		return dst;
	}

	return nullptr;
}

void vsfilterRenderYV12(const VSFrameRef *pSrc, VSFrameRef *pDst, int n, VSFilterData *d, const VSAPI *vsapi) {
	tcas_u16  x, y;
	tcas_byte r, g, b, a;
	const tcas_byte *srcpY = vsapi->getReadPtr(pSrc, VSPLANAR_Y);
	const tcas_byte *srcpV = vsapi->getReadPtr(pSrc, VSPLANAR_V);
	const tcas_byte *srcpU = vsapi->getReadPtr(pSrc, VSPLANAR_U);
	tcas_byte *dstpY = vsapi->getWritePtr(pDst, VSPLANAR_Y);
	tcas_byte *dstpV = vsapi->getWritePtr(pDst, VSPLANAR_V);
	tcas_byte *dstpU = vsapi->getWritePtr(pDst, VSPLANAR_U);
	int src_pitchY = vsapi->getStride(pSrc, VSPLANAR_Y);
	int src_pitchUV = vsapi->getStride(pSrc, VSPLANAR_V);
	int dst_pitchY = vsapi->getStride(pDst, VSPLANAR_Y);
	int dst_pitchUV = vsapi->getStride(pDst, VSPLANAR_V);
	int width = vsapi->getFrameWidth(pDst, VSPLANAR_Y);
	int height = vsapi->getFrameHeight(pDst, VSPLANAR_Y);

#if 0
	// copy data from source video frame to target video frame
	if (dst_pitchY == src_pitchY) {
		int sizeY = height * src_pitchY;
		int sizeUV = sizeY >> 2;
		::memcpy(dstpY, srcpY, sizeY);
		::memcpy(dstpV, srcpV, sizeUV);
		::memcpy(dstpU, srcpU, sizeUV);
	}
	else {
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				dstpY[h * dst_pitchY + w] = srcpY[h * src_pitchY + w];
			}
		}
		int hUV = height >> 1;
		int wUV = width >> 1;
		for (int h = 0; h < hUV; h++) {
			for (int w = 0; w < wUV; w++) {
				dstpV[h * dst_pitchUV + w] = srcpV[h * src_pitchUV + w];
				dstpU[h * dst_pitchUV + w] = srcpU[h * src_pitchUV + w];
			}
		}
	}
#endif
	// Create TCAS frame
	tcas_byte *tcasFrameBuf;
	if (d->bUseFrameCache)
		while (!::libtcas_frame_cache_get(&d->frameCache, n, &tcasFrameBuf)) continue;
	else
		::libtcas_framer_get(&d->framer, n, &tcasFrameBuf);
	// Render
	int tcasPitch = (width << 2) * sizeof(tcas_byte);
	// Y
	for (int h = 0; h < height; h++) {
		y = h;
		for (int w = 0; w < width; w++) {
			x = w;
			int w4 = w << 2;
			int yy = h * tcasPitch;
			r = tcasFrameBuf[yy + w4];
			g = tcasFrameBuf[yy + w4 + 1];
			b = tcasFrameBuf[yy + w4 + 2];
			a = tcasFrameBuf[yy + w4 + 3];
			if (0 == a) continue;
			tcas_byte Y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			yy = y * dst_pitchY;
			int yySrc = y * src_pitchY;
			int xx = x;
			dstpY[yy + xx] = (Y * a + srcpY[yySrc + xx] * (255 - a)) / 255;
		}
	}
	// UV
	for (int h = 0; h < height; h += 2) {
		y = h >> 1;
		for (int w = 0; w < width; w += 2) {
			x = w >> 1;
			int w4 = w << 2;
			int yy1 = h * tcasPitch;
			int yy2 = (h + 1) * tcasPitch;
			tcas_byte r11, g11, b11, a11, r12, g12, b12, a12, r13, g13, b13, a13;
			tcas_byte r21, g21, b21, a21, r22, g22, b22, a22, r23, g23, b23, a23;
			if (0 == w) {
				r11 = tcasFrameBuf[yy1 + w4];
				g11 = tcasFrameBuf[yy1 + w4 + 1];
				b11 = tcasFrameBuf[yy1 + w4 + 2];
				a11 = tcasFrameBuf[yy1 + w4 + 3];
				r21 = tcasFrameBuf[yy2 + w4];
				g21 = tcasFrameBuf[yy2 + w4 + 1];
				b21 = tcasFrameBuf[yy2 + w4 + 2];
				a21 = tcasFrameBuf[yy2 + w4 + 3];
			}
			else {
				r11 = tcasFrameBuf[yy1 + w4 - 4];
				g11 = tcasFrameBuf[yy1 + w4 - 3];
				b11 = tcasFrameBuf[yy1 + w4 - 2];
				a11 = tcasFrameBuf[yy1 + w4 - 1];
				r21 = tcasFrameBuf[yy2 + w4 - 4];
				g21 = tcasFrameBuf[yy2 + w4 - 3];
				b21 = tcasFrameBuf[yy2 + w4 - 2];
				a21 = tcasFrameBuf[yy2 + w4 - 1];
			}
			r12 = tcasFrameBuf[yy1 + w4];
			g12 = tcasFrameBuf[yy1 + w4 + 1];
			b12 = tcasFrameBuf[yy1 + w4 + 2];
			a12 = tcasFrameBuf[yy1 + w4 + 3];
			r22 = tcasFrameBuf[yy2 + w4];
			g22 = tcasFrameBuf[yy2 + w4 + 1];
			b22 = tcasFrameBuf[yy2 + w4 + 2];
			a22 = tcasFrameBuf[yy2 + w4 + 3];
			if (width - 2 == w) {
				r13 = tcasFrameBuf[yy1 + w4];
				g13 = tcasFrameBuf[yy1 + w4 + 1];
				b13 = tcasFrameBuf[yy1 + w4 + 2];
				a13 = tcasFrameBuf[yy1 + w4 + 3];
				r23 = tcasFrameBuf[yy2 + w4];
				g23 = tcasFrameBuf[yy2 + w4 + 1];
				b23 = tcasFrameBuf[yy2 + w4 + 2];
				a23 = tcasFrameBuf[yy2 + w4 + 3];
			}
			else {
				r13 = tcasFrameBuf[yy1 + w4 + 4];
				g13 = tcasFrameBuf[yy1 + w4 + 5];
				b13 = tcasFrameBuf[yy1 + w4 + 6];
				a13 = tcasFrameBuf[yy1 + w4 + 7];
				r23 = tcasFrameBuf[yy2 + w4 + 4];
				g23 = tcasFrameBuf[yy2 + w4 + 5];
				b23 = tcasFrameBuf[yy2 + w4 + 6];
				a23 = tcasFrameBuf[yy2 + w4 + 7];
			}
			if (0 == a11 && 0 == a12 && 0 == a13 && 0 == a21 && 0 == a22 && 0 == a23) continue;
			// V
			tcas_byte V11 = ((112 * r11 - 94 * g11 - 18 * b11 + 128) / 256) + 128;
			tcas_byte V12 = ((112 * r12 - 94 * g12 - 18 * b12 + 128) / 256) + 128;
			tcas_byte V13 = ((112 * r13 - 94 * g13 - 18 * b13 + 128) / 256) + 128;
			tcas_byte V21 = ((112 * r21 - 94 * g21 - 18 * b21 + 128) / 256) + 128;
			tcas_byte V22 = ((112 * r22 - 94 * g22 - 18 * b22 + 128) / 256) + 128;
			tcas_byte V23 = ((112 * r23 - 94 * g23 - 18 * b23 + 128) / 256) + 128;
			tcas_byte V1 = (V11 + (V12 << 1) + V13) >> 2;
			tcas_byte V2 = (V21 + (V22 << 1) + V23) >> 2;
			tcas_byte V = (V1 + V2) >> 1;   // progressvie
			// U
			tcas_byte U11 = ((-38 * r11 - 74 * g11 + 112 * b11 + 128) / 256) + 128;
			tcas_byte U12 = ((-38 * r12 - 74 * g12 + 112 * b12 + 128) / 256) + 128;
			tcas_byte U13 = ((-38 * r13 - 74 * g13 + 112 * b13 + 128) / 256) + 128;
			tcas_byte U21 = ((-38 * r21 - 74 * g21 + 112 * b21 + 128) / 256) + 128;
			tcas_byte U22 = ((-38 * r22 - 74 * g22 + 112 * b22 + 128) / 256) + 128;
			tcas_byte U23 = ((-38 * r23 - 74 * g23 + 112 * b23 + 128) / 256) + 128;
			tcas_byte U1 = (U11 + (U12 << 1) + U13) >> 2;
			tcas_byte U2 = (U21 + (U22 << 1) + U23) >> 2;
			tcas_byte U = (U1 + U2) >> 1;
			// A
			tcas_byte A1 = (a11 + (a12 << 1) + a13) >> 2;
			tcas_byte A2 = (a21 + (a22 << 1) + a23) >> 2;
			tcas_byte A = (A1 + A2) >> 1;
			// Blend
			int yy = y * dst_pitchUV;
			int yySrc = y * src_pitchUV;
			int xx = x;
			dstpV[yy + xx] = (V * A + srcpV[yySrc + xx] * (255 - A)) / 255;
			dstpU[yy + xx] = (U * A + srcpU[yySrc + xx] * (255 - A)) / 255;
		}
	}
	delete[] tcasFrameBuf;
}

void vsfilterRenderYUV2(const VSFrameRef *pSrc, VSFrameRef *pDst, int n, VSFilterData *d, const VSAPI *vsapi) {
	tcas_u16  x, y;
	tcas_byte r, g, b, a;
	const tcas_byte *srcp = vsapi->getReadPtr(pSrc, 0);
	tcas_byte *dstp = vsapi->getWritePtr(pDst, 0);
	int srcPitch = vsapi->getStride(pSrc, 0);
	int dstPitch = vsapi->getStride(pDst, 0);
	int width = vsapi->getFrameWidth(pDst, 0) >> 1;
	int height = vsapi->getFrameHeight(pDst, 0);

#if 0
	// copy data from source video frame to target video frame
	if (dstPitch == srcPitch) ::memcpy(dstp, srcp, dstPitch * height);
	else {
		for (int h = 0; h < height; h++) {
			int yy = h * dstPitch;
			int yySrc = h * srcPitch;
			for (int w = 0; w < width; w++) {
				int xx = w << 2;
				dstp[yy + xx] = srcp[yySrc + xx];
				dstp[yy + xx + 1] = srcp[yySrc + xx + 1];
				dstp[yy + xx + 2] = srcp[yySrc + xx + 2];
				dstp[yy + xx + 3] = srcp[yySrc + xx + 3];
			}
		}
	}
#endif
	// Create TCAS frame
	tcas_byte *tcasFrameBuf;
	if (d->bUseFrameCache)
		while (!::libtcas_frame_cache_get(&d->frameCache, n, &tcasFrameBuf)) continue;
	else
		::libtcas_framer_get(&d->framer, n, &tcasFrameBuf);
	// Render
	int tcasPitch = (width << 2) * sizeof(tcas_byte);
	// Y
	for (int h = 0; h < height; h++) {
		y = h;
		for (int w = 0; w < width; w++) {
			x = w;
			int w4 = w << 2;
			int yy = h * tcasPitch;
			r = tcasFrameBuf[yy + w4];
			g = tcasFrameBuf[yy + w4 + 1];
			b = tcasFrameBuf[yy + w4 + 2];
			a = tcasFrameBuf[yy + w4 + 3];
			if (0 == a) continue;
			tcas_byte Y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			yy = y * dstPitch;
			int yySrc = y * srcPitch;
			int xx = x << 1;
			dstp[yy + xx] = (Y * a + srcp[yySrc + xx] * (255 - a)) / 255;
		}
	}
	// UV
	for (int h = 0; h < height; h++) {
		y = h;
		for (int w = 0; w < width; w += 2) {
			x = w;
			int w4 = w << 2;
			int yy = h * tcasPitch;
			tcas_byte r1, g1, b1, a1;
			tcas_byte r2, g2, b2, a2;
			tcas_byte r3, g3, b3, a3;
			if (0 == w) {
				r1 = tcasFrameBuf[yy + w4];
				g1 = tcasFrameBuf[yy + w4 + 1];
				b1 = tcasFrameBuf[yy + w4 + 2];
				a1 = tcasFrameBuf[yy + w4 + 3];
			}
			else {
				r1 = tcasFrameBuf[yy + w4 - 4];
				g1 = tcasFrameBuf[yy + w4 - 3];
				b1 = tcasFrameBuf[yy + w4 - 2];
				a1 = tcasFrameBuf[yy + w4 - 1];
			}
			r2 = tcasFrameBuf[yy + w4];
			g2 = tcasFrameBuf[yy + w4 + 1];
			b2 = tcasFrameBuf[yy + w4 + 2];
			a2 = tcasFrameBuf[yy + w4 + 3];
			if (width - 2 == w) {
				r3 = tcasFrameBuf[yy + w4];
				g3 = tcasFrameBuf[yy + w4 + 1];
				b3 = tcasFrameBuf[yy + w4 + 2];
				a3 = tcasFrameBuf[yy + w4 + 3];
			}
			else {
				r3 = tcasFrameBuf[yy + w4 + 4];
				g3 = tcasFrameBuf[yy + w4 + 5];
				b3 = tcasFrameBuf[yy + w4 + 6];
				a3 = tcasFrameBuf[yy + w4 + 7];
			}
			if (0 == a1 && 0 == a2 && 0 == a3) continue;
			// V
			tcas_byte V1 = ((112 * r1 - 94 * g1 - 18 * b1 + 128) / 256) + 128;
			tcas_byte V2 = ((112 * r2 - 94 * g2 - 18 * b2 + 128) / 256) + 128;
			tcas_byte V3 = ((112 * r3 - 94 * g3 - 18 * b3 + 128) / 256) + 128;
			tcas_byte V = (V1 + (V2 << 1) + V3) >> 2;
			// U
			tcas_byte U1 = ((-38 * r1 - 74 * g1 + 112 * b1 + 128) / 256) + 128;
			tcas_byte U2 = ((-38 * r2 - 74 * g2 + 112 * b2 + 128) / 256) + 128;
			tcas_byte U3 = ((-38 * r3 - 74 * g3 + 112 * b3 + 128) / 256) + 128;
			tcas_byte U = (U1 + (U2 << 1) + U3) >> 2;
			// A
			tcas_byte A = (a1 + (a2 << 1) + a3) >> 2;
			// Blend
			yy = y * dstPitch;
			int yySrc = y * srcPitch;
			int xx = x << 1;
			dstp[yy + xx + 1] = (U * A + srcp[yySrc + xx + 1] * (255 - A)) / 255;
			dstp[yy + xx + 3] = (V * A + srcp[yySrc + xx + 3] * (255 - A)) / 255;
		}
	}
	delete[] tcasFrameBuf;
}

void vsfilterRenderRGB24(const VSFrameRef *pSrc, VSFrameRef *pDst, int n, VSFilterData *d, const VSAPI *vsapi) {
	tcas_u16  x, y;
	tcas_byte r, g, b, a;
	const tcas_byte *srcp = vsapi->getReadPtr(pSrc, 0);
	tcas_byte *dstp = vsapi->getWritePtr(pDst, 0);
	int srcPitch = vsapi->getStride(pSrc, 0);
	int dstPitch = vsapi->getStride(pDst, 0);
	int width = vsapi->getFrameWidth(pDst, 0) / 3;
	int height = vsapi->getFrameHeight(pDst, 0);
	
#if 0
	// copy data from source video frame to target video frame
	if (dstPitch == srcPitch) ::memcpy(dstp, srcp, dstPitch * height);
	else {
		for (int h = 0; h < height; h++) {
			int yy = h * dstPitch;
			int yySrc = h * srcPitch;
			for (int w = 0; w < width; w++) {
				int xx = 3 * w;
				dstp[yy + xx] = srcp[yySrc + xx];
				dstp[yy + xx + 1] = srcp[yySrc + xx + 1];
				dstp[yy + xx + 2] = srcp[yySrc + xx + 2];
			}
		}
	}
#endif
	// Create TCAS frame
	tcas_byte *tcasFrameBuf;
	if (d->bUseFrameCache)
		while (!::libtcas_frame_cache_get(&d->frameCache, n, &tcasFrameBuf)) continue;
	else
		::libtcas_framer_get(&d->framer, n, &tcasFrameBuf);
	// Render
	int tcasPitch = (width << 2) * sizeof(tcas_byte);
	for (int h = 0; h < height; h++) {
		y = height - h - 1;
		for (int w = 0; w < width; w++) {
			x = w;
			int w4 = w << 2;
			int yy = h * tcasPitch;
			r = tcasFrameBuf[yy + w4];
			g = tcasFrameBuf[yy + w4 + 1];
			b = tcasFrameBuf[yy + w4 + 2];
			a = tcasFrameBuf[yy + w4 + 3];
			if (0 == a) continue;
			yy = y * dstPitch;
			int yySrc = y * srcPitch;
			int xx = x * 3;
			dstp[yy + xx] = (b * a + srcp[yySrc + xx] * (255 - a)) / 255;
			dstp[yy + xx + 1] = (g * a + srcp[yySrc + xx + 1] * (255 - a)) / 255;
			dstp[yy + xx + 2] = (r * a + srcp[yySrc + xx + 2] * (255 - a)) / 255;
		}
	}
	delete[] tcasFrameBuf;
}

static void VS_CC vsfilterFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
	VSFilterData *d = (VSFilterData *)instanceData;
	if (d->bUseFrameCache)
		::libtcas_frame_cache_fin(&d->frameCache);
	else
		::libtcas_framer_fin(&d->framer);
    vsapi->freeNode(d->node);
	delete d;
}

static void VS_CC vsfilterCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
	std::unique_ptr<VSFilterData> ud(new VSFilterData{});
	VSFilterData &d = *ud;
    int err;

    d.node = vsapi->propGetNode(in, "clip", 0, nullptr);
    d.vi = vsapi->getVideoInfo(d.node);

	if (!isConstantFormat(d.vi) || d.vi->format->id != pfYUV420P8) {
        vsapi->setError(out, "TcasFilter: only YUV420P8 input supported.");
        vsapi->freeNode(d.node);
        return;
    }

	const char * filename = vsapi->propGetData(in, "file", 0, nullptr);

	if (!filename)
		filename = "";
	d.filename = filename;
	//::GetFullPathNameA(filename, MAX_PATH, d.filename, nullptr);
	
	double fps = vsapi->propGetFloat(in, "fps", 0, &err);

	if (err)
		fps = -1.f;

	if (fps < 0.0) {        // use TCAS default frame rate
		d.fpsNum = (tcas_u32)(-1);
		d.fpsDen = 1;
	}
	else if (0.0 == fps) {
		d.fpsNum = static_cast<tcas_u32>(d.vi->fpsNum);
		d.fpsDen = static_cast<tcas_u32>(d.vi->fpsDen);
	}
	else {
		d.fpsDen = 100000;
		d.fpsNum = (tcas_u32)(fps * d.fpsDen);
		tcas_u32 x = d.fpsNum;
		tcas_u32 y = d.fpsDen;
		tcas_u32 t;
		while (y) {   // find gcd
			t = x % y;
			x = y;
			y = t;
		}
		d.fpsNum /= x;
		d.fpsDen /= x;
	}

	int maxFrameCount = int64ToIntS(vsapi->propGetInt(in, "max_frame", 0, &err));
	if (err)
		maxFrameCount = -1;

	if (maxFrameCount > 0)
		d.bUseFrameCache = TRUE;
	else
		d.bUseFrameCache = FALSE;

	if (d.bUseFrameCache) {
		if (::libtcas_frame_cache_init(d.filename, d.fpsNum, d.fpsDen, d.vi->width, d.vi->height, maxFrameCount, &d.frameCache) != tcas_error_success) {
			vsapi->setError(out, "TcasFilter: failed to initialize the frame cache caused by access failure to the TCAS file.");
			vsapi->freeNode(d.node);
			return;
		}
		d.minFrame = d.frameCache.minFrame;
		d.maxFrame = d.frameCache.maxFrame;
		::libtcas_frame_cache_run(&d.frameCache);
	}
	else {
		if (::libtcas_framer_init(d.filename, d.fpsNum, d.fpsDen, d.vi->width, d.vi->height, &d.framer) != tcas_error_success) {
			vsapi->setError(out, "TcasFilter: failed to initialize the frame cache caused by access failure to the TCAS file.");
			vsapi->freeNode(d.node);
			return;
		}
		d.minFrame = d.framer.minFrame;
		d.maxFrame = d.framer.maxFrame;
	}

    vsapi->createFilter(in, out, static_cast<const char *>(userData), invertInit, vsfilterGetFrame, vsfilterFree, fmParallel, 0, ud.release(), core);
}

VS_EXTERNAL_API(void) VapourSynthPluginInit(VSConfigPlugin configFunc, VSRegisterFunction registerFunc, VSPlugin *plugin) {
    configFunc("github.com.emako", "tcax", "VapourSynth TCAS Filter", VAPOURSYNTH_API_VERSION, 1, plugin);
    registerFunc("TcasSub", 
				 "clip:clip;"
				 "file:data;"
				 "fps:float:opt;"
				 "max_frame:int:opt;",
				 vsfilterCreate, "TcasSub", plugin);
}
