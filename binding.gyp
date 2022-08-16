{
  "targets": [
    {
      "target_name": "addon",
        'include_dirs': ["<!(node -p \"require('node-addon-api').include_dir\")"],
          'cflags': [ '-mxsave' ], #'-fno-exceptions'
  'cflags_cc': [ '-mxsave' ], #'-fno-exceptions'
  'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
  "sources": [ "src/AVXSupport.cpp"]
    }
  ]
}
