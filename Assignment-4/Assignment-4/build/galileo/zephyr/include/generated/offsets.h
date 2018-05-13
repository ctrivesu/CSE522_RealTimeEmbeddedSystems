/* THIS FILE IS AUTO GENERATED.  PLEASE DO NOT EDIT.
 *
 * This header file provides macros for the offsets of various structure
 * members.  These offset macros are primarily intended to be used in
 * assembly code.
 */

#ifndef __GEN_OFFSETS_H__
#define __GEN_OFFSETS_H__

#define ___kernel_t_current_OFFSET 0x8
#define ___kernel_t_nested_OFFSET 0x0
#define ___kernel_t_irq_stack_OFFSET 0x4
#define ___kernel_t_ready_q_OFFSET 0x14
#define ___kernel_t_arch_OFFSET 0x11c
#define ___ready_q_t_cache_OFFSET 0x0
#define ___thread_base_t_user_options_OFFSET 0x8
#define ___thread_base_t_thread_state_OFFSET 0x9
#define ___thread_base_t_prio_OFFSET 0xa
#define ___thread_base_t_sched_locked_OFFSET 0xb
#define ___thread_base_t_preempt_OFFSET 0xa
#define ___thread_base_t_swap_data_OFFSET 0xc
#define ___thread_t_base_OFFSET 0x0
#define ___thread_t_caller_saved_OFFSET 0x28
#define ___thread_t_callee_saved_OFFSET 0x28
#define ___thread_t_arch_OFFSET 0x38
#define K_THREAD_SIZEOF 0x38
#define ___thread_arch_t_coopFloatReg_OFFSET 0x0
#define ___thread_arch_t_preempFloatReg_OFFSET 0x0
#define _K_THREAD_NO_FLOAT_SIZEOF 0x38
#define ___callee_saved_t_esp_OFFSET 0x0
#define __tSwapStk_eax_OFFSET 0x0
#define __tSwapStk_ebp_OFFSET 0x4
#define __tSwapStk_ebx_OFFSET 0x8
#define __tSwapStk_esi_OFFSET 0xc
#define __tSwapStk_edi_OFFSET 0x10
#define __tSwapStk_retAddr_OFFSET 0x14
#define __tSwapStk_param_OFFSET 0x18
#define __tSwapStk_SIZEOF 0x1c
#define __NANO_ESF_esp_OFFSET 0x0
#define __NANO_ESF_ebp_OFFSET 0x4
#define __NANO_ESF_ebx_OFFSET 0x8
#define __NANO_ESF_esi_OFFSET 0xc
#define __NANO_ESF_edi_OFFSET 0x10
#define __NANO_ESF_edx_OFFSET 0x14
#define __NANO_ESF_ecx_OFFSET 0x1c
#define __NANO_ESF_eax_OFFSET 0x18
#define __NANO_ESF_errorCode_OFFSET 0x20
#define __NANO_ESF_eip_OFFSET 0x24
#define __NANO_ESF_cs_OFFSET 0x28
#define __NANO_ESF_eflags_OFFSET 0x2c
#define __ISR_LIST_SIZEOF 0x18
#define __MMU_REGION_SIZEOF 0x10

#endif /* __GEN_OFFSETS_H__ */
