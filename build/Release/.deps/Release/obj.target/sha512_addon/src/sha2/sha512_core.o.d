cmd_Release/obj.target/sha512_addon/src/sha2/sha512_core.o := g++ -o Release/obj.target/sha512_addon/src/sha2/sha512_core.o ../src/sha2/sha512_core.cpp '-DNODE_GYP_MODULE_NAME=sha512_addon' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-DV8_DEPRECATION_WARNINGS' '-DV8_IMMINENT_DEPRECATION_WARNINGS' '-D_GLIBCXX_USE_CXX11_ABI=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-D__STDC_FORMAT_MACROS' '-DNAPI_DISABLE_CPP_EXCEPTIONS' '-DBUILDING_NODE_EXTENSION' -I/usr/include/nodejs/include/node -I/usr/include/nodejs/src -I/usr/include/nodejs/deps/openssl/config -I/usr/include/nodejs/deps/openssl/openssl/include -I/usr/include/nodejs/deps/uv/include -I/usr/include/nodejs/deps/zlib -I/usr/include/nodejs/deps/v8/include -I/home/bui-kien/Documents/SHA-GUI-App/node_modules/node-addon-api  -fPIC -pthread -Wall -Wextra -Wno-unused-parameter -fPIC -m64 -O3 -fno-omit-frame-pointer -fno-rtti -std=gnu++17 -MMD -MF ./Release/.deps/Release/obj.target/sha512_addon/src/sha2/sha512_core.o.d.raw   -c
Release/obj.target/sha512_addon/src/sha2/sha512_core.o: \
 ../src/sha2/sha512_core.cpp ../src/sha2/sha512_core.h \
 ../src/sha2/../utils/common.h ../src/sha2/sha_init.h \
 ../src/sha2/../utils/bit_operations.h
../src/sha2/sha512_core.cpp:
../src/sha2/sha512_core.h:
../src/sha2/../utils/common.h:
../src/sha2/sha_init.h:
../src/sha2/../utils/bit_operations.h:
