#!/usr/bin/fish

set start (objdump -d -M intel famine | grep "_start>:" -n | cut -f1 -d' ' | cut -b 6-)
set end (objdump -d -M intel famine | tail -n1 | cut -f1 -d: | cut -b 5-)
set last_op (objdump -d -M intel famine | tail -n1 | cut -f2 | wc -w)

echo '#define _START_SIZE' (math --base=hex 0x$end - 0x$start + $last_op - 1)
