#!/bin/bash
set -x

../build/bellerophon -v -objs ./object/fap.o -p ./ -r ./aprxCode/flap_float_report.csv -t  FLAPAprx -P std.param 50000 
