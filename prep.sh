#!/bin/bash

TARGET_FOLDER="data"
ARCHIVE1="data_raw_32_error.zip"
ARCHIVE2="data_raw_32_ok.zip"

mkdir -p "$TARGET_FOLDER"

for ARCHIVE in "$ARCHIVE1" "$ARCHIVE2"; do
    if [[ -f "$ARCHIVE" ]]; then
        unzip "$ARCHIVE" -d "$TARGET_FOLDER"
        echo "Extracted $ARCHIVE to $TARGET_FOLDER"
    else
        echo "Archive not found: $ARCHIVE"
    fi
done