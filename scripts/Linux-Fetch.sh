#!/bin/sh

set -xe

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

if [ ! -x "$SCRIPT" ]; then
	echo "Warning: Linux-Fetch.sh is not marked executable. Run: chmod +x Linux-Fetch.sh"
fi

: "${ZENITH_DIR:=$(realpath "$SCRIPTPATH/..")}"
export ZENITH_DIR

: "${VULKAN_VERSION:=1.4.313.0}"
: "${PREMAKE_VERSION:=5.0.0-beta6}"

export VULKAN_VERSION
export PREMAKE_VERSION

VENDOR="$ZENITH_DIR/Zenith/vendor"
PREMAKE_PATH="$ZENITH_DIR/premake5"

# ==================
# Premake Setup
# ==================

if [ -n "$BUILD_PREMAKE" ]; then
	TEMP_DIR=$(mktemp -d)
	cd "$TEMP_DIR"

	curl -Lo premake.zip "https://github.com/premake/premake-core/releases/download/v$PREMAKE_VERSION/premake-$PREMAKE_VERSION-src.zip"
	unzip premake.zip
	cd "premake-$PREMAKE_VERSION-src"

	sh Bootstrap.sh
	mv bin/release/premake5 "$PREMAKE_PATH"

	cd "$SCRIPTPATH"
	rm -rf "$TEMP_DIR"
else
	curl -L "https://github.com/premake/premake-core/releases/download/v$PREMAKE_VERSION/premake-$PREMAKE_VERSION-linux.tar.gz" \
		| tar -xz -C "$ZENITH_DIR"
fi

chmod +x "$PREMAKE_PATH"

# ====================
# Vulkan SDK Setup
# ====================

curl -L "https://sdk.lunarg.com/sdk/download/$VULKAN_VERSION/linux/vulkansdk-linux-x86_64-$VULKAN_VERSION.tar.xz" | tar -xJ -C "$VENDOR"

mv "$VENDOR/$VULKAN_VERSION" "$VENDOR/VulkanSDK"
