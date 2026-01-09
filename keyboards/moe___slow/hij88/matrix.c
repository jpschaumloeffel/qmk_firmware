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


void matrix_set_col2row(void) {
    // bring gpios to "col2row" configuration
    // diodes are configured like this:
    // COL_N --[>|]-- ROW_M

    for (int i=0; i < MATRIX_ROWS; i++) {
        // row pins are inputs with pulldown
        setPinInputLow(row_pins[i]);
    }
    for (int i=0; i < COL_PIN_COUNT; i++) {
        // col pins are outputs and start inactive (low)
        setPinOutput(col_pins[i]);
        writePinLow(col_pins[i]);
    }
}

void matrix_set_row2col(void) {
    // bring gpios to "row2col" configuration
    // diodes are configured like this:
    // COL_N --[|<]-- ROW_M

    for (int i=0; i < MATRIX_ROWS; i++) {
        // row pins are outputs with and start inactive (low)
        setPinOutput(row_pins[i]);
        writePinLow(row_pins[i]);
    }
    for (int i=0; i < COL_PIN_COUNT; i++) {
        // col pins are inputs with pulldown
        setPinInputLow(col_pins[i]);
    }
}

void matrix_init_custom(void) {

    // pin configuration
    for (int i=0; i < INACTIVE_PIN_COUNT; i++) {
        palSetLineMode(inactive_pins[i], PAL_MODE_UNCONNECTED);
    }
    // just define something
    matrix_set_col2row();
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

    matrix_set_col2row();

    for (uint8_t col = 0; col < COL_PIN_COUNT; col++) {

        // enable output col
        writePinHigh(col_pins[col]);

        matrix_io_delay();

        // read all rows
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            // insert bit for EVEN COLUMN if row pulled high by pressed key
            bool active = readPin(row_pins[row]);
            int bit_idx = col * 2;
            if (active) {
                current_matrix[row] |= (1 << bit_idx);
            } else {
                // mask col
                current_matrix[row] &= ~(0x01 << bit_idx);
            }
        }

        // disable output col
        writePinLow(col_pins[col]);
    }

    matrix_set_row2col();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

        // enable output row
        writePinHigh(row_pins[row]);

        matrix_io_delay();

        // read all cols
        for (uint8_t col = 0; col < COL_PIN_COUNT - 1; col++) {
            // insert bit for EVEN COLUMN if row pulled high by pressed key
            bool active = readPin(col_pins[col]);
            int bit_idx = col * 2 + 1;
            if (active) {
                current_matrix[row] |= (1 << bit_idx);
            } else {
                // mask col
                current_matrix[row] &= ~(0x01 << bit_idx);
            }
        }

        // disable output row
        writePinLow(row_pins[row]);
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        if (last_matrix[row] != current_matrix[row]) {
            matrix_has_changed = true;
            break;
        }
    }

    return matrix_has_changed;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}