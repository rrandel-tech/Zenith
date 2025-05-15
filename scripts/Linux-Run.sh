#!/bin/sh

set -xe

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

: "${ZENITH_DIR:=$(realpath "$SCRIPTPATH/..")}"
: "${BUILD_CONFIG:=Debug}"
: "${VULKAN_SDK:=$(realpath "$ZENITH_DIR/Zenith/vendor/VulkanSDK/x86_64")}"

export LD_LIBRARY_PATH="$VULKAN_SDK/lib:$ZENITH_DIR/Zenith/vendor/assimp/bin/linux"

cd "$ZENITH_DIR/Zenith-Editor"

if [ -z "$_EXEC" ]; then
	"$ZENITH_DIR/bin/${BUILD_CONFIG}-linux-x86_64/Zenith-Editor/Zenith-Editor" "$@"
else
	$_EXEC "$ZENITH_DIR/bin/${BUILD_CONFIG}-linux-x86_64/Zenith-Editor/Zenith-Editor" "$@"
fi