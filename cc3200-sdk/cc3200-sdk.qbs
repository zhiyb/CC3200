import qbs

Product {
    name: "cc3200-sdk"
    cpp.optimization: project.optimization

    property var mcuflags: ["-mcpu=cortex-m4", "-mthumb", "-msoft-float", "-mfloat-abi=soft"]
    Depends {name: "cpp"}

    Export {
        Depends {name: "cpp"}
        cpp.includePaths: [product.sourceDirectory, product.sourceDirectory + "/inc"]
        cpp.commonCompilerFlags: ["-ffunction-sections", "-fdata-sections",
            "-Wno-unused-parameter", "-Wno-unused-variable"].concat(product.mcuflags)
        cpp.linkerFlags: ["-nostartfiles", "-nostdlib", "-static", "-lc", "-lm", "-lgcc",
            "-Wl,--gc-sections"].concat(product.mcuflags)
        cpp.defines: ["SYS_CLK=80000000", "gcc"]
        cpp.entryPoint: "ResetISR"
    }

    files: [
        "inc/asmdefs.h",
        "inc/hw_adc.h",
        "inc/hw_aes.h",
        "inc/hw_apps_config.h",
        "inc/hw_apps_rcm.h",
        "inc/hw_camera.h",
        "inc/hw_common_reg.h",
        "inc/hw_des.h",
        "inc/hw_dthe.h",
        "inc/hw_flash_ctrl.h",
        "inc/hw_gpio.h",
        "inc/hw_gprcm.h",
        "inc/hw_hib1p2.h",
        "inc/hw_hib3p3.h",
        "inc/hw_i2c.h",
        "inc/hw_ints.h",
        "inc/hw_mcasp.h",
        "inc/hw_mcspi.h",
        "inc/hw_memmap.h",
        "inc/hw_mmchs.h",
        "inc/hw_nvic.h",
        "inc/hw_ocp_shared.h",
        "inc/hw_shamd5.h",
        "inc/hw_stack_die_ctrl.h",
        "inc/hw_timer.h",
        "inc/hw_types.h",
        "inc/hw_uart.h",
        "inc/hw_udma.h",
        "inc/hw_wdt.h",
    ]
}
