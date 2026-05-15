{
  "targets": [
    {
      "target_name": "sha512_addon",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "src/addon.cpp",
        "src/sha2/sha256_init.cpp",
        "src/sha2/sha256_core.cpp",
        "src/sha2/sha512_init.cpp",
        "src/sha2/sha512_core.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}