/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _VCD_DDL_API_H_
#define _VCD_DDL_API_H_

#include "vidc_type.h"
#include "vcd_api.h"

#define VCD_EVT_RESP_DDL_BASE             0x3000
#define VCD_EVT_RESP_DEVICE_INIT          (VCD_EVT_RESP_DDL_BASE + 0x1)
#define VCD_EVT_RESP_OUTPUT_REQ           (VCD_EVT_RESP_DDL_BASE + 0x2)
#define VCD_EVT_RESP_EOS_DONE             (VCD_EVT_RESP_DDL_BASE + 0x3)
#define VCD_EVT_RESP_TRANSACTION_PENDING  (VCD_EVT_RESP_DDL_BASE + 0x4)

#define VCD_S_DDL_ERR_BASE       0x90000000
#define VCD_ERR_MAX_NO_CODEC     (VCD_S_DDL_ERR_BASE + 0x1)
#define VCD_ERR_CLIENT_PRESENT   (VCD_S_DDL_ERR_BASE + 0x2)
#define VCD_ERR_CLIENT_FATAL     (VCD_S_DDL_ERR_BASE + 0x3)
#define VCD_ERR_NO_SEQ_HDR       (VCD_S_DDL_ERR_BASE + 0x4)

#define VCD_I_CUSTOM_BASE        (VCD_I_RESERVED_BASE)
#define VCD_I_RC_LEVEL_CONFIG    (VCD_I_CUSTOM_BASE + 0x1)
#define VCD_I_FRAME_LEVEL_RC     (VCD_I_CUSTOM_BASE + 0x2)
#define VCD_I_ADAPTIVE_RC        (VCD_I_CUSTOM_BASE + 0x3)
#define VCD_I_CUSTOM_DDL_BASE    (VCD_I_RESERVED_BASE + 0x100)
#define DDL_I_INPUT_BUF_REQ      (VCD_I_CUSTOM_DDL_BASE + 0x1)
#define DDL_I_OUTPUT_BUF_REQ     (VCD_I_CUSTOM_DDL_BASE + 0x2)
#define DDL_I_DPB                (VCD_I_CUSTOM_DDL_BASE + 0x3)
#define DDL_I_DPB_RELEASE        (VCD_I_CUSTOM_DDL_BASE + 0x4)
#define DDL_I_DPB_RETRIEVE       (VCD_I_CUSTOM_DDL_BASE + 0x5)
#define DDL_I_REQ_OUTPUT_FLUSH   (VCD_I_CUSTOM_DDL_BASE + 0x6)
#define DDL_I_SEQHDR_ALIGN_BYTES (VCD_I_CUSTOM_DDL_BASE + 0x7)
#define DDL_I_CAPABILITY         (VCD_I_CUSTOM_DDL_BASE + 0x8)
#define DDL_I_FRAME_PROC_UNITS   (VCD_I_CUSTOM_DDL_BASE + 0x9)
#define DDL_I_SEQHDR_PRESENT     (VCD_I_CUSTOM_DDL_BASE + 0xA)

struct vcd_property_rc_level_type{
	u32 b_frame_level_rc;
	u32 b_mb_level_rc;
};
struct vcd_property_frame_level_rc_params_type{
	u32 n_reaction_coeff;
};
struct vcd_property_adaptive_rc_params_type{
	u32 b_dark_region_as_flag;
	u32 b_smooth_region_as_flag;
	u32 b_static_region_as_flag;
	u32 b_activity_region_flag;
};
struct ddl_property_dec_pic_buffers_type{
	struct ddl_frame_data_type_tag *a_dec_pic_buffers;
	u32 n_no_of_dec_pic_buf;
};
struct ddl_property_capability_type{
	u32 n_max_num_client;
	u32 n_general_command_depth;
	u32 b_exclusive;
	u32 n_frame_command_depth;
	u32 n_ddl_time_out_in_ms;
};
struct ddl_init_config_type{
	u8 *p_core_virtual_base_addr;
	void (*pf_interrupt_clr) (void);
	void (*ddl_callback) (u32 event, u32 status, void *payload, u32 size,
		u32 *p_ddl_handle, void *const p_client_data);
};
struct ddl_frame_data_type_tag{
	struct vcd_frame_data_type vcd_frm;
	u32 b_frm_trans_end;
	u32 n_frm_delta;
};
u32 ddl_device_init(struct ddl_init_config_type *p_ddl_init_config,
	void *p_client_data);
u32 ddl_device_release(void *p_client_data);
u32 ddl_open(u32 **p_ddl_handle, u32 b_decoding);
u32 ddl_close(u32 **p_ddl_handle);
u32 ddl_encode_start(u32 *ddl_handle, void *p_client_data);
u32 ddl_encode_frame(u32 *ddl_handle,
	struct ddl_frame_data_type_tag *p_input_frame,
	struct ddl_frame_data_type_tag *p_output_bit, void *p_client_data);
u32 ddl_encode_end(u32 *ddl_handle, void *p_client_data);
u32 ddl_decode_start(u32 *ddl_handle, struct vcd_sequence_hdr_type *p_header,
	void *p_client_data);
u32 ddl_decode_frame(u32 *ddl_handle,
	struct ddl_frame_data_type_tag *p_input_bits, void *p_client_data);
u32 ddl_decode_end(u32 *ddl_handle, void *p_client_data);
u32 ddl_set_property(u32 *ddl_handle,
	struct vcd_property_hdr_type *p_property_hdr, void *p_property_value);
u32 ddl_get_property(u32 *ddl_handle,
	struct vcd_property_hdr_type *p_property_hdr, void *p_property_value);
u32 ddl_process_core_response(void);
u32 ddl_reset_hw(u32 n_mode);
void ddl_read_and_clear_interrupt(void);
#endif
