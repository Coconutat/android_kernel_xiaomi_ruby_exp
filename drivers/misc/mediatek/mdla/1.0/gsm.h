/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2019 MediaTek Inc.
 */

#ifndef __GSM_H__
#define __GSM_H__

#include <linux/types.h>
#include <linux/sizes.h>
#include "mdla_ioctl.h"

#define GSM_SIZE        (SZ_1M)
#define GSM_MVA_BASE    (0x80000000)
#define GSM_MVA_INVALID (0xFFFFFFFF)

void *gsm_alloc(size_t size);
void *gsm_mva_to_virt(u32 mva);
int gsm_release(void *vaddr, size_t size);
int mdla_gsm_alloc(struct ioctl_malloc *malloc_data);
void mdla_gsm_free(struct ioctl_malloc *malloc_data);

#define is_gsm_mva(a) \
	(likely(((a) >= GSM_MVA_BASE) && ((a) < (GSM_MVA_BASE + SZ_1M))))

#endif /* KMOD_GSM_H_ */

