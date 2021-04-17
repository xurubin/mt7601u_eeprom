#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define REGVAL_S "/sys/kernel/debug/ieee80211/%s/mt7601u/regval"
#define REGIDX_S "/sys/kernel/debug/ieee80211/%s/mt7601u/regidx"
char regval[256];
char regidx[256];

FILE* fRegVal;
FILE* fRegIdx;
uint32_t curRegIndex = -1;

#define INVALID_ADDRESS 0xffff
#define INVALID_BLOCK   0xff

#define MT_EFUSE_CTRL			0x0024
#define MT_EFUSE_DATA_BASE		0x0028
#define MT_EE_USAGE_MAP_START   0x1e0
#define MT_EE_USAGE_MAP_END     0x1fc
#define MT_EE_USAGE_MAP_SIZE    (MT_EE_USAGE_MAP_END - MT_EE_USAGE_MAP_START + 1)

typedef	union	_EFUSE_CTRL_STRUC {
	struct	{
		uint32_t            EFSROM_AOUT:6;
		uint32_t            EFSROM_MODE:2;
		uint32_t            EFSROM_LDO_OFF_TIME:6;
		uint32_t            EFSROM_LDO_ON_TIME:2;
		uint32_t            EFSROM_AIN:10;
		uint32_t            RESERVED:4;
		uint32_t            EFSROM_KICK:1;
		uint32_t            SEL_EFUSE:1;
	}	field;
	uint32_t			word;
}	EFUSE_CTRL_STRUC, *PEFUSE_CTRL_STRUC;


enum mt7601u_eeprom_access_modes {
	MT_EE_READ = 0,
	MT_EE_PHYSICAL_READ = 1,
	MT_EE_PHYSICAL_WRITE = 3,
};

void updateRegIndex(uint32_t reg) {
    if (curRegIndex == reg) return;
    curRegIndex = reg;
    FILE* fRegIdx = fopen(regidx, "w");
    if (!fRegIdx) {
        printf("failed to open regidx\n");
        return;
    }

    char buf[32];
    sprintf(buf, "%lu", reg);
    if (strlen(buf) != fwrite(buf, 1, strlen(buf), fRegIdx)) {
        printf("failed to write regIdx\n");
    }
    fclose(fRegIdx);
}

static inline uint32_t mt76_rr(uint32_t reg) {
    updateRegIndex(reg);
    char buf[32];
    FILE* fRegVal = fopen(regval, "r");
    if (!fRegVal) {
        printf("failed to open regval\n");
        return -1;
    }
    int n = fread(buf, 1, sizeof(buf), fRegVal);
    fclose(fRegVal);

    buf[n] = '\0';
    if (n < 10) {
        printf("failed to read regVal(%x): %s\n", curRegIndex, buf);
        return 0;
    } else {
        //printf("read regVal(%x) return: %s", curRegIndex, buf);
        return strtoul(buf, NULL, 0);
    }
}

static inline bool mt76_wr(uint32_t reg, uint32_t val) {
    updateRegIndex(reg);
    FILE* fRegVal = fopen(regval, "w");
    if (!fRegVal) {
        printf("failed to open regval\n");
        return false;
    }
    char buf[32];
    sprintf(buf, "0x%08llx", val);
    if (strlen(buf) != fwrite(buf, 1, strlen(buf), fRegVal)) {
        printf("failed to write regVal\n");
    }
    fclose(fRegVal);
    return true;
}

static uint32_t mt76_poll() {
	EFUSE_CTRL_STRUC val;
    while (1) {
        val.word = mt76_rr(MT_EFUSE_CTRL);
/*
    printf("[A] %.8x %d %d %d %d %d %d %d %d\n", val, 
            val.field.SEL_EFUSE,
            val.field.EFSROM_KICK,
            val.field.RESERVED,
            val.field.EFSROM_AIN,
            val.field.EFSROM_LDO_ON_TIME,
            val.field.EFSROM_LDO_OFF_TIME,
            val.field.EFSROM_MODE,
            val.field.EFSROM_AOUT);
*/
        if (val.field.EFSROM_KICK == 0) {
            break;
        }
        usleep(20000); // 20ms
    }
    return val.word;
}

// Read 16 bytes to address. addr is clamped to be 16-byte aligned.
static int
mt7601u_efuse_read(uint32_t addr, uint8_t *data,
		   enum mt7601u_eeprom_access_modes mode)
{
	EFUSE_CTRL_STRUC val;
	int i;

	val.word = mt76_rr(MT_EFUSE_CTRL);
    val.field.EFSROM_AIN = addr & ~0xf;
    val.field.EFSROM_MODE = mode;
    val.field.EFSROM_KICK = 1;
    val.field.EFSROM_LDO_ON_TIME = 2;
    val.field.EFSROM_LDO_OFF_TIME = 8;
    mt76_wr(MT_EFUSE_CTRL, val.word);

    val.word = mt76_poll();

    if (val.field.EFSROM_AOUT == 0x3F) {
		/* Parts of eeprom not in the usage map (0x80-0xc0,0xf0)
		 * will not return valid data but it's ok.
		 */
		memset(data, 0xff, 16);
		return 0;
	}
    /*
    printf("mt7601u_efuse_read: %.8x %s addr %.4x AOUT %.4x\n", 
            val.word,
            mode == MT_EE_READ ? "logical" : "physical",
            addr, val.field.EFSROM_AOUT);
    */
	for (i = 0; i < 4; i++) {
		uint32_t word = mt76_rr(MT_EFUSE_DATA_BASE + (i << 2));
        memcpy(data + i*4, &word, 4);
	}

	return 16;
}

// Write 16 bytes to physical address. addr must be 16-byte aligned.
static bool
mt7601u_efuse_write_physical(uint32_t addr, uint8_t *data)
{
    int i;
    
	for (i = 0; i < 4; i++) {
        uint32_t word;
        memcpy(&word, data + i*4, 4);
		if (!mt76_wr(MT_EFUSE_DATA_BASE + (i << 2), word)) return false;
	}

	EFUSE_CTRL_STRUC val;
	val.word = mt76_rr(MT_EFUSE_CTRL);
    val.field.EFSROM_AIN = addr & ~0xf;
    val.field.EFSROM_MODE = MT_EE_PHYSICAL_WRITE;
    val.field.EFSROM_KICK = 1;
    val.field.EFSROM_LDO_ON_TIME = 2;
    val.field.EFSROM_LDO_OFF_TIME = 8;
    mt76_wr(MT_EFUSE_CTRL, val.word);

    mt76_poll();

    uint8_t readback[16];
    if (16 != mt7601u_efuse_read(addr, readback, MT_EE_PHYSICAL_READ)) {
        printf("Readback incorrect length\n");
        return false;
    }
    for (i = 0; i < 16; i++) {
        if (readback[i] != data[i]) {
            printf("Readback incorrect data: exp %.2x act %.2x\n", data[i], readback[i]);
            return false;
        }
    }
    return true;
}

void dumpEEPROM(bool physical) {
    uint8_t data[16];
    printf("DUMP %s:\n", physical ? "physical" : "logical");
    for (int addr = 0; addr < 0x200; addr += 16) {
        printf("%.4x: ", addr);
        int len = mt7601u_efuse_read(addr, data, physical ? MT_EE_PHYSICAL_READ : MT_EE_READ);
        //printf("Read back %d bytes: ", len); 
        for (int i = 0; i < 16; i++) {
            if (i < len) {
                printf("%.2x ", data[i]);
            } else {
                printf("?? ");
            }
        }
        printf("\n");
    }
}

uint8_t crc(uint16_t addr) {
    /*convert the address from 10 to 8 bit ( bit7, 6 = parity and bit5 ~ 0 = bit9~4), and write to logical map entry*/
    addr >>= 4;
    uint8_t r = 0;

    r |= ((~((addr & 0x01) ^ ( addr >> 1 & 0x01) ^  (addr >> 2 & 0x01) ^  (addr >> 3 & 0x01))) << 6) & 0x40;
    r |= ((~((addr >> 2 & 0x01) ^ (addr >> 3 & 0x01) ^ (addr >> 4 & 0x01) ^ ( addr >> 5 & 0x01))) << 7) & 0x80;
    return r;
}
uint16_t tagToAddress(uint8_t tag) {
    uint16_t addr = (tag & 0x3f) << 4;
    if (crc(addr) == (tag & 0xc0)) {
        return addr;
    } else {
        return INVALID_ADDRESS;
    }
}

uint8_t addressToTag(uint16_t address) {
    return (0x3f & (address >> 4)) | crc(address);
}

uint8_t getBlockNum(uint8_t *map, uint32_t logicalAddr) {
    for (int i = 0; i < MT_EE_USAGE_MAP_SIZE; i++) {
        if (tagToAddress(map[i]) == (logicalAddr & 0xfff0)) {
            return i;
        }
    }
    return INVALID_BLOCK;
}

uint8_t findFreeBlock(uint8_t *map) {
    for (int i = MT_EE_USAGE_MAP_SIZE - 1; i >= 0; i--) {
        if (map[i] == 0) return i;
    }
    return INVALID_BLOCK;
}

bool canUpdate(uint8_t *src, uint8_t *dst) {
    for (int i = 0; i < 16; i++) {
        // 1 cannot be flashed back to 0
        if ((src[i] & dst[i]) != src[i]) return false;
    }
    return true;
}

void printBlock(uint8_t* data, uint16_t addr) {
    printf("%.4x: ", addr);
    for (int i = 0; i < 16; i++) {
        printf("%.2x ", data[i]);
    }
    printf("\n");
}

void save(const char* fname) {
    FILE* fp = fopen(fname, "wb");
    uint8_t data[16];
    // Since kmod_7601u only consumes first 256 bytes, limit the dump to 256 bytes.
    for (int addr = 0; addr < 0x100; addr += 16) {
        mt7601u_efuse_read(addr, data, MT_EE_READ);
        fwrite(data, 1, sizeof(data), fp);
    }
    fclose(fp);
}

bool isAllFF(uint8_t* data) {
    for (int i = 0; i < 16; i++) {
        if (data[i] != 0xFF) return false;
    }
    return true;
}

bool flash(const char* fname, bool forReal) {
    // Since kmod_7601u only consumes first 256 bytes, limit the dump to 256 bytes.
    uint8_t eeprom[0x100];
    FILE* fp = fopen(fname, "rb");
    fread(eeprom, 1, sizeof(eeprom), fp);
    fclose(fp);

    uint8_t current[0x200];
    uint8_t updated[0x200];
    for (int addr = 0; addr < 0x200; addr += 16) {
        mt7601u_efuse_read(addr, current + addr, MT_EE_PHYSICAL_READ);
        //printBlock(current + addr, addr);
    }
    memcpy(updated, current, sizeof(current));
    uint8_t *usageMap = updated + MT_EE_USAGE_MAP_START;

    for (int laddr = 0; laddr < sizeof(eeprom); laddr += 16) {
        uint8_t blkNum = getBlockNum(usageMap, laddr);
        printf("laddr %x blk %d\n", laddr, blkNum);
        if (blkNum == INVALID_BLOCK) {
            // Skip empty block 
            if (isAllFF(eeprom + laddr)) continue;
            // Else we need to write to a new block
        } else {
            //printBlock(current + blkNum * 16,  blkNum * 16);
            //printBlock(eeprom + laddr, laddr);
            if (!canUpdate(current + blkNum * 16, eeprom + laddr)) {
                // Can't modify existing block in place, need a new block
                blkNum = INVALID_BLOCK;
            }
        }
        if (blkNum == INVALID_BLOCK) {
            blkNum = findFreeBlock(usageMap);
        }
        if (blkNum == INVALID_BLOCK) {
            printf("No more free usage map slot.\n");
            return false;
        }
        memcpy(updated + blkNum * 16, eeprom + laddr, 16);
        usageMap[blkNum] = addressToTag(laddr);
    }
    for (int paddr = 0; paddr < 0x200; paddr += 16) {
        if (!memcmp(current + paddr, updated + paddr, 16)) continue;
        printf("-"); printBlock(current + paddr, paddr);
        printf("+"); printBlock(updated + paddr, paddr);
        printf("--\n");
        if (!canUpdate(current + paddr, updated + paddr)) {
            printf("Invalid update.");
            return false;
        }
    }
    if (forReal) {
        for (int paddr = 0; paddr < 0x200; paddr += 16) {
            if (!memcmp(current + paddr, updated + paddr, 16)) continue;
            printf("Writing %.4x...", paddr);
            if (mt7601u_efuse_write_physical(paddr, updated + paddr)) {
                printf("OK.\n");
            } else {
                printf("Failed.\n");
                return false;
            }
        }
    }
    return true;
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Missing phy argument\n");
        return -1;
    }
    if (argc < 3) {
        printf("Missing cmd argument\n");
        return -1;
    }
    sprintf(regval, REGVAL_S, argv[1]);
    sprintf(regidx, REGIDX_S, argv[1]);
    if (!strcmp(argv[2], "dump")) {
        if (argc < 4) {
            printf("Missing dump option\n");
            return -1;
        }
        if (!strcmp(argv[3], "physical")) {
            dumpEEPROM(true);
        } else {
            dumpEEPROM(false);
        }
        return 0;
    }
    if (!strcmp(argv[2], "flash_test") || !strcmp(argv[2], "flash_real") ) {
        if (argc < 4) {
            printf("Missing flash file\n");
            return -1;
        }
        flash(argv[3], !strcmp(argv[2], "flash_real"));
        return 0;
    }
    if (!strcmp(argv[2], "save")) {
        if (argc < 4) {
            printf("Missing save file\n");
            return -1;
        }
        save(argv[3]);
        return 0;
    }
    return 0;
}