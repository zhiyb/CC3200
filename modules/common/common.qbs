import qbs

Product {
    type: "staticlibrary"
    name: "common"
    cpp.optimization: "small"
    Depends {name: "cpp"}
    Depends {name: "cc3200-sdk"}

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: ["."]
    }

    files: [
        "*.c",
        "*.h",
    ]
}
