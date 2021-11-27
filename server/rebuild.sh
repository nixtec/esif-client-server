#!/bin/sh -x

make clean
make all
make delpass inspass enapass dispass showpassdb showregdb genpass
