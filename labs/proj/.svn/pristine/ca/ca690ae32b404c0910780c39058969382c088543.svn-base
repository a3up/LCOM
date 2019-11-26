#pragma once

#include <lcom/lcf.h>
#include "macros.h"

#pragma pack(1)

struct vbe_cntr_info_t {
    uint8_t VBESignature[4];
    uint8_t VbeVersion[2];
    uint32_t OemStringPtr;
    uint8_t Capabilities[4];
    uint32_t VideoModePtr;
    uint16_t TotalMemory;
    uint16_t OemSoftwareRev;
    uint32_t OemVendorNamePtr;
    uint32_t OemProductNamePtr;
    uint32_t OemProductRevPtr;
    uint8_t Reserved[222];
    uint8_t OEMData[256];
};

#pragma options align = reset

void *video_mem;
vbe_mode_info_t vbe_info;
struct vg_vbe_contr_info *vbe_controller_info;

uint32_t calcPtrDelta(uint32_t ptr, mmap_t *mmry);

uint32_t far2linear(uint32_t far);

int set_graphics_mode(uint16_t mode, reg86_t *reg86);

int allow_memory_mapping(uint16_t mode, struct minix_mem_range *mr);

int init_graphics(uint16_t mode);

int vbe_get_controller_info();

int m_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vbe_info);
