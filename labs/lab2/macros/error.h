#pragma once
#define panic(message) { printf("\033[0;36m%s: %s\n\033[0m", __func__, message); return 1; }
#define error(expression, message) { if(expression) panic(message); }
