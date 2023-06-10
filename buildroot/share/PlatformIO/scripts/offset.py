#
# offset.py
#
# - If 'build.offset' is provided, either by JSON or by the environment...
#   - Set linker flag LD_FLASH_OFFSET and relocate the VTAB based on 'build.offset'.
#   - Set linker flag LD_MAX_DATA_SIZE based on 'build.maximum_ram_size'.
#   - Define STM32_FLASH_SIZE from 'upload.maximum_size' for use by Flash-based EEPROM emulation.
#
#
import pioutil
if pioutil.is_pio_build():
    import marlin

    env = marlin.env
    board = env.BoardConfig()
    board_keys = board.get("build").keys()

    #
    # For build.offset define LD_FLASH_OFFSET, used by ldscript.ld
    #
    if 'offset' in board_keys:
        LD_FLASH_OFFSET = board.get("build.offset")
        marlin.relocate_vtab(LD_FLASH_OFFSET)

        # Get upload.maximum_ram_size (defined by /buildroot/share/PlatformIO/boards/VARIOUS.json)
        maximum_ram_size = board.get("upload.maximum_ram_size")

        for i, flag in enumerate(env["LINKFLAGS"]):
            if "-Wl,--defsym=LD_FLASH_OFFSET" in flag:
                env["LINKFLAGS"][i] = "-Wl,--defsym=LD_FLASH_OFFSET=" + LD_FLASH_OFFSET
