#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x4766f3ab, "module_layout" },
	{ 0xf594bedf, "ieee80211_rx_napi" },
	{ 0x3351716f, "kmalloc_caches" },
	{ 0xf9a482f9, "msleep" },
	{ 0x4ae4635, "trace_handle_return" },
	{ 0x737b964b, "generic_file_llseek" },
	{ 0x915b64aa, "debugfs_create_dir" },
	{ 0x2a15cc69, "single_open" },
	{ 0x58536a98, "debugfs_create_u8" },
	{ 0x1cc9a88b, "page_address" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x7761ff17, "single_release" },
	{ 0x204933ca, "seq_puts" },
	{ 0xe7e4d52a, "_raw_spin_lock_bh" },
	{ 0x7ef39823, "ieee80211_hdrlen" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0x342f6139, "seq_printf" },
	{ 0xdeee77ba, "usb_kill_urb" },
	{ 0xb1b939e, "kmemdup" },
	{ 0x64ae1aca, "ieee80211_unregister_hw" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x2d5f69b3, "rcu_read_unlock_strict" },
	{ 0x29d9f26e, "cancel_delayed_work_sync" },
	{ 0x67ea780, "mutex_unlock" },
	{ 0xd66ebe42, "trace_event_buffer_reserve" },
	{ 0xc6fcaf47, "debugfs_create_file" },
	{ 0xe4302601, "seq_read" },
	{ 0xac11cd47, "bpf_trace_run3" },
	{ 0xa3186107, "__alloc_pages_nodemask" },
	{ 0x5a15740f, "skb_trim" },
	{ 0x75a44c47, "ieee80211_stop_queues" },
	{ 0xda5bad0b, "simple_attr_read" },
	{ 0xeaa85e3d, "ieee80211_stop_queue" },
	{ 0xbd735667, "ieee80211_tx_status" },
	{ 0x61f088d4, "debugfs_create_u32" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xe3f4ed75, "_dev_warn" },
	{ 0x5f754e5a, "memset" },
	{ 0xf4ab9c1, "bpf_trace_run9" },
	{ 0xf3d0b495, "_raw_spin_unlock_irqrestore" },
	{ 0xe6bd5aaa, "usb_deregister" },
	{ 0xa89ee708, "ieee80211_alloc_hw_nm" },
	{ 0xe346f67a, "__mutex_init" },
	{ 0xc5850110, "printk" },
	{ 0xb4a4b2e9, "ieee80211_free_txskb" },
	{ 0x921b07b1, "__cpu_online_mask" },
	{ 0xbbe0e81c, "ieee80211_wake_queues" },
	{ 0x9d2ab8ac, "__tasklet_schedule" },
	{ 0x7c73cbd3, "trace_event_reg" },
	{ 0xb0280de5, "usb_poison_urb" },
	{ 0xd3d60a8, "seq_putc" },
	{ 0x7977a84a, "usb_control_msg" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0x184ae64b, "skb_push" },
	{ 0xc271c3be, "mutex_lock" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x2364c85a, "tasklet_init" },
	{ 0x46344e3f, "simple_attr_release" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x83e548a4, "skb_pull" },
	{ 0x98401bdc, "usb_free_coherent" },
	{ 0x56755c4a, "_dev_err" },
	{ 0xa1b928cf, "perf_trace_run_bpf_submit" },
	{ 0xaaecf75d, "perf_trace_buf_alloc" },
	{ 0x8404439b, "devm_kmemdup" },
	{ 0x7d4c6513, "ieee80211_queue_delayed_work" },
	{ 0xea3c74e, "tasklet_kill" },
	{ 0xff6b6eb6, "ieee80211_stop_tx_ba_cb_irqsafe" },
	{ 0xffeaebe8, "_dev_info" },
	{ 0xeeedc392, "usb_submit_urb" },
	{ 0x276c8e8f, "__free_pages" },
	{ 0x299fefc, "__alloc_skb" },
	{ 0xd29e973d, "usb_get_dev" },
	{ 0x12a38747, "usleep_range" },
	{ 0x1eb7cb1c, "trace_event_ignore_this_pid" },
	{ 0x3ec80fa0, "_raw_spin_unlock_bh" },
	{ 0xcf86cdac, "queue_delayed_work_on" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x254bab79, "usb_reset_device" },
	{ 0x2a3076dc, "usb_bulk_msg" },
	{ 0x557514b0, "usb_put_dev" },
	{ 0xee43fd9b, "___ratelimit" },
	{ 0x531c4f62, "ieee80211_send_bar" },
	{ 0xd46b54dd, "flush_delayed_work" },
	{ 0x87c76f88, "ieee80211_get_tx_rates" },
	{ 0x256a02d4, "trace_event_buffer_commit" },
	{ 0xf11d67ab, "pskb_expand_head" },
	{ 0x81f8ac7e, "kmem_cache_alloc_trace" },
	{ 0xae577d60, "_raw_spin_lock" },
	{ 0xde55e795, "_raw_spin_lock_irqsave" },
	{ 0x4588f169, "ieee80211_wake_queue" },
	{ 0x12baa630, "event_triggers_call" },
	{ 0x54df09bd, "ieee80211_get_hdrlen_from_skb" },
	{ 0x8f0b9637, "bpf_trace_run2" },
	{ 0xa432fb9b, "ieee80211_register_hw" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x9f929d5e, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0x51ec437d, "trace_event_raw_init" },
	{ 0x54bdaf6a, "ieee80211_tx_status_ext" },
	{ 0x644b09c6, "skb_add_rx_frag" },
	{ 0xb1b79332, "usb_register_driver" },
	{ 0x60791269, "request_firmware" },
	{ 0xd79f8e2c, "ieee80211_free_hw" },
	{ 0x870d5a1c, "__init_swait_queue_head" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xa249bf65, "trace_raw_output_prep" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xc37335b0, "complete" },
	{ 0xca54fee, "_test_and_set_bit" },
	{ 0xb39c7cf9, "trace_seq_printf" },
	{ 0x99bb8806, "memmove" },
	{ 0x1ec72601, "consume_skb" },
	{ 0x1aa9ea55, "usb_alloc_coherent" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0x13a660c6, "skb_put" },
	{ 0x4185d389, "devm_kmalloc" },
	{ 0xa16b21fb, "wait_for_completion_timeout" },
	{ 0xd9830768, "bpf_trace_run4" },
	{ 0xf61a65c1, "usb_free_urb" },
	{ 0xdf9208c0, "alloc_workqueue" },
	{ 0x1eb22214, "simple_attr_open" },
	{ 0xd0e9fb09, "release_firmware" },
	{ 0x79543800, "ieee80211_start_tx_ba_cb_irqsafe" },
	{ 0x687f86d0, "simple_attr_write" },
	{ 0xcac7328f, "__skb_pad" },
	{ 0xd96d9198, "usb_alloc_urb" },
};

MODULE_INFO(depends, "mac80211,cfg80211");

MODULE_ALIAS("usb:v0B05p17D3d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0E8Dp760Ad*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0E8Dp760Bd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3431d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3434d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v148Fp7601d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v148Fp760Ad*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v148Fp760Bd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v148Fp760Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v148Fp760Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2001p3D04d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2717p4106d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2955p0001d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2955p1001d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2A5Fp1000d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v7392p7710d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "18D22B3DCA18418BBA41AA7");
