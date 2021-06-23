BUILD_TYPE=RelWithDebInfo
BUILD_DIR=build

cmake -S . -B ${BUILD_DIR} -G "Unix Makefiles" -DCMAKE_CONFIGURATION_TYPES="${BUILD_TYPE}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
cmake --build ${BUILD_DIR}/ --config ${BUILD_TYPE}

# make sure release folder exist
mkdir -p  ./release/ynet/include
mkdir -p  ./release/ynet/lib
# copy header file and lib file to release folder
cp ./src/ynet/tcp_server.h ./release/ynet/include/
cp ./build/libynet.a	./release/ynet/lib/