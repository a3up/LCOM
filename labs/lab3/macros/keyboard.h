#pragma once

/// Keyboard IRQ line
#define KEYBOARD_IRQ_LINE       1

/// IBM's maximum keyboard response time
#define DELAY_US                20000
#define delay(n)                tickdelay(micros_to_ticks(n))
#define MAX_TRIES               10

/// Keyboard ports
#define STATUS_REGISTER         0x64
#define COMMAND_REGISTER        0x64
#define OUT_BUFFER              0x60
#define IN_BUFFER               0x60

/// Status byte
#define PARITY_ERROR            BIT(7)
#define TIMEOUT_ERROR           BIT(6)
#define MOUSE_DATA              BIT(5)
#define INHIBIT_FLAG            BIT(4)
#define A2_INPUT_LINE           BIT(3)
#define SYSTEM_FLAG             BIT(2)
#define FULL_IN_BUFFER          BIT(1)
#define FULL_OUT_BUFFER         BIT(0)

/// Keyboard commands
#define CMD_BYTE_READ           0x20
#define CMD_BYTE_WRITE          0x60
#define CHECK_KBC               0xAA
#define CHECK_KBD_IF            0xAB
#define DISABLE_KBD_IF          0xAD
#define ENABLE_KBD_IF           0xAE

/// Keyboard command byte
#define ENABLE_KBD_INT          BIT(4)
#define ENABLE_KBD_OUT_BUFFER   BIT(0)

/// Key codes
#define BREAKCODE               BIT(7)
#define LONG_SCANCODE           0xE0
#define ESC_MAKECODE            0x01
#define ESC_BREAKCODE           (BREAKCODE | ESC_MAKECODE)
