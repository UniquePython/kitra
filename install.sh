#!/bin/bash
set -e

REPO="UniquePython/kitra"
INSTALL_INCLUDE="/usr/local/include/kitra"
INSTALL_LIB="/usr/local/lib"

# ── Detect platform ────────────────────────────────────────────────────────────

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux-x86_64"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macos-x86_64"
else
    echo "Unsupported OS: $OSTYPE"
    exit 1
fi

# ── Install SDL2 dependencies ──────────────────────────────────────────────────

echo "Installing SDL2 dependencies..."

if [[ "$PLATFORM" == "linux-x86_64" ]]; then
    if command -v apt &>/dev/null; then
        sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev libsdl2-ttf-dev libsdl2-mixer-dev
    elif command -v pacman &>/dev/null; then
        sudo pacman -S --noconfirm sdl2 sdl2_image sdl2_gfx sdl2_ttf sdl2_mixer
    elif command -v dnf &>/dev/null; then
        sudo dnf install -y SDL2-devel SDL2_image-devel SDL2_gfx-devel SDL2_ttf-devel SDL2_mixer-devel
    else
        echo "Unsupported Linux distro. Please install SDL2 and its extensions manually."
        exit 1
    fi
elif [[ "$PLATFORM" == "macos-x86_64" ]]; then
    if ! command -v brew &>/dev/null; then
        echo "Homebrew not found. Install it from https://brew.sh and re-run this script."
        exit 1
    fi
    brew install sdl2 sdl2_image sdl2_gfx sdl2_ttf sdl2_mixer
fi

# ── Download latest release tarball ───────────────────────────────────────────

echo "Downloading Kitra ($PLATFORM)..."

TARBALL="kitra-${PLATFORM}.tar.gz"
URL="https://github.com/${REPO}/releases/latest/download/${TARBALL}"

curl -sSL "$URL" -o "/tmp/$TARBALL"

# ── Extract ────────────────────────────────────────────────────────────────────

echo "Extracting..."
mkdir -p /tmp/kitra-install
tar -xzf "/tmp/$TARBALL" -C /tmp/kitra-install

# ── Install headers and library ───────────────────────────────────────────────

echo "Installing Kitra..."
sudo mkdir -p "$INSTALL_INCLUDE"
sudo cp -r /tmp/kitra-install/include/kitra/* "$INSTALL_INCLUDE/"
sudo cp /tmp/kitra-install/lib/libkitra.a "$INSTALL_LIB/"

# ── Clean up ───────────────────────────────────────────────────────────────────

rm -rf "/tmp/$TARBALL" /tmp/kitra-install

echo ""
echo "Kitra installed. You can now use #include <kitra/kitra.h> in your project."