#!/bin/bash

# Define the directory to start from
start_dir="./src/spa/src"

# Define the Uncrustify configuration file
uncrustify_cfg="../../uncrustify.cfg"

# Recursively find all source files in the start directory
find "$start_dir" -type f \( -name "*.cpp" -o -name "*.h" \) | while read file
do
    # Run Uncrustify on each file
    uncrustify -c "$uncrustify_cfg" --replace --no-backup "$file"
done
