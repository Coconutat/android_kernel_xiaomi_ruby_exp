/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2016 MediaTek Inc.
 */

#ifndef __DFD_H__
#define __DFD_H__

#define DFD_SMC_MAGIC_SETUP (0x99716150)
#define DFD_CACHE_DUMP_ENABLE	1
#define DFD_PARITY_ERR_TRIGGER	2

struct dfd_drv {
	u64 base_addr;
	u32 base_addr_msb;
	unsigned long chain_length;
	unsigned long rg_dfd_timeout;
	unsigned int enabled;
	unsigned int cachedump_en;
	unsigned int mem_reserve;
	unsigned int l2c_trigger;
	unsigned int check_dfd_support;
	unsigned long dfd_infra_base;
	unsigned int dfd_ap_addr_offset;
};

extern int mtk_dbgtop_dfd_count_en(int value);
extern int mtk_dbgtop_dfd_therm1_dis(int value);
extern int mtk_dbgtop_dfd_therm2_dis(int value);
extern int mtk_dbgtop_dfd_timeout(int value);
extern void get_dfd_base(void __iomem *dfd_base, unsigned int latch_offset);
extern unsigned int check_dfd_support(void);
extern unsigned int dfd_infra_base(void);
extern unsigned int dfd_ap_addr_offset(void);

#endif
