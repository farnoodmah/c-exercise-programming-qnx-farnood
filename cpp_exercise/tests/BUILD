load("@rules_cc//cc:defs.bzl", "cc_test")


cc_test(
    name = "linuxipc_test",
    srcs = ["linuxipc_test.cc"],
    deps = [
        "//src/lib:LINUXIPCLib",
         "//src/lib:FILEHANDLERLib",
          "//src/lib:IPCPIPELib",
          "//src/lib:IPCEXCEPTIONLib",
        "@googletest//:gtest_main",
    ],
)
