import qbs

Product {
    type: "staticlibrary"
    name: "cc3200-sdk-driverlib"
    Depends {name: "cc3200-sdk"}
    Depends {name: "cpp"}

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: [product.sourceDirectory]
    }

    files: [
        "adc.c",
        "adc.h",
        "aes.c",
        "aes.h",
        "camera.c",
        "camera.h",
        "cpu.c",
        "cpu.h",
        "crc.c",
        "crc.h",
        "debug.h",
        "des.c",
        "des.h",
        "flash.c",
        "flash.h",
        "gpio.c",
        "gpio.h",
        "hwspinlock.c",
        "hwspinlock.h",
        "i2c.c",
        "i2c.h",
        "i2s.c",
        "i2s.h",
        "interrupt.c",
        "interrupt.h",
        "pin.c",
        "pin.h",
        "prcm.c",
        "prcm.h",
        "rom.h",
        "rom_map.h",
        "rom_patch.h",
        "sdhost.c",
        "sdhost.h",
        "shamd5.c",
        "shamd5.h",
        "spi.c",
        "spi.h",
        "systick.c",
        "systick.h",
        "timer.c",
        "timer.h",
        "uart.c",
        "uart.h",
        "udma.c",
        "udma.h",
        "utils.c",
        "utils.h",
        "version.h",
        "wdt.c",
        "wdt.h",
    ]
}
