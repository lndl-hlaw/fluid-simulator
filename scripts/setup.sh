#!/bin/bash

set -e

# Directories
DEPS_DIR="dependencies"
INCLUDE_DIR="$DEPS_DIR/include"
LIB_DIR="$DEPS_DIR/lib"
GLAD_TMP_DIR="temp_glad"

# Clean up previous temp files
rm -rf "$GLAD_TMP_DIR" glad.zip glad-master

echo "[INFO] Creating directory structure..."
mkdir -p "$INCLUDE_DIR/glad" "$INCLUDE_DIR/KHR" "$LIB_DIR"

echo "[INFO] Fetching glad (OpenGL loader)..."
curl -L https://github.com/Dav1dde/glad/archive/refs/heads/master.zip -o glad.zip
unzip -q glad.zip
cd glad-master

# Generate GLAD files with OpenGL core profile 4.6
python3 -m glad --generator=c --out-path="../$GLAD_TMP_DIR" --api="gl=4.6"
cd ..

# Move glad implementation files to correct locations
mv "$GLAD_TMP_DIR/include/glad/glad.h" "$INCLUDE_DIR/glad/"
mv "$GLAD_TMP_DIR/include/KHR/khrplatform.h" "$INCLUDE_DIR/KHR/"
mv "$GLAD_TMP_DIR/src/glad.c" "$LIB_DIR/"

echo "[INFO] Removing temporary files..."
rm -rf glad.zip glad-master "$GLAD_TMP_DIR"

echo "[INFO] Fetching glm (OpenGL mathematics)..."
GLM_DIR="$INCLUDE_DIR/glm"
if [ ! -d "$GLM_DIR" ]; then
    git clone --depth=1 https://github.com/g-truc/glm.git "$GLM_DIR"
    mv "$GLM_DIR/glm/"* "$GLM_DIR/"
    rm -rf "$GLM_DIR/.git"  # Optional: remove git metadata
fi

echo "[DONE] Dependencies are ready in '$DEPS_DIR'."
