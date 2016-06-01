import qbs

Product {
    type: "staticlibrary"
    name: "uart0"
    cpp.optimization: "small"
    Depends {name: "cpp"}
    Depends {name: "cc3200-sdk"}
    Depends {name: "cc3200-sdk-driverlib"}

    Properties {
        condition: project.uart0_baud
        cpp.defines: outer.concat(["UART0_BAUD=" + project.uart0_baud])
    }

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: ["."]
        cpp.defines: product.cpp.defines
    }

    files: [
        "*.c",
        "*.h",
    ]
}
