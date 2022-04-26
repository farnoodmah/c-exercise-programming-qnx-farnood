load("@rules_cc//cc:defs.bzl", "cc_library")


cc_library(
    name = "LINUXIPCLib",
    srcs = ["linuxipclib.cc"],
    hdrs = ["linuxipclib.h","ipcmsgqlib.h", "ipcshmlib.h","ipcpipelib.h","filehandlerlib.h","ipcexceptionlib.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "IPCPIPELib",
    srcs = ["ipcpipelib.cc"],
    hdrs = ["ipcpipelib.h","filehandlerlib.h","ipcexceptionlib.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "FILEHANDLERLib",
    srcs = ["filehandlerlib.cc"],
    hdrs = ["filehandlerlib.h","ipcexceptionlib.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "IPCEXCEPTIONLib",
    hdrs = ["ipcexceptionlib.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "IPCMSGQLib",
    srcs = ["ipcmsgqlib.cc"],
    hdrs = ["ipcmsgqlib.h","filehandlerlib.h","ipcexceptionlib.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "IPCSHMLib",
    srcs = ["ipcshmlib.cc"],
    hdrs = ["ipcshmlib.h","filehandlerlib.h","ipcexceptionlib.h"],
    visibility = ["//visibility:public"]
)