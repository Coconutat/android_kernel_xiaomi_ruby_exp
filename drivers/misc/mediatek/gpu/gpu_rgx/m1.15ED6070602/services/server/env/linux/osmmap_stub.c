/*************************************************************************/ /*!
@File
@Title          Device Memory Management
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    OS abstraction for the mmap2 interface for mapping PMRs into
                User Mode memory
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

/* our exported API */
#include "osmmap.h"

/* include/ */
#include "img_types.h"
#include "img_defs.h"
#include "pvr_debug.h"
#include "pvrsrv_error.h"

/* services/include/ */

/* services/include/srvhelper/ */
#include "ra.h"

#include "pmr.h"

PVRSRV_ERROR
OSMMapPMR(IMG_HANDLE hBridge,
          IMG_HANDLE hPMR,
          IMG_DEVMEM_SIZE_T uiPMRSize,
          PVRSRV_MEMALLOCFLAGS_T uiFlags,
          IMG_HANDLE *phOSMMapPrivDataOut,
          void **ppvMappingAddressOut,
          size_t *puiMappingLengthOut)
{
    PVRSRV_ERROR eError;
    PMR *psPMR;
    void *pvKernelAddress;
    size_t uiLength;
    IMG_HANDLE hPriv;

    PVR_UNREFERENCED_PARAMETER(hBridge);
    PVR_UNREFERENCED_PARAMETER(uiFlags);

    /*
      Normally this function would mmap a PMR into the memory space of
      user process, but in this case we're taking a PMR and mapping it
      into kernel virtual space.  We keep the same function name for
      symmetry as this allows the higher layers of the software stack
      to not care whether they are user mode or kernel
    */

    psPMR = hPMR;

    if (PMR_IsSparse(psPMR))
    {
        eError = PMRAcquireSparseKernelMappingData(psPMR,
                                            0,
                                            0,
                                            &pvKernelAddress,
                                            &uiLength,
                                            &hPriv);
    }
    else
    {
        eError = PMRAcquireKernelMappingData(psPMR,
                                            0,
                                            0,
                                            &pvKernelAddress,
                                            &uiLength,
                                            &hPriv);
    }
    if (eError != PVRSRV_OK)
    {
        goto e0;
    }

    *phOSMMapPrivDataOut = hPriv;
    *ppvMappingAddressOut = pvKernelAddress;
    *puiMappingLengthOut = uiLength;

    /* MappingLength might be rounded up to page size */
    PVR_ASSERT(*puiMappingLengthOut >= uiPMRSize);

    return PVRSRV_OK;

    /*
      error exit paths follow
    */

e0:
    PVR_ASSERT(eError != PVRSRV_OK);
    return eError;
}

void
OSMUnmapPMR(IMG_HANDLE hBridge,
            IMG_HANDLE hPMR,
            IMG_HANDLE hOSMMapPrivData,
            void *pvMappingAddress,
            size_t uiMappingLength)
{
    PMR *psPMR;

    PVR_UNREFERENCED_PARAMETER(hBridge);
    PVR_UNREFERENCED_PARAMETER(pvMappingAddress);
    PVR_UNREFERENCED_PARAMETER(uiMappingLength);

    psPMR = hPMR;
    PMRReleaseKernelMappingData(psPMR,
                                hOSMMapPrivData);
}
