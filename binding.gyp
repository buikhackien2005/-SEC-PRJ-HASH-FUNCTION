{
  "targets": [
    {
      "target_name": "sha512_addon",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "src/addon.cpp",
        "src/utils/padding_parsing.cpp",
        "src/sha2/sha_init.cpp",
        "src/sha2/sha512_core.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}