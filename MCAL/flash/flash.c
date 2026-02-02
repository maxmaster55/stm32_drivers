#include "flash.h"


void flash_unlock()
{
    FLASH->KEYR.KEY = 0x45670123;
    FLASH->KEYR.KEY = 0xCDEF89AB;
}


void flash_lock()
{
    while (FLASH->SR.BSY);
    FLASH->CR.LOCK = 1;
}


void flash_mass_erase()
{
    while (FLASH->SR.BSY);
    FLASH->CR.MER = 1;
    FLASH->CR.STRT = 1;
    while (FLASH->SR.BSY);
}


void flash_erase_sector(flash_sector_t num)
{
    if (num >= FLASH_SECTOR_MAX) return; // tf you doin? 
    
    while (FLASH->SR.BSY);
    FLASH->CR.SER = 1;
    FLASH->CR.SNB = num;
    FLASH->CR.STRT = 1;
    while (FLASH->SR.BSY);
}
void flash_write(uint32_t address, uint32_t data, flash_w_len_t len)
{
    if (len >= FLASH_WRITE_MAX) return;

    // Alignment check (THIS PREVENTS PGAERR)
    if ((len == FLASH_WRITE_HALFWORD && (address & 0x1)) ||
        (len == FLASH_WRITE_WORD     && (address & 0x3)) ||
        (len == FLASH_WRITE_DOUBLE   && (address & 0x7)))
        return;

    // Wait until flash is idle
    while (FLASH->SR.BSY);

    // Clear previous errors
    FLASH->SR.reg =
        FLASH_SR_PGSERR |
        FLASH_SR_PGPERR |
        FLASH_SR_PGAERR |
        FLASH_SR_WRPERR |
        FLASH_SR_EOP;

    FLASH->CR.PSIZE = len;

    // Enable programming
    FLASH->CR.PG = 1;

    switch (len)
    {
        case FLASH_WRITE_BYTE:
            *(volatile uint8_t *)address = (uint8_t)data;
            break;

        case FLASH_WRITE_HALFWORD:
            *(volatile uint16_t *)address = (uint16_t)data;
            break;

        case FLASH_WRITE_WORD:
            *(volatile uint32_t *)address = data;
            break;

        default:
            FLASH->CR.PG = 0;
            return;
    }

    while (FLASH->SR.BSY);

    FLASH->CR.PG = 0;
}
