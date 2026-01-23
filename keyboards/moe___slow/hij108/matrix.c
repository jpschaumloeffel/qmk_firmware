#include <stdint.h>

#include "quantum.h"
#include "info_config.h"
#include "matrix.h"
#include "print.h"
#include "gpio.h"



#define INACTIVE_PIN_COUNT 3
static uint8_t inactive_pins[INACTIVE_PIN_COUNT] = { 9U, 10U, 19U };

static uint8_t row_pins[MATRIX_ROWS] = { GPIO_ROW0, GPIO_ROW1, GPIO_ROW2, GPIO_ROW3, GPIO_ROW4, GPIO_ROW5 };

#define COL_PIN_COUNT ((MATRIX_COLS+1) / 2)
// each col represents two actual columns
static uint8_t col_pins[COL_PIN_COUNT] = { GPIO_COL0, GPIO_COL2, GPIO_COL4, GPIO_COL6, GPIO_COL8, GPIO_COL10, GPIO_COL12, GPIO_COL14, GPIO_COL16 };

void matrix_activate_row(int row) {
    setPinOutput(row_pins[row]);
    writePinLow(row_pins[row]);
    matrix_io_delay();
}

void matrix_deactivate_row(int row) {
    setPinInput(row_pins[row]);
}

void matrix_activate_col(int col) {
    setPinOutput(col_pins[col]);
    writePinLow(col_pins[col]);
    matrix_io_delay();      
}

void matrix_deactivate_col(int col) {
    setPinInput(col_pins[col]);
}

void matrix_set_col2row(void) {
    // bring gpios to "col2row" configuration
    // diodes are configured like this:
    // COL_N --[>|]-- ROW_M

    // we have the columns as inputs with a pullup
    // rows are outputs and all set to 1 initially
    // in this state, we can't detect any keys yes

    for (int i=0; i < COL_PIN_COUNT; i++) {
        // col pins are inputs with pullup
        setPinInputHigh(col_pins[i]);
    }
    for (int i=0; i < MATRIX_ROWS; i++) {
        // row pins are outputs and start inactive (high)
        matrix_deactivate_row(i);
    }
}

void matrix_set_row2col(void) {
    // bring gpios to "row2col" configuration
    // diodes are configured like this:
    // COL_N --[|<]-- ROW_M

    for (int i=0; i < MATRIX_ROWS; i++) {
        // row pins are inputs with pullup
        setPinInputHigh(row_pins[i]);
    }
    for (int i=0; i < COL_PIN_COUNT; i++) {
        // col pins are outputs and start inactive (high)
        matrix_deactivate_col(i);
    }
}

bool matrix_read_row(int row) {
    return !readPin(row_pins[row]);;
}

bool matrix_read_col(int col) {
    return !readPin(col_pins[col]);
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
    // 1) configure pins for col2row-switches (even columns)
    // 2) activate even columns, read all rows
    // 3) configure pins row2col (odd columns)
    // 4) activate rows, read odd columns

    matrix_set_col2row();
    matrix_io_delay();

    // now cols are inputs with pullups. should read 1 if key not pressed

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

        // zero the row
        current_matrix[row] = 0;

        // enable output row
        matrix_activate_row(row);

        // read all cols
        for (uint8_t col = 0; col < COL_PIN_COUNT; col++) {
            // insert bit for EVEN COLUMN if col pulled low by pressed key
            bool active = matrix_read_col(col);
            int bit_idx = col * 2;
            if (active) {
                current_matrix[row] |= (1 << bit_idx);
            }
        }

        // disable output row
        matrix_deactivate_row(row);
    }

    matrix_set_row2col();
    matrix_io_delay();

    // now cols are outputs and write a 1
    // rows are pullup inputs

    for (uint8_t col = 0; col < COL_PIN_COUNT; col++) {

        // enable output col
        matrix_activate_col(col);

        // read all rows
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            // insert bit for ODD COLUMN if row pulled low by pressed key
            bool active = matrix_read_row(row);
            int bit_idx = col * 2 + 1;
            if (active) {
                current_matrix[row] |= (1 << bit_idx);
            }
        }

        // disable output col
        matrix_deactivate_col(col);
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
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}