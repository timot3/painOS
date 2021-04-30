#ifndef SYSCALL_H
#define SYSCALL_H
#include "lib.h"
#include "terminal.h"
#include "filesys.h"
#include "rtc.h"
#include "paging.h"
#include "x86_desc.h"

#define CMD_MAX_LEN 32
#define PROCESS_LIMIT 7
#define KERNEL_PAGE_BOT 0x800000
#define KERNEL_STACK_SIZE 8192
#define DELETE 0x7F
#define E 0x45
#define L 0x4c
#define F 0x46
#define BUFFER_START 0x8048000
#define MAX_FILE_SIZE 0x400000
//0x8000000 - 4
#define USER_PAGE_TOP 0x7FFFFFC
#define USER_PAGE_BOT 0x83FFFFC
#define VIDMAP_LOC 0x19

#define ACTIVE_FLAG_MASK 4
#define ACTIVE_FLAG 1
#define INACTIVE_FLAG 0

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
    uint32_t flags; // bit 0, 1 == read, write, read/write perms
} fd_items_t;

typedef struct parent_pcb {
    uint32_t ksp;
    uint32_t kbp;
    uint32_t pid;
} parent_pcb_t;

typedef struct register_str {
    uint32_t fl;
    uint32_t esi;
    uint32_t ebx;
    uint32_t edx;
    uint32_t edi;
    uint32_t ecx;
    uint32_t eax;
    uint32_t eip;
    uint32_t esp;
    uint32_t ebp;
} register_str_t;

typedef struct pcb {
    fd_items_t fd_items[MAX_OPEN_FILES];
    uint8_t argument_buf[TERM_BUF_SIZE - CMD_MAX_LEN - 1];
    parent_pcb_t parent;
    register_str_t registers;
    uint32_t pid;
    uint8_t** vidmap_addr;

    // for scheduling
    uint32_t esp;
    uint32_t ebp;
    uint32_t esp0;
} pcb_t;



int32_t halt (uint8_t status);
int32_t execute (const uint8_t* command);
int32_t read (int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t open (const uint8_t* filename);
int32_t set_active_flag (int32_t fd, int32_t new_status);
int32_t close (int32_t fd);
int32_t getargs (uint8_t* buf, int32_t nbytes);
int32_t vidmap (uint8_t** screen_start);
int32_t set_handler (int32_t signum, void* handler_address);
int32_t sigreturn (void);

int assign_pid(void);
int unassign_pid(int pid, int parentPid);
int get_latest_pid();
pcb_t* get_pcb_addr(int pid);
pcb_t* get_latest_pcb();
pcb_t* allocate_pcb(int pid);
int parse_command(const uint8_t* command, pcb_t* pcb, int pid, dentry_t *dentry);
void setup_TSS(int pid);

extern int8_t curr_pids[MAX_TERMINALS];
extern int8_t pid_arr[PROCESS_LIMIT];


#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02


#define READ_WRITE_MASK 3 // 11 in binary

#define STDIN_IDX 0
#define STDOUT_IDX 1

#endif /* SYSCALL_H */
