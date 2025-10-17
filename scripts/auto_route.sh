#!/usr/bin/env bash
##############################################################################
# auto_route.sh
#   Automatically generate router file.
#
# Usage:
#   auto_route.sh
##############################################################################

ROUTER_FILE=utility/router/gen_router.py
OUTPUT_DIR=build

python $ROUTER_FILE \
    --output $OUTPUT_DIR \
    --N_IN 4 \
    --N_OUT 2
