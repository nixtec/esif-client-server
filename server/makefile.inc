OUTC   := ${patsubst %.c, %, ${wildcard *.c}}
OUTCC  := ${patsubst %.cc, %, ${wildcard *.cc}}
OBJC   := ${patsubst %.c, %.o, ${wildcard *.c}}
OBJCC  := ${patsubst %.cc, %.o, ${wildcard *.cc}}
OBJS   := ${patsubst %.s, %.o, ${wildcard *.s}}
CTOS   := ${patsubst %.c, %.s, ${wildcard *.c}}
S2LIST := ${patsubst %.s, %.list, ${wildcard *.s}}
OUTST  := ${patsubst %.c, %.static, ${wildcard *.c}}

all: ${OUTC} ${OUTCC}
# possible targets 'asm', 'lib', 'list', 'strip', 'clean', 'static'

# now define the rules
#
# *.c => *
%:%.c
	${CC} ${CFLAGS} -o $@ $< ${LDFLAGS}

# *.cc => *
%:%.cc
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}

# *.s => *
%:%.s
	${AS} ${ASFLAGS} -ahlms=$*.list -o $*.o $<
	${LD} -o $@ $*.o ${LDFLAGS}

# *.c => *.s
%.s:%.c
	${CC1} ${CC1FLAGS} -o $@ $<

# *.c => *.o
%.o:%.c
	${CC} ${CFLAGS} -c $<

# *.cc => *.o
%.o:%.cc
	${CXX} ${CXXFLAGS} -c $<

# *.s => *.o
%.o:%.s
	${AS} ${ASFLAGS} -ahlms=$*.list -o $*.o $<

# *.s => *.list
%.list:%.s
	${AS} ${ASFLAGS} -ahlms=$*.list -o $*.o $<

%.static:%.c
	${CC} -static ${CFLAGS} -o $@ $< ${LDFLAGS}
