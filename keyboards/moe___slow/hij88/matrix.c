#include <stdint.h>

#include "quantum.h"
#include "info_config.h"
#include "matrix.h"
#include "print.h"
#include "gpio.h"



#define INACTIVE_PIN_COUNT 3
static uint8_t inactive_pins[INACTIVE_PIN_COUNT] = { 0U, 1U, 9U };

static uint8_t row_pins[MATRIX_ROWS] = { GPIO_ROW0, GPIO_ROW1, GPIO_ROW2, GPIO_ROW3, GPIO_ROW4, GPIO_ROW5 };

#define COL_PIN_COUNT ((MATRIX_COLS+1) / 2)
// each col represents two actual columns
static uint8_t col_pins[COL_PIN_COUNT] = { GPIO_COL0, GPIO_COL2, GPIO_COL4, GPIO_COL6, GPIO_COL8, GPIO_COL10, GPIO_COL12, GPIO_COL14, GPIO_COL16 };


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

}

void matrix_set_col_status(uint8_t col) {

    /*
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
    */
}


bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;

    // save entire matrix
    // could probably eliminate with some bit masking below
    matrix_row_t last_matrix[MATRIX_ROWS];
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        last_matrix[row] = current_matrix[row];
    }

    // scan matrix
    // 1) configure pins col2row
    // 2) activate even columns, read all rows
    // 3) configure pins row2col
    // 4) activate rows, read uneven columns



    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        if (last_matrix[row] != current_matrix[row]) {
            matrix_has_changed = true;
            break;
        }
    }

    return matrix_has_changed;
}
