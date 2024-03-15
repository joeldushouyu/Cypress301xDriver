#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x30ff7695, "module_layout" },
	{ 0xdaf5fc38, "noop_llseek" },
	{ 0x458f49d8, "usb_deregister" },
	{ 0xb0a1b0c0, "usb_register_driver" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x1000e51, "schedule" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x800473f, "__cond_resched" },
	{ 0x89940875, "mutex_lock_interruptible" },
	{ 0x8b146ea0, "usb_deregister_dev" },
	{ 0xc9c51f39, "usb_autopm_get_interface" },
	{ 0x80c06e26, "usb_find_interface" },
	{ 0x7680ff31, "usb_autopm_put_interface" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xcf2aeb9e, "_dev_info" },
	{ 0x75e53cd6, "usb_register_dev" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x93c7edeb, "usb_find_common_endpoints" },
	{ 0xbc52a41f, "usb_get_intf" },
	{ 0xce0dc0cc, "usb_get_dev" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x4f00afd3, "kmem_cache_alloc_trace" },
	{ 0xac1c4313, "kmalloc_caches" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x49bab3c9, "usb_unanchor_urb" },
	{ 0xbd70750d, "usb_submit_urb" },
	{ 0x29169978, "usb_anchor_urb" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xbab8c874, "usb_alloc_coherent" },
	{ 0x1f861c5f, "usb_alloc_urb" },
	{ 0xe9ffc063, "down_trylock" },
	{ 0x8427cc7b, "_raw_spin_lock_irq" },
	{ 0x6bd0e573, "down_interruptible" },
	{ 0x92997ed8, "_printk" },
	{ 0x3d519628, "_dev_err" },
	{ 0xcf2a6966, "up" },
	{ 0x78e3401a, "usb_free_coherent" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0x37a0cba, "kfree" },
	{ 0xd5ab5b23, "usb_put_dev" },
	{ 0x2ced330f, "usb_put_intf" },
	{ 0x88bce085, "usb_free_urb" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x962c8ae1, "usb_kill_anchored_urbs" },
	{ 0x73dc96bd, "usb_kill_urb" },
	{ 0x407af304, "usb_wait_anchor_empty_timeout" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x33a21a09, "pv_ops" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v04B4p00F1d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "7B56A4881F793C37DD0D8B6");
