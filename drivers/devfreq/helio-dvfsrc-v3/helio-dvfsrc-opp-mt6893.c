/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/

#include <linux/kernel.h>
#include <mt-plat/mtk_devinfo.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <helio-dvfsrc-opp.h>
#include <helio-dvfsrc-qos.h>

#ifdef CONFIG_MEDIATEK_DRAMC
#include <dramc.h>
#endif

#define V_VMODE_SHIFT 0
/*#define V_CT_SHIFT 5*/
#define V_CT_TEST_SHIFT 6
#define V_CT_V2_SHIFT 7

static int dvfsrc_rsrv;

#ifndef CONFIG_MEDIATEK_DRAMC
static int mtk_dramc_get_steps_freq(unsigned int step)
{
	pr_info("get dram steps_freq fail\n");
	return 4266;
}
#endif


void dvfsrc_opp_level_mapping(void)
{
	set_vcore_opp(VCORE_DVFS_OPP_0, VCORE_OPP_0);
	set_vcore_opp(VCORE_DVFS_OPP_1, VCORE_OPP_0);
	set_vcore_opp(VCORE_DVFS_OPP_2, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_3, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_4, VCORE_OPP_2);
	set_vcore_opp(VCORE_DVFS_OPP_5, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_6, VCORE_OPP_2);
	set_vcore_opp(VCORE_DVFS_OPP_7, VCORE_OPP_3);
	set_vcore_opp(VCORE_DVFS_OPP_8, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_9, VCORE_OPP_2);
	set_vcore_opp(VCORE_DVFS_OPP_10, VCORE_OPP_3);
	set_vcore_opp(VCORE_DVFS_OPP_11, VCORE_OPP_4);
	set_vcore_opp(VCORE_DVFS_OPP_12, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_13, VCORE_OPP_2);
	set_vcore_opp(VCORE_DVFS_OPP_14, VCORE_OPP_3);
	set_vcore_opp(VCORE_DVFS_OPP_15, VCORE_OPP_4);
	set_vcore_opp(VCORE_DVFS_OPP_16, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_17, VCORE_OPP_2);
	set_vcore_opp(VCORE_DVFS_OPP_18, VCORE_OPP_3);
	set_vcore_opp(VCORE_DVFS_OPP_19, VCORE_OPP_4);
	set_vcore_opp(VCORE_DVFS_OPP_20, VCORE_OPP_1);
	set_vcore_opp(VCORE_DVFS_OPP_21, VCORE_OPP_2);
	set_vcore_opp(VCORE_DVFS_OPP_22, VCORE_OPP_3);
	set_vcore_opp(VCORE_DVFS_OPP_23, VCORE_OPP_4);

	set_ddr_opp(VCORE_DVFS_OPP_0, DDR_OPP_0);
	set_ddr_opp(VCORE_DVFS_OPP_1, DDR_OPP_1);
	set_ddr_opp(VCORE_DVFS_OPP_2, DDR_OPP_1);
	set_ddr_opp(VCORE_DVFS_OPP_3, DDR_OPP_2);
	set_ddr_opp(VCORE_DVFS_OPP_4, DDR_OPP_2);
	set_ddr_opp(VCORE_DVFS_OPP_5, DDR_OPP_3);
	set_ddr_opp(VCORE_DVFS_OPP_6, DDR_OPP_3);
	set_ddr_opp(VCORE_DVFS_OPP_7, DDR_OPP_3);
	set_ddr_opp(VCORE_DVFS_OPP_8, DDR_OPP_4);
	set_ddr_opp(VCORE_DVFS_OPP_9, DDR_OPP_4);
	set_ddr_opp(VCORE_DVFS_OPP_10, DDR_OPP_4);
	set_ddr_opp(VCORE_DVFS_OPP_11, DDR_OPP_4);
	set_ddr_opp(VCORE_DVFS_OPP_12, DDR_OPP_5);
	set_ddr_opp(VCORE_DVFS_OPP_13, DDR_OPP_5);
	set_ddr_opp(VCORE_DVFS_OPP_14, DDR_OPP_5);
	set_ddr_opp(VCORE_DVFS_OPP_15, DDR_OPP_5);
	set_ddr_opp(VCORE_DVFS_OPP_16, DDR_OPP_6);
	set_ddr_opp(VCORE_DVFS_OPP_17, DDR_OPP_6);
	set_ddr_opp(VCORE_DVFS_OPP_18, DDR_OPP_6);
	set_ddr_opp(VCORE_DVFS_OPP_19, DDR_OPP_6);
	set_ddr_opp(VCORE_DVFS_OPP_20, DDR_OPP_7);
	set_ddr_opp(VCORE_DVFS_OPP_21, DDR_OPP_7);
	set_ddr_opp(VCORE_DVFS_OPP_22, DDR_OPP_7);
	set_ddr_opp(VCORE_DVFS_OPP_23, DDR_OPP_7);
}

void dvfsrc_opp_table_init(void)
{
	int i;
	int vcore_opp, ddr_opp;

	for (i = 0; i < VCORE_DVFS_OPP_NUM; i++) {
		vcore_opp = get_vcore_opp(i);
		ddr_opp = get_ddr_opp(i);

		if (vcore_opp == VCORE_OPP_UNREQ || ddr_opp == DDR_OPP_UNREQ) {
			set_opp_table(i, 0, 0);
			continue;
		}
		set_opp_table(i, get_vcore_uv_table(vcore_opp),
		mtk_dramc_get_steps_freq(ddr_opp) * 1000);
	}
}

static int query_rising_idx(void)
{
	int idx;
	int ptpod = get_devinfo_with_index(134);

	pr_info("%s: PTPOD: 0x%x\n", __func__, ptpod);
	idx = ptpod & 0x7;

	return idx;
}

static int get_vb_volt(int vcore_opp, int info_mode)
{
	int ret = 0;
	bool ct_test = false;
	u32 ptpod = get_devinfo_with_index(69);

	pr_info("%s: PTPOD: 0x%x\n", __func__, ptpod);
	if (info_mode & (1 << V_CT_TEST_SHIFT))
		ct_test = true;

	if (vcore_opp == VCORE_OPP_0) {
		ptpod = (ptpod >> 8) & 0xF;
			ret = (ptpod <= 3) ? ptpod : 3;
		if (!ct_test && (ret > 0))
			ret = ret - 1;
	}
	pr_info("%s: OPP = %d %d %d\n", __func__, vcore_opp, ptpod, ret);

	return ret * 12500;
}


static int __init dvfsrc_opp_init(void)
{
	struct device_node *dvfsrc_node = NULL;
	int vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv;
	int vcore_opp_4_uv;
	int is_vcore_ct = 0;
	int dvfs_v_mode = 0;
	int rising_idx = 0;
	void __iomem *dvfsrc_base;

	set_pwrap_cmd(VCORE_OPP_0, 16);
	set_pwrap_cmd(VCORE_OPP_1, 17);
	set_pwrap_cmd(VCORE_OPP_2, 18);
	set_pwrap_cmd(VCORE_OPP_3, 19);
	set_pwrap_cmd(VCORE_OPP_4, 20);

	vcore_opp_0_uv = 750000;
	vcore_opp_1_uv = 725000;
	vcore_opp_2_uv = 650000;
	vcore_opp_3_uv = 600000;
	vcore_opp_4_uv = 575000;

	dvfsrc_node =
		of_find_compatible_node(NULL, NULL, "mediatek,dvfsrc");

	/* For Doe */
	if (dvfsrc_node) {
		dvfsrc_base = of_iomap(dvfsrc_node, 0);
		if (dvfsrc_base) {
			dvfsrc_rsrv = readl(dvfsrc_base + 0x610);
			iounmap(dvfsrc_base);
		}
		pr_info("%s: vcore_arg = %08x\n",
			__func__, dvfsrc_rsrv);
		dvfs_v_mode = (dvfsrc_rsrv >> V_VMODE_SHIFT) & 0x3;
		is_vcore_ct = (dvfsrc_rsrv >> V_CT_V2_SHIFT) & 0x1;
	}

	rising_idx = query_rising_idx();
	if (rising_idx > 2)
		rising_idx = 2;

	vcore_opp_2_uv = vcore_opp_2_uv + rising_idx * 25000;
	vcore_opp_3_uv = vcore_opp_3_uv + rising_idx * 25000;
	vcore_opp_4_uv = vcore_opp_4_uv + rising_idx * 25000;

	if (is_vcore_ct && (rising_idx == 0)) {
			vcore_opp_0_uv -= get_vb_volt(VCORE_OPP_0, dvfsrc_rsrv);
			vcore_opp_1_uv = min(vcore_opp_0_uv, vcore_opp_1_uv);
	}

	if (dvfs_v_mode == 3) {
		/* LV */
		vcore_opp_0_uv = rounddown((vcore_opp_0_uv * 95) / 100, 6250);
		vcore_opp_1_uv = rounddown((vcore_opp_1_uv * 95) / 100, 6250);
		vcore_opp_2_uv = rounddown((vcore_opp_2_uv * 95) / 100, 6250);
		vcore_opp_3_uv = rounddown((vcore_opp_3_uv * 95) / 100, 6250);
		vcore_opp_4_uv = rounddown((vcore_opp_4_uv * 95) / 100, 6250);
	} else if (dvfs_v_mode == 1) {
		/* HV */
		vcore_opp_0_uv = roundup((vcore_opp_0_uv * 105) / 100, 6250);
		vcore_opp_1_uv = roundup((vcore_opp_1_uv * 105) / 100, 6250);
		vcore_opp_2_uv = roundup((vcore_opp_2_uv * 105) / 100, 6250);
		vcore_opp_3_uv = roundup((vcore_opp_3_uv * 105) / 100, 6250);
		vcore_opp_4_uv = roundup((vcore_opp_4_uv * 105) / 100, 6250);
	}

	pr_info("%s: VMODE=%d, RSV4=%x\n",
			__func__,
			dvfs_v_mode,
			dvfsrc_rsrv);

	pr_info("%s: FINAL vcore_opp_uv: %d, %d, %d, %d, %d\n",
		__func__,
		vcore_opp_0_uv,
		vcore_opp_1_uv,
		vcore_opp_2_uv,
		vcore_opp_3_uv,
		vcore_opp_4_uv);

	set_vcore_uv_table(VCORE_OPP_0, vcore_opp_0_uv);
	set_vcore_uv_table(VCORE_OPP_1, vcore_opp_1_uv);
	set_vcore_uv_table(VCORE_OPP_2, vcore_opp_2_uv);
	set_vcore_uv_table(VCORE_OPP_3, vcore_opp_3_uv);
	set_vcore_uv_table(VCORE_OPP_4, vcore_opp_4_uv);

	return 0;
}

fs_initcall_sync(dvfsrc_opp_init)

static int __init dvfsrc_dram_opp_init(void)
{
	int i;

	for (i = 0; i < DDR_OPP_NUM; i++) {
		set_opp_ddr_freq(i,
			mtk_dramc_get_steps_freq(i) * 1000);
	}

	return 0;
}

device_initcall_sync(dvfsrc_dram_opp_init)
