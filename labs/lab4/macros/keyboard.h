#pragma once

/// Keyboard IRQ line
#define KEYBOARD_IRQ_LINE       1

/// Key codes
#define BREAKCODE               BIT(7)
#define LONG_SCANCODE           0xE0
#define ESC_MAKECODE            0x01
#define ESC_BREAKCODE           (BREAKCODE | ESC_MAKECODE)
