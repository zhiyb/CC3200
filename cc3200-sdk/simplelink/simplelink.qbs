import qbs

Product {
    type: "staticlibrary"
    name: "cc3200-sdk-simplelink"
    cpp.includePaths: [".", "include"]
    cpp.defines: ["SL_FULL"]
    cpp.commonCompilerFlags: [
        "-Wno-missing-braces",
        "-Wno-missing-field-initializers",
        "-Wno-strict-aliasing",
    ]
    cpp.optimization: project.optimization

    Depends {name: "cc3200-sdk-driverlib"}
    Depends {name: "cc3200-sdk"}
    Depends {name: "cpp"}

    Properties {
        condition: project.target != "NONOS"
        cpp.defines: outer.concat(["SL_PLATFORM_MULTI_THREADED"])
    }

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: [product.sourceDirectory + "/include"]
        cpp.defines: product.cpp.defines
    }

    files: [
        "cc_pal.c",
        "cc_pal.h",
        "include/device.h",
        "include/fs.h",
        "include/netapp.h",
        "include/netcfg.h",
        "include/simplelink.h",
        "include/socket.h",
        "include/trace.h",
        "include/wlan.h",
        "include/wlan_rx_filters.h",
        "source/device.c",
        "source/driver.c",
        "source/driver.h",
        "source/flowcont.c",
        "source/flowcont.h",
        "source/fs.c",
        "source/netapp.c",
        "source/netcfg.c",
        "source/nonos.c",
        "source/nonos.h",
        "source/objInclusion.h",
        "source/protocol.h",
        "source/socket.c",
        "source/spawn.c",
        "source/spawn.h",
        "source/wlan.c",
        "user.h",
    ]
}
