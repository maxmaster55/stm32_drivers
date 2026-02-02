#pragma once

#include "flash_regs.h"


#define FLASH ((flash_regs_t*)0x40023C00)

#define FLASH_SR_EOP     (1 << 0)
#define FLASH_SR_WRPERR  (1 << 4)
#define FLASH_SR_PGAERR  (1 << 5)
#define FLASH_SR_PGPERR  (1 << 6)
#define FLASH_SR_PGSERR  (1 << 7)

typedef enum{
    FLASH_SECTOR_0,
    FLASH_SECTOR_1,
    FLASH_SECTOR_2,
    FLASH_SECTOR_3,
    FLASH_SECTOR_4,
    FLASH_SECTOR_5,
    FLASH_SECTOR_MAX,
} flash_sector_t ;

typedef enum{
    FLASH_WRITE_BYTE,
    FLASH_WRITE_HALFWORD,
    FLASH_WRITE_WORD,
    FLASH_WRITE_DOUBLE,
    FLASH_WRITE_MAX,
} flash_w_len_t ;




void flash_unlock();
void flash_lock();
void flash_mass_erase();
void flash_erase_sector(flash_sector_t num);
void flash_write(uint32_t address, uint32_t data, flash_w_len_t len);


