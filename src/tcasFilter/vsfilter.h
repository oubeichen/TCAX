#ifndef VSFILTER_H
#define VSFILTER_H
#pragma once

#include <string>
#include <stdlib.h>
#include <atlpath.h>
#include <atlconv.h>
#include <iostream>
#include "vapoursynth/VapourSynth.h"
#include "vapoursynth/VSHelper.h"
#include "../libtcas/tcas/hla_frame_cache.h"
#include "../libtcas/tcas/hla_framer.h"

using namespace std;

typedef struct {
	VSNodeRef *node;
	const VSVideoInfo *vi;
	char filename[MAX_PATH];
	bool bUseFrameCache;      // use m_framer or m_frameCache
	TCAS_Framer framer;
	TCAS_FrameCache frameCache;
	tcas_u32 fpsNum;
	tcas_u32 fpsDen;
	tcas_s32 minFrame;
	tcas_s32 maxFrame;
} VSFilterData;

typedef enum {
	VSPLANAR_YUV = 0,
	VSPLANAR_Y = 0,
	VSPLANAR_U = 1,
	VSPLANAR_V = 2,
	VSPLANAR_RGB = 0,
	VSPLANAR_R = 0,
	VSPLANAR_G = 1,
	VSPLANAR_B = 2,
} VSPLANAR;

void vsfilterRenderYV12(const VSFrameRef *pSrc, VSFrameRef *pDst, int n, VSFilterData *d, const VSAPI *vsapi);
void vsfilterRenderYUV2(const VSFrameRef *pSrc, VSFrameRef *pDst, int n, VSFilterData *d, const VSAPI *vsapi);
void vsfilterRenderRGB24(const VSFrameRef *pSrc, VSFrameRef *pDst, int n, VSFilterData *d, const VSAPI *vsapi);

#endif    /* VSFILTER_H */