#!/usr/bin/env bash
##############################################################################
# auto_wrap.sh
#   Recursively find YAML configs and generate RTL wrappers via wrapper.py
#
# Usage:
#   ./auto_wrap.sh [search_root]
#   search_root   Directory to start search (default: current dir)
##############################################################################

ROOT_DIR=.
TOOL_DIR=$ROOT_DIR/utility/wrapper
WRAPPER_FILE=$TOOL_DIR/gen_wrapper.py
CONF_STR_DIR=${1:-"./app/lenet"}/config/stream
CONF_MEM_DIR=${1:-"./app/lenet"}/config/mem
OUTPUT_DIR=build

wrap() {
    find "$1" -type f \( -name '*.yaml' -o -name '*.yml' \) | while read -r cfg; do
        dir=$(dirname "$cfg")
        base=$(basename "$cfg" | sed -E 's/\.(yaml|yml)//')
        out="$dir/${base}_wrapper.v"
        echo "Generating $out"
        python $WRAPPER_FILE \
            --config "$cfg" \
            --output "$2"
    done
}
wrap "$CONF_STR_DIR" "$OUTPUT_DIR"
wrap "$CONF_MEM_DIR" "$OUTPUT_DIR"
