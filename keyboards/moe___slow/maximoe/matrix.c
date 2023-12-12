#include <stdint.h>

#include "quantum.h"
#include "info_config.h"
#include "matrix.h"
#include "print.h"
#include "gpio.h"
#include "moe_i2c.h"

#define GPIO_ROW0 GP2
#define GPIO_ROW1 GP18  // 22 on SparkFun
#define GPIO_ROW2 GP4
#define GPIO_ROW3 GP7
#define GPIO_ROW4 GP6
#define GPIO_ROW5 GP5

static uint8_t row_pins[MATRIX_ROWS] = { GPIO_ROW0, GPIO_ROW1, GPIO_ROW2, GPIO_ROW3, GPIO_ROW4, GPIO_ROW5 };

#define COL_PIN_COUNT 7
// col0..6 are on GPIOs directly
static uint8_t col_pins[COL_PIN_COUNT] = { GP26, GP20, GP28, GP29, GP19, GP9, GP8 };

#define COLS_ON_PORT_A 6

#define MCP23017_TWI_ADDRESS 0x20
#define MCP23017_RESET_GPIO GP3

#define IOCON 0x0A
#define IOCON_BANK1 0x05

#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01

#define GPPUA  0x0C  // GPIO pull-up resistor register
#define GPPUB  0x0D

#define GPIOA  0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB  0x13


static uint8_t mcp23017_init(void) {
    // default: iocon.bank = 0, iocon.seqop = 0
    // this is the "special mode" described in the datasheet which toggles between port a and port b reads

    uint8_t ret = 0;
    uint8_t data[4];

    ret = moe_i2c_read_reg(MCP23017_TWI_ADDRESS, IOCON, data, 2);
    uprintf("%s: read from IOCON (%x), result=%d, data=%x %x\n", __FUNCTION__, IOCON, ret, data[0], data[1]);

    ret = moe_i2c_read_reg(MCP23017_TWI_ADDRESS, IOCON, data, 2);
    uprintf("%s: read from IOCON (%x), result=%d, data=%x %x\n", __FUNCTION__, IOCON_BANK1, ret, data[0], data[1]);

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0

    // set column pins as output
    data[0] = 0b11000000;  // IODIRA
    data[1] = 0b11000000;  // IODIRB
    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, IODIRA, data, 2);

    uprintf("%s: wrote to %x, result=%d, data=%x %x\n", __FUNCTION__, IODIRA, ret, data[0], data[1]);

    if (ret) goto out;  // make sure we got an ACK

    // configure no pull-ups for col pins
    data[0] = 0b11000000;
    data[1] = 0b11000000;

    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPPUA, data, 2);

    uprintf("%s: wrote to %x, result=%d, data=%x %x\n", __FUNCTION__, GPPUA, ret, data[0], data[1]);

    if (ret) goto out;  // make sure we got an ACK

    // write ones to all columns (no column active)
    data[0] = 0xFF;
    data[1] = 0xFF;
    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPIOA, data, 2);

 out:
    return ret;
}

void matrix_init_custom(void) {
    // TODO: initialize hardware here

    // not sure if needed (?) copied from dilemma.c
    for (int i=0; i<MATRIX_ROWS; i++) {
        // row pins are inputs with pullup
        setPinInputHigh(row_pins[i]);
    }
    for (int i=0; i<COL_PIN_COUNT; i++) {
        // col pins are outputs and start all inactive (== high)
        setPinOutput(col_pins[i]);
        writePinHigh(col_pins[i]);
    }

    debug_enable = true;
    debug_keyboard = true;

    wait_ms(2000);
    uprintf("%s - start\n", __FUNCTION__);

    setPinOutput(MCP23017_RESET_GPIO);

    moe_i2c_init();

/*
    // reset mcp
    writePinHigh(MCP23017_RESET_GPIO);
    wait_ms(1);
    writePinLow(MCP23017_RESET_GPIO);
    wait_ms(1);
    writePinHigh(MCP23017_RESET_GPIO);
    wait_ms(5);

    // i2c bus scan for debug
    uprintf("\nPIO I2C Bus Scan\n");
    uprintf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }
        // Perform a 0-byte read from the probe address. The read function
        // returns a negative result NAK'd any time other than the last data
        // byte. Skip over reserved addresses.
        int result;
        result = pio_i2c_read_blocking(pio, sm, addr, NULL, 0);
        // pio_sm_clear_fifos(pio, sm);

        uprintf(result < 0 ? "." : "@");
        uprintf(addr % 16 == 15 ? "\n" : "  ");
    }
    uprintf("Done.\n");
    // bus scan end

    wait_ms(2000);
*/

    // reset mcp
    writePinHigh(MCP23017_RESET_GPIO);
    wait_ms(1);
    writePinLow(MCP23017_RESET_GPIO);
    wait_ms(1);
    writePinHigh(MCP23017_RESET_GPIO);
    wait_ms(5);

    mcp23017_init();
}

void matrix_set_col_status(uint8_t col) {
    // set only one col as output (low), others inactive (high)
    uint8_t data[2];
    uint8_t ret;
    (void) ret;

    if (col == 0) {
        // first col should be active, set all the cols on the expander inactive once
        data[0] = 0xFF;
        data[1] = 0xFF;
        ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPIOA, data, 2);
    }

    if (col > 0 && col <= COL_PIN_COUNT) {
        // deactivate previously inactive col GPIO
        writePinHigh(col_pins[col - 1]);
    }

    if (col < COL_PIN_COUNT) {
        writePinLow(col_pins[col]);
    } else {
        // set the col active via the port expander
        // first col starts near the MSB on each port
        data[0] = 0xFF;
        data[1] = 0xFF;
        if ((col - COL_PIN_COUNT) < COLS_ON_PORT_A) {
            // col is on port A

            data[0] = ~(0x20 >> (col - COL_PIN_COUNT));
        } else {
            // col is on port B
            data[1] = ~(0x20 >> (col - (COL_PIN_COUNT + COLS_ON_PORT_A)));
        }
        ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPIOA, data, 2);

        // TODO this could be optimized a bit, we don't need to write both registers all the time if they don't change
    }

/*
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (row == i) {
            setPinOutput(row_pins[i]);
            writePinLow(row_pins[i]);
        } else {
            setPinInput(row_pins[i]);
        }
    }
    */
    /*
    uint8_t data;
    uint8_t ret;
    (void) ret;

    // set one zero
    data = 0xFF & ~(1 << row);
    ret = i2c_write_reg(MCP23017_TWI_ADDRESS, GPIOA, &data, 1);

    */

    // uprintf("%s: wrote to %x, result=%d, data=%x\n", __FUNCTION__, GPIOA, ret, data);
}


bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;

    // save entire matrix
    // could probably eliminate with some bit masking below
    matrix_row_t last_matrix[MATRIX_ROWS];
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        last_matrix[row] = current_matrix[row];
    }

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {

        // enable output col
        matrix_set_col_status(col);
        matrix_io_delay();

        // read all rows
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            // insert bit if row pulled low by pressed key
            bool active = readPin(row_pins[row]);
            if (!active) {
                current_matrix[row] |= (1 << col);
            } else {
                // mask col
                current_matrix[row] &= ~(0x01 << col);
            }
        }
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        if (last_matrix[row] != current_matrix[row]) {
            matrix_has_changed = true;
            break;
        }
    }

    /*

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t last_row_value = current_matrix[row];
        matrix_row_t cols = 0;

        // enable output row
        matrix_set_col_status(row);
        matrix_io_delay();

        // now detect which cols can detect a zero (== key pressed)

        uint8_t port_b;
        uint8_t ret;
        (void) ret;

        ret = i2c_read_reg(MCP23017_TWI_ADDRESS, GPIOB, &port_b, 1);

        // uprintf("%s: read from %x, result=%d, data=%x\n", __FUNCTION__, GPIOB, ret, port_b);
        cols = ~port_b & 0x03;

        / *
        // read port expander
        uint8_t port_a, port_b;
        i2c_readReg(MCP23017_TWI_ADDRESS, GPIOA, &port_a, 1, I2C_TIMEOUT);
        i2c_readReg(MCP23017_TWI_ADDRESS, GPIOB, &port_b, 1, I2C_TIMEOUT);

        uprintf("read port a = %x, port b = %x\n", port_a, port_b);

        // read cols on GPIOs

        // cols auf B (msb to lsb): 13 14 15 16 17 18
        // cols auf A (msb to lsb): 7 8 9 10 11 12
        // cols auf GPIOS see col_pins 6 5 4 3 2 1 0

        // shift in cols from port b
        for (int i = 0; i < 6; i++) {
            cols |= (port_b & 1);
            cols <<= 1;
        }

        // shift in cols from port a
        for (int i = 0; i < 6; i++) {
            cols |= (port_a & 1);
            cols <<= 1;
        }

        // read cols from GPIOs
        for (int i = (COL_PIN_COUNT-1); i >= 0; i--) {
            cols |= readPin(col_pins[i]);
            if (i < 5) {
                cols <<= 1;
            }
        }
        * /

        current_matrix[row] = cols;
        matrix_has_changed |= (last_row_value != current_matrix[row]);
    }
    */

    return matrix_has_changed;
}
