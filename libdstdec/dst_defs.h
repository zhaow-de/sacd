/*
    MPEG-4 Audio RM Module
    Lossless coding of 1-bit oversampled audio - DST (Direct Stream Transfer)

    This software was originally developed by:

    * Aad Rijnberg
    Philips Digital Systems Laboratories Eindhoven
    <aad.rijnberg@philips.com>

    * Fons Bruekers
    Philips Research Laboratories Eindhoven
    <fons.bruekers@philips.com>

    * Eric Knapen
    Philips Digital Systems Laboratories Eindhoven
    <h.w.m.knapen@philips.com>

    And edited by:

    * Richard Theelen
    Philips Digital Systems Laboratories Eindhoven
    <r.h.m.theelen@philips.com>

    * Maxim V.Anisiutkin
    <maxim.anisiutkin@gmail.com>

    * Robert Tari
    <robert.tari@gmail.com>

    in the course of development of the MPEG-4 Audio standard ISO-14496-1, 2 and 3.
    This software module is an implementation of a part of one or more MPEG-4 Audio
    tools as specified by the MPEG-4 Audio standard. ISO/IEC gives users of the
    MPEG-4 Audio standards free licence to this software module or modifications
    thereof for use in hardware or software products claiming conformance to the
    MPEG-4 Audio standards. Those intending to use this software module in hardware
    or software products are advised that this use may infringe existing patents.
    The original developers of this software of this module and their company,
    the subsequent editors and their companies, and ISO/EIC have no liability for
    use of this software module or modifications thereof in an implementation.
    Copyright is not released for non MPEG-4 Audio conforming products. The
    original developer retains full right to use this code for his/her own purpose,
    assign or donate the code to a third party and to inhibit third party from
    using the code for non MPEG-4 Audio conforming products. This copyright notice
    must be included in all copies of derivative works.

    Copyright � 2004-2016.

    This file is part of SACD.

    SACD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SACD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SACD.  If not, see <http://www.gnu.org/licenses/gpl-3.0.txt>.
*/

#ifndef __DST_DEFS_H__
#define __DST_DEFS_H__

#include <memory.h>
#include <cstdint>
#include "dst_consts.h"

#ifndef MIN
#define MIN(a, b) (((a)<(b))?(a):(b))
#endif  // MIN

#ifndef MAX
#define MAX(a, b) (((a)>(b))?(a):(b))
#endif  // MAX

#define GET_BIT(BitBase, BitIndex) ((((unsigned char*)(BitBase))[(BitIndex) >> 3] >> (7 - ((BitIndex) & 7))) & 1)
#define GET_NIBBLE(NibbleBase, NibbleIndex) ((((unsigned char*)(NibbleBase))[(NibbleIndex) >> 1] >> (((NibbleIndex) & 1) << 2)) & 0x0f)
#define dst_memcpy(dst, src, size) ::memcpy(dst, src, size)
#define dst_memset(dst, val, size) ::memset(dst, val, size)

enum ETTable
{
    T_FILTER, T_PTABLE
};

class CSegment
{
public:
    int Resolution; // Resolution for segments
    int SegmentLen[MAX_CHANNELS][MAXNROF_SEGS]; // SegmentLen[ChNr][SegmentNr]
    int NrOfSegments[MAX_CHANNELS]; // NrOfSegments[ChNr]
    int Table4Segment[MAX_CHANNELS][MAXNROF_SEGS]; // Table4Segment[ChNr][SegmentNr]
};

class CFrameHeader
{
public:
    int FrameNr; // Nr of frame that is currently processed
    int NrOfChannels; // Number of channels in the recording
    int NrOfFilters; // Number of filters used for this frame
    int NrOfPtables; // Number of Ptables used for this frame
    int PredOrder[2 * MAX_CHANNELS]; // Prediction order used for this frame
    int PtableLen[2 * MAX_CHANNELS]; // Nr of Ptable entries used for this frame
    int16_t ICoefA[2 * MAX_CHANNELS][1 << SIZE_CODEDPREDORDER]; // Integer coefs for actual coding
    int DSTCoded; // 1=DST coded is put in DST stream, 0=DSD is put in DST stream
    size_t CalcNrOfBytes; // Contains number of bytes of the complete
    long CalcNrOfBits; // Contains number of bits of the complete channel stream after arithmetic encoding (also containing bytestuff-, ICoefA-bits, etc.)
    int HalfProb[MAX_CHANNELS]; // Defines per channel which probability is applied for the first PredOrder[] bits of a frame (0 = use Ptable entry, 1 = 128)
    int NrOfHalfBits[MAX_CHANNELS]; // Defines per channel how many bits at the start of each frame are optionally coded with p=0.5
    CSegment FSeg; // Contains segmentation data for filters
    uint8_t Filter4Bit[MAX_CHANNELS][MAX_DSDBITS_INFRAME / 2]; // Filter4Bit[ChNr][BitNr]
    CSegment PSeg; // Contains segmentation data for Ptables
    uint8_t Ptable4Bit[MAX_CHANNELS][MAX_DSDBITS_INFRAME / 2]; // Ptable4Bit[ChNr][BitNr]
    int PSameSegAsF; // 1 if segmentation is equal for F and P
    int PSameMapAsF; // 1 if mapping is equal for F and P
    int FSameSegAllCh; // 1 if all channels have same Filtersegm.
    int FSameMapAllCh; // 1 if all channels have same Filtermap
    int PSameSegAllCh; // 1 if all channels have same Ptablesegm.
    int PSameMapAllCh; // 1 if all channels have same Ptablemap
    int MaxNrOfFilters; // Max. nr. of filters allowed per frame
    int MaxNrOfPtables; // Max. nr. of Ptables allowed per frame
    size_t MaxFrameLen; // Max frame length of this file
    size_t NrOfBitsPerCh; // MaxFrameLen * RESOL
};

typedef uint8_t ADataByte;

#endif  // __DST_DEFS_H__
