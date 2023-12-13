#include <stdint.h>

#include "quantum.h"
#include "info_config.h"
#include "matrix.h"
#include "print.h"
#include "gpio.h"
#include "moe_i2c.h"

#define GPIO_ROW0 GP27  // different from layout, but same on kb2040 and sparkfun23
#define GPIO_ROW1 GP18  // 22 on SparkFun
#define GPIO_ROW2 GP4
#define GPIO_ROW3 GP7
#define GPIO_ROW4 GP6
#define GPIO_ROW5 GP5

#define INACTIVE_PIN_COUNT 1
static uint8_t inactive_pins[INACTIVE_PIN_COUNT] = { 2U };

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

#define GPINTENA 0x04 
#define GPINTENB 0x05

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  // debug_enable=true;
  // debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

static uint8_t mcp23017_init(void) {
    // default: iocon.bank = 0, iocon.seqop = 0
    // this is the "special mode" described in the datasheet which toggles between port a and port b reads

    uint8_t ret = 0;
    uint8_t data[4];

    ret = moe_i2c_read_reg(MCP23017_TWI_ADDRESS, IOCON, data, 2);
    ret = moe_i2c_read_reg(MCP23017_TWI_ADDRESS, IOCON, data, 2);

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0

    // set column pins as output
    data[0] = 0b00000000;  // IODIRA
    data[1] = 0b00000000;  // IODIRB
    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, IODIRA, data, 2);

    if (ret) goto out;  // make sure we got an ACK

    // configure no pull-ups for col pins
    data[0] = 0b00000000;
    data[1] = 0b00000000;

    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPPUA, data, 2);

    if (ret) goto out;  // make sure we got an ACK

    // disable interrupts
    data[0] = 0;
    data[1] = 0;
    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPINTENA, data, 2);

    // write ones to all columns (no column active)
    data[0] = 0x3F;
    data[1] = 0x3F;
    ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPIOA, data, 2);

 out:
    return ret;
}

void matrix_init_custom(void) {

    // pin configuration
    for (int i=0; i < INACTIVE_PIN_COUNT; i++) {
        palSetLineMode(inactive_pins[i], PAL_MODE_UNCONNECTED);
    }
    for (int i=0; i < MATRIX_ROWS; i++) {
        // row pins are inputs with pullup
        palSetLineMode(row_pins[i], PAL_MODE_OUTPUT_PUSHPULL);
        setPinInputHigh(row_pins[i]);
    }
    for (int i=0; i < COL_PIN_COUNT; i++) {
        // col pins are outputs and start all inactive (== high)
        palSetLineMode(col_pins[i], PAL_MODE_OUTPUT_PUSHPULL);
        setPinOutput(col_pins[i]);
        writePinHigh(col_pins[i]);
    }
    setPinOutput(MCP23017_RESET_GPIO);

    // init i2c
    moe_i2c_init();

    // reset and init mcp23017
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
        data[0] = 0x3F;
        data[1] = 0x3F;
        ret = moe_i2c_write_reg(MCP23017_TWI_ADDRESS, GPIOA, data, 2);
    }

    if (col > 0 && col <= COL_PIN_COUNT) {
        // deactivate previously inactive col GPIO
        writePinHigh(col_pins[col - 1]);
    }

    // write high column
    if (col < COL_PIN_COUNT) {
        // directly via pin
        writePinLow(col_pins[col]);
    } else {
        // set the col active via the port expander
        // first col starts near the MSB on each port
        data[0] = 0x3F;
        data[1] = 0x3F;
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

    return matrix_has_changed;
}
