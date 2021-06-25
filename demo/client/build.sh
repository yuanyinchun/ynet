BUILD_TYPE=RelWithDebInfo
BUILD_DIR=build

cmake -S . -B ${BUILD_DIR} -G "Unix Makefiles" -DCMAKE_CONFIGURATION_TYPES="${BUILD_TYPE}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
cmake --build ${BUILD_DIR}/ --config ${BUILD_TYPE}
