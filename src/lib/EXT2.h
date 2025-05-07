#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EXT2_SUPER_MAGIC 0xEF53
#define PARTITION_TYPE_EXT2 0x83  // EXT2 partition type


// MBR Partition Table Entry (16 bytes)
typedef struct __attribute__((packed)) {
    uint8_t  boot_flag;         // Boot flag (0x80 for bootable)
    uint8_t  start_head;        // Starting head
    uint8_t  start_sector;      // Starting sector
    uint8_t  start_cylinder;    // Starting cylinder
    uint8_t  partition_type;    // Partition type (0x83 for Linux)
    uint8_t  end_head;          // Ending head
    uint8_t  end_sector;        // Ending sector
    uint8_t  end_cylinder;      // Ending cylinder
    uint32_t start_lba;         // Starting LBA of the partition
    uint32_t size_in_sectors;   // Size of the partition in sectors
} MBRPartitionEntry;

// MBR Structure (512 bytes)
typedef struct __attribute__((packed)) {
    uint8_t  bootstrap[446];     // Bootstrap code (unused)
    MBRPartitionEntry partitions[4]; // Four partitions
    uint16_t signature;          // Signature (0xAA55)
} MBR;


typedef struct __attribute__((packed)) {
    uint32_t inodes_count;
    uint32_t blocks_count;
    uint32_t reserved_blocks_count;
    uint32_t free_blocks_count;
    uint32_t free_inodes_count;
    uint32_t first_data_block;
    uint32_t log_block_size;
    uint32_t log_frag_size;
    uint32_t blocks_per_group;
    uint32_t frags_per_group;
    uint32_t inodes_per_group;
    uint32_t mtime;
    uint32_t wtime;
    uint16_t mount_count;
    uint16_t max_mount_count;
    uint16_t magic;
    uint16_t state;
    uint16_t errors;
    uint16_t minor_revision_level;
    uint32_t lastcheck;
    uint32_t checkinterval;
    uint32_t creator_os;
    uint32_t revision_level;
    uint16_t def_resuid;
    uint16_t def_resgid;
} Ext2Superblock;

typedef struct __attribute__((packed)) {
    uint16_t i_mode;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    uint32_t i_block[15];
} Ext2Inode;


typedef struct __attribute__((packed)) {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char name[];
} Ext2DirEntry;

bool ext2_mount(uint8_t ata_index, uint32_t lba_start);
bool ext2_read_inode(uint32_t inode_num, Ext2Inode* inode);
bool ext2_find_inode_by_path(const char* path, Ext2Inode* out_inode);
bool ext2_read_file(const Ext2Inode* inode, void* buffer, uint32_t buf_size);

bool parse_mbr(uint32_t drive_index);
