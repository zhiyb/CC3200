import qbs

CppApplication {
    property string cc3200sdk: '../cc3200-sdk/'
    property string modules: '../modules/'
    property string entry: 'ResetISR'
    property stringList mcuflags: ['-mcpu=cortex-m4', '-mthumb',
        '-msoft-float', '-mfloat-abi=soft']

    cpp.optimization: 'small'
    cpp.commonCompilerFlags: ['-ffunction-sections', '-fdata-sections',
        '-Wno-unused-parameter', '-Wno-unused-variable', '-Wno-char-subscripts'].concat(mcuflags)
    cpp.defines: ['UART0_BAUD=115200', 'SYS_CLK=80000000', 'gcc']
    cpp.includePaths: [cc3200sdk + 'inc',
        cc3200sdk + 'driverlib',
        cc3200sdk + 'simplelink/include',
        modules + 'common',
        modules + 'rgbled',
        modules + 'uart0']
    cpp.libraryPaths: [cc3200sdk + 'simplelink/gcc/exe',
        cc3200sdk + 'driverlib/gcc/exe']
    cpp.staticLibraries: ['simplelink_nonos_opt', 'driver_opt']
    cpp.linkerFlags: ['-nostartfiles', '-nostdlib', '-static', '-lc', '-lm', '-lgcc',
        '-Wl,--gc-sections', '--entry=' + entry].concat(mcuflags)
    cpp.linkerScripts: ['cc3200.ld']

    files: [
        "../modules/common/colours.c",
        "../modules/common/colours.h",
        "../modules/common/escape.h",
        "../modules/common/gpio_if.h",
        "../modules/common/macros.h",
        "../modules/common/timer_if.h",
        "../modules/uart0/uart0.c",
        "../modules/uart0/uart0.h",
        "cc3200.ld",
        "device.c",
        "device.h",
        "events.c",
        "fs.c",
        "fs.h",
        "global.h",
        "main.c",
        "startup_gcc.c",
        "wlan.c",
        "wlan.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
    }
}
