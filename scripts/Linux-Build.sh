#!/bin/sh

set -e
set -x

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

if [ ! -x "$SCRIPT" ]; then
	echo "Warning: build.sh is not marked executable. Run: chmod +x build.sh"
fi

set_default() {
	var_name="$1"
	default_value="$2"
	eval "[ -n \"\${$var_name+set}\" ]" || export "$var_name"="$default_value"
}

set_default ZENITH_DIR "$(realpath "$SCRIPTPATH/..")"
set_default BUILD_CONFIG "Debug"
set_default VULKAN_SDK "$(realpath "$ZENITH_DIR/Zenith/vendor/VulkanSDK/x86_64")"
set_default PREMAKE "$ZENITH_DIR/premake5"

if [ ! -x "$PREMAKE" ]; then
	echo "Premake not found or not executable at $PREMAKE"
	exit 1
fi

"$PREMAKE" gmake --cc=clang --verbose --file="$ZENITH_DIR/premake5.lua"
make -C "$ZENITH_DIR" config=$(echo "$BUILD_CONFIG" | tr '[:upper:]' '[:lower:]') "$@"
