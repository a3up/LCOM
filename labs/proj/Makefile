# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = main.c interrupts.c graphics.c draw.c utils.c kbd.c sprite.c timer.c game.c chess.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DPROJ

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
