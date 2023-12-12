
#define PIN_SCL GP0
#define PIN_SDA GP1

uint8_t moe_i2c_read_reg(uint8_t i2c_addr, uint8_t reg_addr, void* rxbuf, uint8_t len);
uint8_t moe_i2c_write_reg(uint8_t i2c_addr, uint8_t reg_addr, void* txbuf, uint8_t len);
void moe_i2c_init(void);
