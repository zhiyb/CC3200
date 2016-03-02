import qbs

Product {
    type: "staticlibrary"
    name: "common"
    files: [
        "colours.c",
        "colours.h",
        "escape.h",
        "gpio_if.h",
        "macros.h",
        "timer_if.h",
    ]
    Depends {name: "cpp"}

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: [product.sourceDirectory]
    }
}
