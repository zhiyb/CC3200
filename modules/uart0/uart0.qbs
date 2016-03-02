import qbs

Product {
    type: "staticlibrary"
    name: "uart0"
    cpp.optimization: project.optimization
    Depends {name: "cpp"}
    Depends {name: "cc3200-sdk"}
    Depends {name: "cc3200-sdk-driverlib"}

    Properties {
        condition: project.uart0_baud
        cpp.defines: outer.concat(["UART0_BAUD=" + project.uart0_baud])
    }

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: [product.sourceDirectory]
    }

    files: [
        "uart0.c",
        "uart0.h",
    ]
}
