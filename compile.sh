#!/bin/bash
qmake && \
make && \
make clean && \
echo -e "COMPILATION COMPLETE! \a"
