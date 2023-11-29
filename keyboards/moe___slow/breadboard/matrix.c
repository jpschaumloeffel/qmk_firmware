#include "gpio.h"
#include "wait.h"
#include "info_config.h"
#include "quantum.h"
#include "matrix.h"
#include "print.h"
#include "i2c_master.h"

/*
#define GPIO_ROW0 GP2
#define GPIO_ROW1 GP22
#define GPIO_ROW2 GP4
#define GPIO_ROW3 GP7
#define GPIO_ROW4 GP6

uint8_t row_pins[MATRIX_ROWS] = {GPIO_ROW0, GPIO_ROW1, GPIO_ROW2, GPIO_ROW3, GPIO_ROW4};

#define COL_PIN_COUNT 7
// col0..6 are on GPIOs directly
uint8_t col_pins[COL_PIN_COUNT] = { GP26, GP20, GP28, GP29, GP23, GP9, GP8 };
*/

#define MCP23017_TWI_ADDRESS (0x20 << 1)

#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01

#define GPPUA  0x0C  // GPIO pull-up resistor register
#define GPPUB  0x1D

#define GPIOA  0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB  0x13


static uint8_t mcp23017_init(void) {

    // default: iocon.bank = 0, iocon.seqop = 0
    // this is the "special mode" described in the datasheet which toggles between port a and port b reads

    uint8_t ret;
    uint8_t data[3];

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0
    data[0] = IODIRA;
    data[1] = 0b11111100;  // IODIRA
    data[2] = (0b11111111);  // IODIRB

    // ???
    ret = i2c_writeReg(MCP23017_TWI_ADDRESS, IODIRA, &data[1], 2, I2C_TIMEOUT);
    if (ret) goto out;  // make sure we got an ACK

    // set pull-up
    // - unused  : on  : 1
    // - input   : on  : 1
    // - driving : off : 0
    data[0] = GPPUA;
    data[1] = 0b11111111;  // IODIRA
    data[2] = (0b11111111);  // IODIRB

    ret = i2c_writeReg(MCP23017_TWI_ADDRESS, GPPUA, &data[1], 2, I2C_TIMEOUT);
    if (ret) goto out;  // make sure we got an ACK

out:
    return ret;
}

void matrix_init_custom(void) {
    // TODO: initialize hardware here

    /*
    // not sure if needed (?) copied from dilemma.c
    for (int i=0; i<MATRIX_ROWS; i++) {
        gpio_init(row_pins[i]);
        setPinInput(row_pins[i]);
    }
    for (int i=0; i<COL_PIN_COUNT; i++) {
        gpio_init(col_pins[i]);
        setPinInput(col_pins[i]);
    }
*/

#ifdef DEBUG
    debug_enable = true;
    debug_keyboard = true;
#endif


    setPinOutput(GP0);
    setPinOutput(GP1);
    setPinOutput(GP2);
    setPinOutput(GP3);

    wait_ms(3000);

    // reset mcp
    writePinHigh(GP1);
    wait_ms(10);
    writePinLow(GP1);
    wait_ms(10);
    writePinHigh(GP1);
    wait_ms(100);

    i2c_init();

    uprintf("i2c scan start\n");

    writePinHigh(GP0);
    wait_ms(10);
    writePinLow(GP0);
    wait_ms(10);
    writePinHigh(GP0);
    wait_ms(10);
    writePinLow(GP0);


    uint8_t data[2];
    i2c_status_t res;
    
    for (uint8_t test_addr = 0; test_addr <= 0x7F; test_addr++) {

        //res = i2c_transmit(test_addr << 1, &data, 1, I2C_TIMEOUT);
        res = i2c_readReg((test_addr << 1) | 1, 0, data, 2, I2C_TIMEOUT);

        if (res == 0) uprintf("i2c ping on %x, data %d\n", test_addr, data[0]);
        wait_ms(1);
    }


    uprintf("i2c scan stop\n");   

    writePinLow(GP0);
    wait_ms(10);
    writePinHigh(GP0);
    wait_ms(10);
    writePinLow(GP0);
    wait_ms(10);
    writePinHigh(GP0);
    wait_ms(10);
    writePinLow(GP0);
    wait_ms(10);

    mcp23017_init();

    writePinLow(GP0);
    wait_ms(10);
    writePinHigh(GP0);
    wait_ms(10);
    writePinLow(GP0);
    wait_ms(10);
    writePinHigh(GP0);
    wait_ms(10);
    writePinLow(GP0);
    wait_ms(10);
}

void matrix_set_row_status(uint8_t row) {
    // set only one row as output
    /*
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (row == i) {
            setPinOutput(row_pins[i]);
            writePinHigh(row_pins[i]);
        } else {
            setPinInput(row_pins[i]);
        }
    }
    */
    uint8_t data;

    data = 1 << row;
    i2c_writeReg(MCP23017_TWI_ADDRESS, GPIOA, &data, 1, I2C_TIMEOUT);
}


bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t last_row_value = current_matrix[row];
        matrix_row_t cols = 0;

        // enable row driver
        matrix_set_row_status(row);
        matrix_io_delay();

        uint8_t port_b;
        i2c_readReg(MCP23017_TWI_ADDRESS, GPIOB, &port_b, 1, I2C_TIMEOUT);
        cols = port_b & 0x03;

        /*
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
        */

        current_matrix[row] = cols;
        matrix_has_changed |= (last_row_value != current_matrix[row]);
    }

    return matrix_has_changed;
}
