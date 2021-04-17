# mt7601u_eeprom

Editing EEPROM / eFuse for MT7601U USB WiFi Adapter

Fixes issue such as `Error: your device needs default EEPROM file and this driver doesn't support it!` when `kmod_mt7601u` is used with unprovisioned USB WiFi Adapter. Also allows tweaking of various parameters in EEPROM.

## Background
MT7601u has 512-byte of built-in efuses which can store various configurations for the adapter. The linux driver `kmod_mt7601u` relies on parsing those information as part of device initialization. It appears that some unbranded MT7601u adapters do not have this EEPROM filled in. They might still work with the vendor-provided driver as those ship with a default `MT7601EEPROM.bin` file, but `kmod_mt7601u` will simply refuse to work. This tool aim to fix the issue by provisioning those devices with appropriate EEPROM. YOu can also use this tool to tweak existing content in EEPROM, such as MAC address.

## Usage
1. Install the kernel headers for the current running device. For Debian-based system, this will be `sudo apt-get install linux-headers-generic`
2. Compile the patched kmod_mt7601u driver and the tool itself in this repository: `make all`

3. Unplug the device if already plugged in. Replace the kernel driver with the one just compiled: `sudo rmmod mt7601u; sudo modprobe mac80211; sudo insmod mt7601u/mt7601u.ko`
4. Plug in the device. The modified driver should load successfully.
5. Make sure the debugfs exposed by the modified driver exists: `sudo find /sys/kernel/debug/ieee80211/ -name regidx` Make a note of the phy's name in the result.
6. Flash the device: first try a dry run with `sudo ./mt7601 <phy_from_previous_step> flash_test eeprom.bin`. If nothing is wrong, replace `flash_test` with `flash_real` to flash for real.

## Technical Information

Most of the information here is derived from the official vendor driver ([1]) which contains logic to program the efuse. Additional information about the register operations can be gathered from datasheet of a similar chip RT5370N ([2]). The efuse is one-time programmable (i.e. 0 can be flipped to 1, but not the other way around) so be careful when experimenting with it.

### EEPROM format

There are two views to the 0x200-byte efuse, the logical view and the  physical view. In the physical view, the address space is uniform from `0` to `0x200`. Most of the physical addresss can store arbitrary data, except between `0x1e0` and `0x1fc`, where the virtual address map (`MT_EE_USAGE_MAP`) lives. The logical view spans between `0` and `0x1d0`, and this is the view through which `kmod_mt7601u` access the configuration data (on-disk eeprom file is also laid out in the logical view address). In the logical view, bytes are grouped into 16-byte unit (let's call it a block), and depending on whether there is an entry in the virtual address map, some blocks are considered unallocated by the chip (in this case `kmod_mt7601u` treats the block as filled with `FF`). Let's have a look at an example virtual address map to understand how it works:
```
01e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 98 97
01f0: d6 d5 94 8e 8d 07 46 45 04 c3 82 81 c0 00 00 00
```

The basic idea is the `i`th byte (tag) in the virtual address map (starting from `0x1e0`) tells you which virtual address block the `i`th physical block maps to. The lower 6-bit of the tag equals the virtual address divided by 16 (i.e. address in unit of blocks) and the top 2-bit is a checksum of the lower 6-bit. For example, at `0x1f1` (`0x1e0` + **17**) the tag is `d5`, this means the physical address range `0x110` (**17** * 16) - `0x120` maps to the logical address `0x100` (low_6_bit(`0xd5`) * 16) - `0x110`.

It's assumed that if multiple tags mapping to the same virtual address box exist, the first one will take precedence. That's inferred from the fact that the official driver tries to allocate tags in reverse order starting from `0x1fe`. This would have allowed updating the existing EEPROM content (remember the efuse is one-time programmable so you cannot always update its content in place) by writing to a new physical block in lower address and setting its virtual address map to the same logical adress that needs updating, so the later physical bock is effectively shadowed.

### Program the EEPROM

To program the EEPROM, we need to access various device registers. This can be done from the kernel module itself, or in our case, from userland through the raw register access points exposed by `kmod_mt7601u`'s debugfs. In the repository we ship a modified version of `kmod_mt7601u` which has a dummy EEPROM content built-in so we could at least load the driver successfully and expose the device registers.

## Reference
1. [Official Ralink efuse driver code](https://github1s.com/eywalink/mt7601u/blob/master/src/common/ee_efuse.c)
2. [RT5370N Datasheet](https://www.acmesystems.it/www/WIFI-2/RT5370N_datasheet.pdf)