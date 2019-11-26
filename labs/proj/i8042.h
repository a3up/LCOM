#pragma once

#ifndef BIT									// Bit function returns a number with an specific bit with a 1
#  define BIT(n) (1 << (n))
#endif

#define IRQ1 0x1 							// Keyboard IRQ line

#define OUT_BUF 0x60						// Out buffer port
#define STAT_REG 0x64						// Status register port

#define KBC_ARG_REG 0x60					// Argument register
#define KBC_CMD_REG 0x64					// Command register
#define KBC_SELF_TEST_OK 0x55

#define READ_COMMAND 0x20					// Read command
#define WRITE_COMMAND 0x60					// Write command
#define CHECK_KBC 0xAA						// Command to check KBC
#define CHECK_KBD_IF 0xAB					// Command to check the keyboard interface
#define DISABLE_KBD_IF 0xAD					// Command to disable the keyboard interface
#define ENABLE_KBD_IF 0xAE					// Command to enable the keyboard interface

#define ENABLE_INTERRUPT_MOUSE BIT(1)		// Enable mouse interruptions
#define ENABLE_INTERRUPT_KBD BIT(0)			// Enable keyboard interruptions

#define PARITY_ERROR BIT(7)					// Parity error
#define TIMEOUT_ERROR BIT(6)				// Timeout error
#define IN_BUF_STATUS BIT(1)				// In buffer status
#define OUT_BUF_STATUS BIT(0)				// Out buffer status

#define DELAY_US 20000						// Delay
#define ESC_BREAKCODE 0x81					// Esc breakcode
#define LONG_SCANCODE 0xe0					// First byte of a 2-byte scancode
