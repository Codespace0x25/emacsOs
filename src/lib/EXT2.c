#include "EXT2.h"
#include "ATA.h"
#include "Com1.h"
#include "Screen.h"
#include "std/mem.h"
#include "std/string.h"
#include "Def.h"
#include <stddef.h>
#include <stdint.h>

static Ext2Superblock sb;
static uint32_t block_size = 1024;
static uint32_t lba_start = 0;
static uint8_t ext2_device_index = 0;

// Corrected block LBA calculator
uint32_t get_block_lba(uint32_t block) {
    if (block_size == 1024)
      return lba_start + 2 + (block - 1); // SUPERBLOCK is block 1 -> LBA + 2
    return lba_start + block;
}
// mounst the ext2 part (doent work.)
bool ext2_mount(uint8_t ata_index, uint32_t lba) {
    ext2_device_index = ata_index;
    lba_start = lba;
    block_size = 1024;

    uint8_t buf[1024];
    uint32_t superblock_lba = get_block_lba(1); // Block 1 always contains superblock if 1024 bytes

    // Read 2 sectors (1024 bytes)
    for (int i = 0; i < 2; i++) {
        ata_pio_read_sector(ext2_device_index, superblock_lba + i, buf + (i * 512));
    }

    memcpy(&sb, buf, sizeof(Ext2Superblock));

    Kerrror("EXT2: Read superblock from LBA %d (device %d)\n", superblock_lba, ext2_device_index);
    Kerrror("EXT2: Magic = %x\n", sb.magic);

    if (sb.magic != EXT2_SUPER_MAGIC) {
        Kerrror("EXT2: mount failed: bad magic %x\n", sb.magic);
        return false;
    }

    block_size = 1024 << sb.log_block_size;

    Kerrror("EXT2: mounted on ata%d at LBA %d. Block size: %d\n", ata_index, lba, block_size);
    return true;
}
// read a inode form the file system. 
bool ext2_read_inode(uint32_t inode_num, Ext2Inode *inode) {
    uint32_t index = inode_num - 1;
    uint32_t inode_table_block = sb.first_data_block + 5; // Simplified assumption
    uint32_t offset = (index * sizeof(Ext2Inode)) % block_size;
    uint32_t block = inode_table_block + (index * sizeof(Ext2Inode)) / block_size;

    Kerrror("Reading inode %u at block %u (offset %u)\n", inode_num, block, offset);

    uint8_t buf[block_size];
    for (int i = 0; i < block_size / 512; i++) {
        ata_pio_read_sector(ext2_device_index, get_block_lba(block) + i, buf + i * 512);
    }

    memcpy(inode, buf + offset, sizeof(Ext2Inode));
    return true;
}

bool ext2_find_inode_by_path(const char *path, Ext2Inode *out_inode) {
    if (path[0] != '/')
        return false;

    Ext2Inode inode;
    if (!ext2_read_inode(2, &inode))
        return false; // Root inode

    char temp[256];
    strncpy(temp, path, sizeof(temp));
    char *token = strtok(temp, "/");

    while (token) {
        bool found = false;

        for (int i = 0; i < 12 && inode.i_block[i]; i++) {
            uint8_t buf[block_size];
            for (int s = 0; s < block_size / 512; s++) {
                ata_pio_read_sector(ext2_device_index, get_block_lba(inode.i_block[i]) + s, buf + s * 512);
            }

            uint32_t offset = 0;
            while (offset < block_size) {
                Ext2DirEntry *entry = (Ext2DirEntry *)&buf[offset];
                if (!entry->inode)
                    break;

                char name[256];
                memcpy(name, entry->name, entry->name_len);
                name[entry->name_len] = '\0';

                if (strcmp(name, token) == 0) {
                    if (!ext2_read_inode(entry->inode, &inode))
                        return false;
                    found = true;
                    break;
                }

                offset += entry->rec_len;
            }

            if (found)
                break;
        }

        if (!found)
            return false;
        token = strtok(NULL, "/");
    }

    *out_inode = inode;
    return true;
}

bool ext2_read_file(const Ext2Inode *inode, void *buffer, uint32_t buf_size) {
    uint8_t *buf = (uint8_t *)buffer;
    uint32_t size = inode->i_size;
    uint32_t to_read = (buf_size < size) ? buf_size : size;
    uint32_t total_read = 0;

    for (int i = 0; i < 12 && total_read < to_read; i++) {
        if (inode->i_block[i] == 0)
            break;

        uint8_t block[block_size];
        for (int s = 0; s < block_size / 512; s++) {
            ata_pio_read_sector(ext2_device_index, get_block_lba(inode->i_block[i]) + s, block + s * 512);
        }

        uint32_t copy = (to_read - total_read < block_size) ? (to_read - total_read) : block_size;
        memcpy(buf + total_read, block, copy);
        total_read += copy;
    }

    return true;
}

// Auto-detect and mount the EXT2 partition from MBR (this should. but it dosent)
bool parse_mbr(uint32_t drive_index) {
    uint8_t mbr_buf[512];
    ata_pio_read_sector(drive_index, 0, mbr_buf);

    MBR *mbr = (MBR *)mbr_buf;

    if (mbr->signature != 0xAA55) {
        Kerrror("MBR: Invalid MBR signature: %x\n", mbr->signature);
        return false;
    }

    for (int i = 0; i < 4; i++) {
        MBRPartitionEntry *entry = &mbr->partitions[i];

        Kerrror("MBR: Partition %d - Type: %d, LBA: %u\n", i, entry->partition_type, entry->start_lba);

        if (entry->partition_type == PARTITION_TYPE_EXT2) {
            Kerrror("note: Found EXT2 partition at LBA: %u\n", entry->start_lba);
            return ext2_mount(drive_index, entry->start_lba);
        }
    }

    Kerrror("No EXT2 partition found.\n");
    return false;
}

char* ext2_read_file_to_string(const Ext2Inode* inode, uint32_t* buf_size){
  
}
