load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "transfer",
    srcs = ["transfer.cc"],
    deps = ["//src/lib:TRANSFERLib"],
)


cc_binary(
    name = "ipcsender",
    srcs = ["ipcsender.cc"],
    deps = ["//src/lib:TRANSFERLib"],
)

cc_binary(
    name = "ipcreceiver",
    srcs = ["ipcreceiver.cc"],
    deps = ["//src/lib:TRANSFERLib"],
)