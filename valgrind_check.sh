#!/bin/bash

# Valgrind Memory Leak Analysis Script
# Usage: ./valgrind_check.sh <executable> <leak_type>
# leak_type: all, definite, indirect, reachable

if [ $# -ne 2 ]; then
    echo "Usage: $0 <executable> <leak_type>"
    echo "leak_type options:"
    echo "  all       - show all leak types"
    echo "  definite  - show only definitely lost"
    echo "  indirect  - show only indirectly lost"
    echo "  reachable - show only still reachable"
    exit 1
fi

EXECUTABLE="$1"
LEAK_TYPE="$2"
SUPPRESSIONS=".valgrind-suppressions"

# Create logs directory if it doesn't exist
mkdir -p logs

# Find next run attempt number
RUN_NUM=1
while [ -f "logs/leak_${RUN_NUM}_${LEAK_TYPE}.log" ]; do
    ((RUN_NUM++))
done

echo "=== Valgrind Memory Leak Analysis (Run #$RUN_NUM) ==="
echo "Executable: $EXECUTABLE"
echo "Leak Type: $LEAK_TYPE"
echo "Suppressions: $SUPPRESSIONS"
echo ""

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable '$EXECUTABLE' not found!"
    exit 1
fi

# Check if suppressions file exists
if [ ! -f "$SUPPRESSIONS" ]; then
    echo "Warning: Suppressions file '$SUPPRESSIONS' not found!"
    SUPP_FLAG=""
else
    SUPP_FLAG="--suppressions=$SUPPRESSIONS"
fi

# Set leak kind based on input
case "$LEAK_TYPE" in
    "all")
        LEAK_KIND="all"
        ;;
    "definite")
        LEAK_KIND="definite"
        ;;
    "indirect")
        LEAK_KIND="indirect"
        ;;
    "reachable")
        LEAK_KIND="reachable"
        ;;
    *)
        echo "Error: Invalid leak type '$LEAK_TYPE'"
        echo "Valid options: all, definite, indirect, reachable"
        exit 1
        ;;
esac

LOG_FILE="logs/leak_${RUN_NUM}_${LEAK_TYPE}.log"

echo "Running Valgrind with leak type: $LEAK_TYPE"
echo "Output will be saved to: $LOG_FILE"
echo "============================================="

valgrind --leak-check=full --show-leak-kinds=$LEAK_KIND --track-origins=yes $SUPP_FLAG "$EXECUTABLE" 2>&1 | tee "$LOG_FILE"

echo ""
echo "=== Analysis Complete (Run #$RUN_NUM) ==="
echo "Log file created: $LOG_FILE"
echo ""
echo "=== LEAK SUMMARY ==="
grep -A 6 "LEAK SUMMARY:" "$LOG_FILE" | tail -7
