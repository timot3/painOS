#ifndef SYSCALL_H
#define SYSCALL_H
#include "lib.h"
#include "terminal.h"
#include "filesys.h"
#include "rtc.h"

#define CMD_MAX_LEN 32
#define PROCESS_LIMIT 2
#define KERNEL_PAGE_BOT 0x800000
#define KERNEL_STACK_SIZE 8192

typedef struct file_op_table {
    int32_t (*open)(const uint8_t *filename);
    int32_t (*read)(int32_t fd, void *buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void *buf, int32_t nbytes);
    int32_t (*close)(int32_t fd);
} file_op_table_t;

typedef struct fd_items {
    file_op_table_t file_op_jmp;
    uint32_t inode;
    uint32_t file_position;
    uint32_t flags;
} fd_items_t;

typedef struct parent_pcb {
    uint32_t ksp;
    uint32_t kbp;
    uint32_t pid;
} parent_pcb_t;

typedef struct pcb {
    fd_items_t fd_items[MAX_OPEN_FILES];
    uint8_t argument_buf[CMD_MAX_LEN];
    uint32_t pid;
    parent_pcb_t parent;
    uint32_t esp;
    uint32_t ebp;
    uint32_t ss0;
    uint32_t esp0;
} pcb_t;

file_op_table_t rtc_table = {
    .open = rtc_open,
    .read = rtc_read,
    .write = rtc_write,
    .close = rtc_close
};

file_op_table_t file_table = {
    .open = file_open,
    .read = file_read,
    .write = file_write,
    .close = file_close
};

file_op_table_t dir_table = {
    .open = dir_open,
    .read = dir_read,
    .write = dir_write,
    .close = dir_close
};

file_op_table_t stdin_table = {
    .open = std_bad_call,
    .read = terminal_read,
    .write = std_bad_call,
    .close = std_bad_call
};

file_op_table_t stdout_table = {
    .open = std_bad_call,
    .read = std_bad_call,
    .write = terminal_write,
    .close = std_bad_call
};

int32_t halt (uint8_t status);
int32_t execute (const uint8_t* command);
int32_t read (int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t open (const uint8_t* filename);
int32_t close (int32_t fd);
int32_t getargs (uint8_t* buf, int32_t nbytes);
int32_t vidmap (uint8_t** screen_start);
int32_t set_handler (int32_t signum, void* handler_address);
int32_t sigreturn (void);

int assign_pid(void);
int unassign_pid(int pid);
int get_latest_pid();
pcb_t* get_pcb_addr(int pid);
pcb_t* allocate_pcb(int pid);
int parse_command(const uint8_t* command, pcb_t* pcb);

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

#define STDIN_IDX 0
#define STDOUT_IDX 1

#endif /* SYSCALL_H */
