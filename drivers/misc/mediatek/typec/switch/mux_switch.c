// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 MediaTek Inc.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/usb/typec.h>
#include <linux/usb/typec_mux.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

#include "bus.h"
#include "mux_switch.h"

/* struct typec_mux_switch */
struct typec_mux_switch {
	struct device *dev;
	struct typec_switch *sw;
	struct typec_mux *mux;
	int orientation;
	int state;
};

/* struct mtk_typec_switch */
struct mtk_typec_switch {
	struct device *dev;
	struct typec_switch *sw;
	struct list_head list;
};

/* struct mtk_typec_mux */
struct mtk_typec_mux {
	struct device *dev;
	struct typec_mux *mux;
	struct list_head list;
};

static LIST_HEAD(mux_list);
static LIST_HEAD(switch_list);
static DEFINE_MUTEX(mux_lock);
static DEFINE_MUTEX(switch_lock);

static struct typec_mux_switch *g_mux_sw;

/* MUX */
struct typec_mux *mtk_typec_mux_register(struct device *dev,
			const struct typec_mux_desc *desc)
{
	struct mtk_typec_mux *typec_mux;
	struct typec_mux *mux;

	mutex_lock(&mux_lock);
	list_for_each_entry(typec_mux, &mux_list, list) {
		if (typec_mux->dev == dev) {
			mux = ERR_PTR(-EEXIST);
			goto out;
		}
	}

	typec_mux = kzalloc(sizeof(*typec_mux), GFP_KERNEL);
	if (!typec_mux) {
		mux = ERR_PTR(-ENOMEM);
		goto out;
	}

	mux = typec_mux_register(dev, desc);
	if (IS_ERR(mux)) {
		kfree(typec_mux);
		mux = ERR_PTR(-EINVAL);
		goto out;
	}
	typec_mux->mux = mux;
	list_add_tail(&typec_mux->list, &mux_list);
out:
	mutex_unlock(&mux_lock);
	return mux;
}
EXPORT_SYMBOL_GPL(mtk_typec_mux_register);

void mtk_typec_mux_unregister(struct typec_mux *mux)
{
	struct mtk_typec_mux *typec_mux;

	mutex_lock(&mux_lock);
	list_for_each_entry(typec_mux, &mux_list, list) {
		if (typec_mux->mux == mux)
			break;
	}

	list_del(&typec_mux->list);
	kfree(typec_mux);
	mutex_unlock(&mux_lock);

	typec_mux_unregister(mux);
}
EXPORT_SYMBOL_GPL(mtk_typec_mux_unregister);

static int mtk_typec_mux_set(struct typec_mux *mux, int state)
{
	struct typec_mux_switch *mux_sw = typec_mux_get_drvdata(mux);
	struct mtk_typec_mux *typec_mux;
	int ret = 0;

	dev_info(mux_sw->dev, "%s %d %d\n", __func__,
		 mux_sw->state, state);

	if (mux_sw->state == state)
		return ret;

	mutex_lock(&mux_lock);

	list_for_each_entry(typec_mux, &mux_list, list) {
		if (!IS_ERR_OR_NULL(typec_mux->mux))
			typec_mux->mux->set(typec_mux->mux, state);
	}

	mux_sw->state = state;

	mutex_unlock(&mux_lock);

	return ret;
}

/* SWITCH */
struct typec_switch *mtk_typec_switch_register(struct device *dev,
			const struct typec_switch_desc *desc)
{
	struct mtk_typec_switch *typec_sw;
	struct typec_switch *sw;

	mutex_lock(&switch_lock);
	list_for_each_entry(typec_sw, &switch_list, list) {
		if (typec_sw->dev == dev) {
			sw = ERR_PTR(-EEXIST);
			goto out;
		}
	}

	typec_sw = kzalloc(sizeof(*typec_sw), GFP_KERNEL);
	if (!typec_sw) {
		sw = ERR_PTR(-ENOMEM);
		goto out;
	}

	sw = typec_switch_register(dev, desc);
	if (IS_ERR(sw)) {
		kfree(typec_sw);
		sw = ERR_PTR(-EINVAL);
		goto out;
	}
	typec_sw->sw = sw;
	list_add_tail(&typec_sw->list, &switch_list);
out:
	mutex_unlock(&switch_lock);
	return sw;
}
EXPORT_SYMBOL_GPL(mtk_typec_switch_register);

void mtk_typec_switch_unregister(struct typec_switch *sw)
{
	struct mtk_typec_switch *typec_sw;

	mutex_lock(&switch_lock);
	list_for_each_entry(typec_sw, &switch_list, list) {
		if (typec_sw->sw == sw)
			break;
	}
	list_del(&typec_sw->list);
	kfree(typec_sw);
	mutex_unlock(&switch_lock);

	typec_switch_unregister(sw);
}
EXPORT_SYMBOL_GPL(mtk_typec_switch_unregister);

static int mtk_typec_switch_set(struct typec_switch *sw,
			      enum typec_orientation orientation)
{
	struct typec_mux_switch *mux_sw = typec_switch_get_drvdata(sw);
	struct mtk_typec_switch *typec_sw;
	int ret = 0;

	dev_info(mux_sw->dev, "%s %d %d\n", __func__,
		 mux_sw->orientation, orientation);

	if (mux_sw->orientation == orientation)
		return ret;

	mutex_lock(&switch_lock);

	list_for_each_entry(typec_sw, &switch_list, list) {
		if (!IS_ERR_OR_NULL(typec_sw->sw))
			typec_sw->sw->set(typec_sw->sw, orientation);
	}

	mux_sw->orientation = orientation;

	mutex_unlock(&switch_lock);

	return ret;
}

void usb3_switch_set(int orientation)
{
	if (!g_mux_sw)
		return;

	mtk_typec_switch_set(g_mux_sw->sw, orientation);
}
EXPORT_SYMBOL_GPL(usb3_switch_set);

static int typec_mux_switch_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct typec_mux_switch *mux_sw;
	struct typec_switch_desc sw_desc;
	struct typec_mux_desc mux_desc;
	int ret = 0;

	dev_info(dev, "%s\n", __func__);

	mux_sw = kzalloc(sizeof(*mux_sw), GFP_KERNEL);
	if (!mux_sw)
		return -ENOMEM;

	mux_sw->dev = dev;

	sw_desc.drvdata = mux_sw;
	sw_desc.fwnode = dev->fwnode;
	sw_desc.set = mtk_typec_switch_set;

	mux_sw->sw = typec_switch_register(dev, &sw_desc);
	if (IS_ERR(mux_sw->sw)) {
		dev_info(dev, "error registering typec switch: %ld\n",
			PTR_ERR(mux_sw->sw));
		return PTR_ERR(mux_sw->sw);
	}

	mux_desc.drvdata = mux_sw;
	mux_desc.fwnode = dev->fwnode;
	mux_desc.set = mtk_typec_mux_set;

	mux_sw->mux = typec_mux_register(dev, &mux_desc);
	if (IS_ERR(mux_sw->mux)) {
		typec_switch_unregister(mux_sw->sw);
		dev_info(dev, "error registering typec mux: %ld\n",
			PTR_ERR(mux_sw->mux));
		return PTR_ERR(mux_sw->mux);
	}

	g_mux_sw = mux_sw;

	platform_set_drvdata(pdev, mux_sw);

	dev_info(dev, "%s done\n", __func__);
	return ret;
}

static int typec_mux_switch_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id typec_mux_switch_ids[] = {
	{.compatible = "mediatek,typec_switch",},
	{.compatible = "mediatek,typec_mux_switch",},
	{},
};

static struct platform_driver typec_mux_switch_driver = {
	.probe = typec_mux_switch_probe,
	.remove = typec_mux_switch_remove,
	.driver = {
		.name = "mtk-typec-mux-switch",
		.of_match_table = typec_mux_switch_ids,
	},
};

module_platform_driver(typec_mux_switch_driver);

MODULE_DESCRIPTION("Mediatek Type-C mux switch driver");
MODULE_LICENSE("GPL");
