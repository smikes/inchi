{
  "target_defaults": {
    "conditions": [
      ['OS=="win"', {
      }, {
        'cflags'    : [ "-fexceptions" ],
        'cflags_cc' : [ "-fexceptions" ]
      } ]
    ],
    "configurations": {
      "Release": {
        'msvs_settings': {
          'VCCLCompilerTool': {
            'WholeProgramOptimization': 'true', # /GL, whole program optimization, needed for LTCG
            'OmitFramePointers': 'true',
            'EnableFunctionLevelLinking': 'true',
            'EnableIntrinsicFunctions': 'true',
            'RuntimeTypeInfo': 'false',
            'ExceptionHandling': '1',
            'AdditionalOptions': [
              '/MP'
            ]
          },
          'VCLibrarianTool': {
            'AdditionalOptions': [
              '/LTCG', # link time code generation
            ],
          },
          'VCLinkerTool': {
            'LinkTimeCodeGeneration': 1, # link-time code generation
            'OptimizeReferences': 2, # /OPT:REF
            'EnableCOMDATFolding': 2, # /OPT:ICF
            'LinkIncremental': 1, # disable incremental linking
          }
        }
      }
    }
  },
  "targets": [
    {
      "target_name": "inchi",
      "msvs_guid": "F1B917E2-75AB-A243-6D62-3C7938A1EF68",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "dependencies": [
        "deps/inchi/inchi.gyp:libINCHIAPI"
      ],
      "sources": [
        "src/node-inchi.cc",
        "src/molecule.cc",
        "src/atom.cc",
        "src/molecule_wrap.cc",
        "src/molecule_native.cc",
        "src/inchi_atom.cc",
        "src/get_inchi_data.cc",
        "src/get_struct_from_inchi_data.cc",
        "src/get_inchi.cc"
      ],
      "conditions": [
        ['OS=="win"', {
        }, {
          'cflags_cc' : [
            "-fexceptions"
          ]
        } ]
      ],
    },
    {
      "target_name": "test",
      "type": "executable",
      "sources": [
        "src/test/TestMain.cc",
        "src/test/hello.cc",
        "src/test/test_molecule.cc",
        "src/test/test_inchi_atom.cc",
        "src/test/test_get_struct_from_inchi.cc",
        "src/molecule_native.cc",
        "src/get_inchi_data.cc",
        "src/get_struct_from_inchi_data.cc",
        "src/inchi_atom.cc"
      ],
      "include_dirs": [
        ".",
        "src",
        "<!(node -e \"require('cppunitlite')\")",
        "<!(node -e \"require('nan')\")"
      ],
      "dependencies": [
        "node_modules/cppunitlite/binding.gyp:CppUnitLite",
        "deps/inchi/inchi.gyp:libINCHIAPI"
      ],
      "conditions": [
        ['OS=="win"', {
          }, {
            'cflags_cc': [ '-fexceptions' ]
          }
        ]
      ],
      # sample unit test
    }
  ]
}
