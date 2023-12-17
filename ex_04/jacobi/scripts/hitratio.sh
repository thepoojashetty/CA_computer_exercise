#!/bin/bash -l

INPUT_FILE="result_row.csv"
OUTPUT_FILE="result_row_hit_ratio.csv"
LOADS=$(cat "$INPUT_FILE" | grep MEM_UOPS_RETIRED_LOADS_ALL | grep STAT | cut -d , -f 3)
HITS=$(cat "$INPUT_FILE" | grep MEM_LOAD_UOPS_RETIRED_L1_HIT | grep STAT | cut -d , -f 3)
#divide corresponding elements of HITS by LOADS
HIT_RATIO=$(paste -d',' <(echo "$HITS") <(echo "$LOADS") | awk -F',' '{print $1/$2*100}')
ARRAY_SIZES=$(for i in {0..31}; do printf "%.0f\n" "$(echo "e($i*l(2^17)/31)" | bc -l)"; done)
echo "LOADS,HITS,HIT_RATIO,ARRAY_SIZE" > "$OUTPUT_FILE"
paste -d',' <(echo "$LOADS") <(echo "$HITS") <(echo "$HIT_RATIO") <(echo "$ARRAY_SIZES") >> "$OUTPUT_FILE"