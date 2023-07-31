// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 MediaTek Inc.
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM apu_power_events
#if !defined(_TRACE_APUSYS_FREQ_EVENTS_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_APUSYS_FREQ_EVENTS_H
#include <linux/tracepoint.h>
#include "apu_power_api.h"
TRACE_EVENT(APUSYS_DFS,
	TP_PROTO(struct apu_power_info *pwr_info,
		 unsigned int mdla0),
	TP_ARGS(pwr_info, mdla0),
	TP_STRUCT__entry(
		__field(int, conn_freq)
		__field(int, vpu0_freq)
		__field(int, vpu1_freq)
		__field(int, mdla0_freq)
		__field(int, iommu_freq)
	),
	TP_fast_assign(
		__entry->conn_freq = pwr_info->conn_freq;
		__entry->vpu0_freq = pwr_info->vpu0_freq;
		__entry->vpu1_freq = pwr_info->vpu1_freq;
		__entry->mdla0_freq = pwr_info->mdla0_freq;
		__entry->iommu_freq = pwr_info->iommu_freq;
	),
	TP_printk("conn=%d,vpu0=%d,vpu1=%d,mdla0=%d,ipuif=%d",
		  __entry->conn_freq, __entry->vpu0_freq,
		  __entry->vpu1_freq,
		  __entry->mdla0_freq,
		  __entry->iommu_freq)
);

#endif /* _TRACE_APUSYS_FREQ_EVENTS_H */
/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE apu_power_events
#include <trace/define_trace.h>
