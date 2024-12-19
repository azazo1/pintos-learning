#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

static void (*syscalls[SYS_CALL_MAX_HOLDER])(struct intr_frame*);

void sys_exit(struct intr_frame*f);
void sys_write(struct intr_frame*f);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  // 存放系统调用函数指针.
  syscalls[SYS_EXIT] = &sys_exit;
  syscalls[SYS_WRITE] = &sys_write;
}

static void
syscall_handler (struct intr_frame *f)
{
  int type = *(int*) f->esp; // 取出栈指针指向的第一个 int.
  if (type <= 0 || type >= SYS_CALL_MAX_HOLDER) {
    printf("syscall_handler: invalid syscall number %d\n", type);
    thread_exit();
  }
  if (type == SYS_EXIT || type == SYS_WRITE) {
    syscalls[type](f);
  } else {
    printf("system call!\n");
    thread_exit();
  }
}

void sys_exit(struct intr_frame *f) {
  uint32_t *user_ptr = f->esp;
  user_ptr++; // type(见syscall_handler) 之后的四字节表示退出状态.
  thread_current()->st_exit = *user_ptr;
  thread_exit();
}

void sys_write(struct intr_frame *f) {
  uint32_t *user_ptr = f->esp;
  user_ptr++;
//  printf("user_ptr:%p, fesp:%p\n", user_ptr, f->esp);
  int fd= *user_ptr; // 文件描述符在 type 之后.
  const char*buffer =(const char*) *(user_ptr+1); // 输出内容在文件描述符之后.
  unsigned size = *(user_ptr + 2); // 缓冲区大小在输出内容之后.
  if (fd == 1) {
    // 输出到 console.
    putbuf(buffer, size);
    f->eax = size;
  }
}