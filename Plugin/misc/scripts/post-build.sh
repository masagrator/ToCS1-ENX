#!/bin/bash
set -e

OUT_NSO=${OUT}/${BINARY_NAME}
OUT_NPDM=${OUT}/main.npdm

# Clear older build.
rm -rf ${OUT}

# Create out directory.
mkdir ${OUT}

# Copy build into out
mv ${NAME}.nso ${OUT_NSO}
mv ${NAME}.npdm ${OUT_NPDM}

# Copy ELF to user path if defined.
if [ ! -z $ELF_EXTRACT ]; then
    cp "$NAME.elf" "$ELF_EXTRACT"
fi

# Ship the game's original NPDM when one is provided (keeps its exact permissions, incl. SD card access).
if [ -f "${PWD}/main.npdm" ]; then
    cp "${PWD}/main.npdm" ${OUT_NPDM}
fi
