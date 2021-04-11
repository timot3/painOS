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
#define DELETE 0x7F
#define E 0x45
#define L 0x4c
#define F 0x46


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
    uint8_t argument_buf[TERM_BUF_SIZE - CMD_MAX_LEN - 1];
    uint32_t pid;
    parent_pcb_t parent;
    uint32_t esp;
    uint32_t ebp;
    uint32_t ss0;
    uint32_t esp0;
} pcb_t;



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
int parse_command(const uint8_t* command, pcb_t* pcb, int pid);

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

#define STDIN_IDX 0
#define STDOUT_IDX 1

#endif /* SYSCALL_H */
