import qbs

Product {
    type: "staticlibrary"
    name: "uart0"
    cpp.defines: if (project.uart0_baud) ["UART0_BAUD=" + project.uart0_baud]
    Depends {name: "cpp"}
    Depends {name: "cc3200-sdk"}
    Depends {name: "cc3200-sdk-driverlib"}

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: [product.sourceDirectory]
    }

    files: [
        "uart0.c",
        "uart0.h",
    ]
}
