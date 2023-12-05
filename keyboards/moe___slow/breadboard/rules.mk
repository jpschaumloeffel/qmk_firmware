# This file intentionally left blank

CUSTOM_MATRIX = lite
SRC += breadboard/matrix.c

# UNICODEMAP_ENABLE = yes
UNICODE_COMMON = yes
UNICODE_ENABLE = yes

QUANTUM_LIB_SRC += i2c_master.c
OPT_DEFS += -DHAL_USE_I2C=TRUE
