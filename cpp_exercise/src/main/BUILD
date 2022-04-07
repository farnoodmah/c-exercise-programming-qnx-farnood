load("@rules_cc//cc:defs.bzl", "cc_binary")



cc_binary(
    name = "ipcsender",
    srcs = ["ipcsender.cc"],
    deps = ["//src/lib:LINUXIPCLib","//src/lib:IPCPIPELib","//src/lib:IPCMSGQLib","//src/lib:FILEHANDLERLib", "//src/lib:IPCEXCEPTIONLib"],
)

cc_binary(
    name = "ipcreceiver",
    srcs = ["ipcreceiver.cc"],
    deps = ["//src/lib:LINUXIPCLib","//src/lib:IPCPIPELib", "//src/lib:IPCMSGQLib","//src/lib:FILEHANDLERLib", "//src/lib:IPCEXCEPTIONLib"],
)