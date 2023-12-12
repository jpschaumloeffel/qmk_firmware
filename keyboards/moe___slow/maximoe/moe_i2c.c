#include <stdint.h>
#include "quantum.h"

#include "moe_i2c.h"
#include "pio_i2c.h"


static PIO pio;
static uint sm = 0;

// i2c read register
uint8_t moe_i2c_read_reg(uint8_t i2c_addr, uint8_t reg_addr, void* rxbuf, uint8_t len) {

    // TODO this only works for single master systems, because no repeated start condition is generated

    // write register address
    pio_i2c_write_blocking(pio, sm, i2c_addr, &reg_addr, 1);

    // read data from device
    return pio_i2c_read_blocking(pio, sm, i2c_addr, rxbuf, len);
}


// i2c write register
uint8_t moe_i2c_write_reg(uint8_t i2c_addr, uint8_t reg_addr, void* txbuf, uint8_t len) {
    uint8_t data[128];
    data[0] = reg_addr;
    memcpy(&data[1], txbuf, len);

    return pio_i2c_write_blocking(pio, sm, i2c_addr, data, len + 1);
}

void moe_i2c_init(void) {

    // select pio
    pio = pio0;

    uint pio_idx = pio_get_index(pio);
    /* Get PIOx peripheral out of reset state. */
    hal_lld_peripheral_unreset(pio_idx == 0 ? RESETS_ALLREG_PIO0 : RESETS_ALLREG_PIO1);

    // clang-format off
    iomode_t rgb_pin_mode = PAL_RP_PAD_SLEWFAST |
                            PAL_RP_GPIO_OE |
                            (pio_idx == 0 ? PAL_MODE_ALTERNATE_PIO0 : PAL_MODE_ALTERNATE_PIO1);
    // clang-format on

    palSetLineMode(PIN_SDA, rgb_pin_mode);
    palSetLineMode(PIN_SCL, rgb_pin_mode);

    sm = pio_claim_unused_sm(pio, true);

    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, PIN_SDA, PIN_SCL);
}
