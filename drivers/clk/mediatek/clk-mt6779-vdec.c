// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Wendell Lin <wendell.lin@mediatek.com>
 */

#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "clk-mtk.h"
#include "clk-gate.h"

#include <dt-bindings/clock/mt6779-clk.h>

#define MT_CLKMGR_MODULE_INIT	0
#define CCF_SUBSYS_DEBUG		1

static const struct mtk_gate_regs vdec0_cg_regs = {
	.set_ofs = 0x0000,
	.clr_ofs = 0x0004,
	.sta_ofs = 0x0000,
};

static const struct mtk_gate_regs vdec1_cg_regs = {
	.set_ofs = 0x0008,
	.clr_ofs = 0x000c,
	.sta_ofs = 0x0008,
};

#define GATE_VDEC0(_id, _name, _parent, _shift) {	\
	.id = _id,				\
	.name = _name,				\
	.parent_name = _parent,			\
	.regs = &vdec0_cg_regs,			\
	.shift = _shift,			\
	.ops = &mtk_clk_gate_ops_setclr_inv,	\
}

#define GATE_VDEC1(_id, _name, _parent, _shift) {	\
	.id = _id,				\
	.name = _name,				\
	.parent_name = _parent,			\
	.regs = &vdec1_cg_regs,			\
	.shift = _shift,			\
	.ops = &mtk_clk_gate_ops_setclr_inv,	\
}

static const struct mtk_gate vdec_clks[] = {
	GATE_VDEC0(CLK_VDEC_VDEC, "vdec_cken", "vdec_sel", 0),
	GATE_VDEC1(CLK_VDEC_LARB1, "vdec_larb1_cken", "vdec_sel", 0),
};

static const struct of_device_id of_match_clk_mt6779_vdec[] = {
	//{ .compatible = "mediatek,mt6779-vcodec-dec", },
	{ .compatible = "mediatek,mt6779-vdecsys", },
	{}
};

static int clk_mt6779_vdec_probe(struct platform_device *pdev)
{
	struct clk_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
	int ret;

	clk_data = mtk_alloc_clk_data(CLK_VDEC_GCON_NR_CLK);
	if (!clk_data) {
		pr_notice("%s(): alloc clk data failed\n", __func__);
		return -ENOMEM;
	}

#if CCF_SUBSYS_DEBUG
	pr_info("%s(): clk data number: %d\n", __func__, clk_data->clk_num);
#endif

	mtk_clk_register_gates(node, vdec_clks, ARRAY_SIZE(vdec_clks),
			       clk_data);

	ret = of_clk_add_provider(node, of_clk_src_onecell_get, clk_data);

	if (ret) {
		pr_notice("%s(): could not register clock provider: %d\n",
					__func__, ret);

		kfree(clk_data);
	}

	return ret;
}

static struct platform_driver clk_mt6779_vdec_drv = {
	.probe = clk_mt6779_vdec_probe,
	.driver = {
		.name = "clk-mt6779-vdec",
		.of_match_table = of_match_clk_mt6779_vdec,
	},
};


#if MT_CLKMGR_MODULE_INIT

builtin_platform_driver(clk_mt6779_vdec_drv);

#else

static int __init clk_mt6779_vdec_platform_init(void)
{
	return platform_driver_register(&clk_mt6779_vdec_drv);
}

arch_initcall_sync(clk_mt6779_vdec_platform_init);

#endif /* MT_CLKMGR_MODULE_INIT */

