#include "dev.h"
#include "syscall.h"

//#include "pk.h"
//#include "file.h"
//#include "frontend.h"
//#include "mmap.h"

#include <string.h>
#include <errno.h>

typedef long (*syscall_t)(long, long, long, long, long, long, long);

#define CLOCK_FREQ MCK_FREQ

void sys_exit(int code)
{
#if 0
	if (current.cycle0) {
		size_t dt = rdtime() - current.time0;
		size_t dc = rdcycle() - current.cycle0;
		size_t di = rdinstret() - current.instret0;

		printk("%ld ticks\n", dt);
		printk("%ld cycles\n", dc);
		printk("%ld instructions\n", di);
		printk("%d.%d%d CPI\n", dc/di, 10ULL*dc/di % 10, (100ULL*dc + di/2)/di % 10);
	}
	shutdown(code);
#else
	_printf("SYS_EXIT(%d)\n", code);
#endif
}

ssize_t sys_read(int fd, char* buf, size_t n)
{
	flprintf("\n");
	ssize_t r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_read(f, buf, n);
		file_decref(f);
	}
#endif
	return r;
}

ssize_t sys_pread(int fd, char* buf, size_t n, off_t offset)
{
	flprintf("\n");
	ssize_t r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_pread(f, buf, n, offset);
		file_decref(f);
	}
#endif
	return r;
}

ssize_t sys_write(int fd, const char* buf, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		if (buf[i] == '\n') UartTx(DEBUG_UART_NUM, '\r');
		UartTx(DEBUG_UART_NUM, buf[i]);
	}
	//hexDump(__func__, buf, n);
	return n;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_write(f, buf, n);
		file_decref(f);
	}
#endif
#if 0
	ssize_t r = -EBADF;
	return r;
#endif
}

static int at_kfd(int dirfd)
{
	flprintf("\n");
#if 0
	if (dirfd == AT_FDCWD) {
		return AT_FDCWD;
	}
	file_t* dir = file_get(dirfd);
	if (dir == NULL) {
		return -1;
	}
	return dir->kfd;
#else
	return -1;
#endif
}

int sys_openat(int dirfd, const char* name, int flags, int mode)
{
	flprintf("\n");
#if 0
	int kfd = at_kfd(dirfd);
	if (kfd != -1) {
		file_t* file = file_openat(kfd, name, flags, mode);
		if (IS_ERR_VALUE(file)) {
			return PTR_ERR(file);
		}

		int fd = file_dup(file);
		if (fd < 0) {
			file_decref(file);
			return -ENOMEM;
		}

		return fd;
	}
#endif
	return -EBADF;
}

int sys_open(const char* name, int flags, int mode)
{
	flprintf("\n");
	return sys_openat(AT_FDCWD, name, flags, mode);
}

int sys_close(int fd)
{
	flprintf("\n");
#if 0
	int ret = fd_close(fd);
	if (ret < 0) {
		return -EBADF;
	}
	return ret;
#else
	return -EBADF;
#endif
}

int sys_renameat(int old_fd, const char *old_path, int new_fd, const char *new_path)
{
	flprintf("\n");
#if 0
	int old_kfd = at_kfd(old_fd);
	int new_kfd = at_kfd(new_fd);
	if(old_kfd != -1 && new_kfd != -1) {
		size_t old_size = strlen(old_path)+1;
		size_t new_size = strlen(new_path)+1;
		return frontend_syscall(SYS_renameat, old_kfd, va2pa(old_path), old_size,
											  new_kfd, va2pa(new_path), new_size, 0);
	}
#endif
  return -EBADF;
}

int sys_fstat(int fd, void* st)
{
	flprintf("\n");
	int r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_stat(f, st);
		file_decref(f);
	}
#endif
	return r;
}

int sys_fcntl(int fd, int cmd, int arg)
{
	flprintf("\n");
	int r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = frontend_syscall(SYS_fcntl, f->kfd, cmd, arg, 0, 0, 0, 0);
		file_decref(f);
	}
#endif
	return r;
}

int sys_ftruncate(int fd, off_t len)
{
	flprintf("\n");
	int r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_truncate(f, len);
		file_decref(f);
	}
#endif
	return r;
}

int sys_dup(int fd)
{
	flprintf("\n");
	int r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_dup(f);
		file_decref(f);
	}
#endif
	return r;
}

ssize_t sys_lseek(int fd, size_t ptr, int dir)
{
	flprintf("\n");
	ssize_t r = -EBADF;
#if 0
	file_t* f = file_get(fd);

	if (f) {
		r = file_lseek(f, ptr, dir);
		file_decref(f);
	}
#endif
	return r;
}

long sys_lstat(const char* name, void* st)
{
	flprintf("\n");
#if 0
	struct frontend_stat buf;
	size_t name_size = strlen(name)+1;
	long ret = frontend_syscall(SYS_lstat, va2pa(name), name_size, va2pa(&buf), 0, 0, 0, 0);
	copy_stat(st, &buf);
	return ret;
#else
	return -EBADF;
#endif
}

long sys_fstatat(int dirfd, const char* name, void* st, int flags)
{
	flprintf("\n");
#if 0
	int kfd = at_kfd(dirfd);
	if (kfd != -1) {
		struct frontend_stat buf;
		size_t name_size = strlen(name)+1;
		long ret = frontend_syscall(SYS_fstatat, kfd, va2pa(name), name_size, va2pa(&buf), flags, 0, 0);
		copy_stat(st, &buf);
		return ret;
	}
#endif
	return -EBADF;
}

long sys_stat(const char* name, void* st)
{
	flprintf("\n");
	return sys_fstatat(AT_FDCWD, name, st, 0);
}

long sys_faccessat(int dirfd, const char *name, int mode)
{
	flprintf("\n");
#if 0
	int kfd = at_kfd(dirfd);
	if (kfd != -1) {
		size_t name_size = strlen(name)+1;
		return frontend_syscall(SYS_faccessat, kfd, va2pa(name), name_size, mode, 0, 0, 0);
	}
#endif
	return -EBADF;
}

long sys_access(const char *name, int mode)
{
	flprintf("\n");
	return sys_faccessat(AT_FDCWD, name, mode);
}

long sys_linkat(int old_dirfd, const char* old_name, int new_dirfd, const char* new_name, int flags)
{
	flprintf("\n");
#if 0
	int old_kfd = at_kfd(old_dirfd);
	int new_kfd = at_kfd(new_dirfd);
	if (old_kfd != -1 && new_kfd != -1) {
		size_t old_size = strlen(old_name)+1;
		size_t new_size = strlen(new_name)+1;
		return frontend_syscall(SYS_linkat, old_kfd, va2pa(old_name), old_size,
											new_kfd, va2pa(new_name), new_size,
											flags);
	}
#endif
	return -EBADF;
}

long sys_link(const char* old_name, const char* new_name)
{
	flprintf("\n");
	return sys_linkat(AT_FDCWD, old_name, AT_FDCWD, new_name, 0);
}

long sys_unlinkat(int dirfd, const char* name, int flags)
{
	flprintf("\n");
#if 0
	int kfd = at_kfd(dirfd);
	if (kfd != -1) {
		size_t name_size = strlen(name)+1;
		return frontend_syscall(SYS_unlinkat, kfd, va2pa(name), name_size, flags, 0, 0, 0);
	}
#endif
  return -EBADF;
}

long sys_unlink(const char* name)
{
	flprintf("\n");
	return sys_unlinkat(AT_FDCWD, name, 0);
}

long sys_mkdirat(int dirfd, const char* name, int mode)
{
	flprintf("\n");
#if 0
	int kfd = at_kfd(dirfd);
	if (kfd != -1) {
		size_t name_size = strlen(name)+1;
		return frontend_syscall(SYS_mkdirat, kfd, va2pa(name), name_size, mode, 0, 0, 0);
	}
#endif
	return -EBADF;
}

long sys_mkdir(const char* name, int mode)
{
	flprintf("\n");
	return sys_mkdirat(AT_FDCWD, name, mode);
}

long sys_getcwd(const char* buf, size_t size)
{
	flprintf("\n");
#if 0
	populate_mapping(buf, size, PROT_WRITE);
	return frontend_syscall(SYS_getcwd, va2pa(buf), size, 0, 0, 0, 0, 0);
#else
	return -EBADF;
#endif
}

size_t sys_brk(size_t pos)
{
#if 1
	//printf("%s\n", __func__);
	//BYTE *addr = pvPortMalloc(pos+16);
	//printf("%s-(Addr:0x%08X, Size:%u/0x%X)\n", __func__, addr, pos, pos);
	//return addr;
	//return -EBADF;
	//return do_brk(pos);




	printf("%s-0x%x\n", __func__, pos);




	if (pos == 0) {
		return 0x81000000;
	} else {
		if (pos > (0x80000000 + 64*1024*1024 - 8*1024*1024))
			return -1;
		return pos;
	}
#else
	return -EBADF;
#endif
}

int sys_uname(void* buf)
{
	flprintf("\n");
	const int sz = 65;
	strcpy(buf + 0*sz, "Eyenix EN675");
	strcpy(buf + 1*sz, "");
	strcpy(buf + 2*sz, "0.0.1");
	strcpy(buf + 3*sz, "");
	strcpy(buf + 4*sz, "");
	strcpy(buf + 5*sz, "");
	return 0;
}

pid_t sys_getpid(void)
{
	flprintf("\n");
	return 0;
}

int sys_getuid(void)
{
	flprintf("\n");
	return 0;
}

uintptr_t sys_mmap(uintptr_t addr, size_t length, int prot, int flags, int fd, off_t offset)
{
#if 0
#if __riscv_xlen == 32
	if (offset != (offset << 12 >> 12)) {
		return -ENXIO;
	}
	offset <<= 12;
#endif
	return do_mmap(addr, length, prot, flags, fd, offset);
#else
	flprintf("\n");
	return -EBADF;
#endif
}

int sys_munmap(uintptr_t addr, size_t length)
{
#if 0
	return do_munmap(addr, length);
#else
	flprintf("\n");
	return -EBADF;
#endif
}

uintptr_t sys_mremap(uintptr_t addr, size_t old_size, size_t new_size, int flags)
{
#if 0
	return do_mremap(addr, old_size, new_size, flags);
#else
	flprintf("\n");
	return -EBADF;
#endif
}

uintptr_t sys_mprotect(uintptr_t addr, size_t length, int prot)
{
#if 0
	return do_mprotect(addr, length, prot);
#else
	flprintf("\n");
	return -EBADF;
#endif
}

int sys_rt_sigaction(int sig, const void* act, void* oact, size_t sssz)
{
	flprintf("\n");
	if (oact) {
		memset(oact, 0, sizeof(long) * 3);
	}

	return 0;
}

long sys_time(long* loc)
{
	flprintf("\n");
	uintptr_t t = rdcycle() / CLOCK_FREQ;
	if (loc) {
		*loc = t;
	}
	return t;
}

int sys_times(long* loc)
{
	flprintf("\n");
	uintptr_t t = rdcycle();
	loc[0] = t / (CLOCK_FREQ / 1000000);
	loc[1] = 0;
	loc[2] = 0;
	loc[3] = 0;
	return 0;
}

int sys_gettimeofday(long* loc)
{
	flprintf("\n");
	uintptr_t t = rdcycle();
	loc[0] = t / CLOCK_FREQ;
	loc[1] = (t % CLOCK_FREQ) / (CLOCK_FREQ / 1000000);
	return 0;
}

ssize_t sys_writev(int fd, const long* iov, int cnt)
{
	flprintf("\n");
	ssize_t ret = 0;
	for (int i = 0; i < cnt; i++) {
		ssize_t r = sys_write(fd, (void*)iov[2*i], iov[2*i+1]);
		if (r < 0) {
			return r;
		}
		ret += r;
	}
	return ret;
}

int sys_chdir(const char *path)
{
	flprintf("\n");
#if 0
	return frontend_syscall(SYS_chdir, va2pa(path), 0, 0, 0, 0, 0, 0);
#else
	return -EBADF;
#endif
}

int sys_getdents(int fd, void* dirbuf, int count)
{
	flprintf("\n");
	return 0; //stub
}

static int sys_stub_success(void)
{
	flprintf("\n");
	return 0;
}

static int sys_stub_nosys(void)
{
	flprintf("\n");
	return -ENOSYS;
}

static void __attribute__((noreturn)) bad_syscall(unsigned long n)
{
	_printf("machine mode: unhandlable syscall %d @ %p\n", read_csr(mcause), n);
	while (1);
}

long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, unsigned long cmd)
{
	const static void* syscall_table[] = {
		[SYS_exit] = sys_exit,
		[SYS_exit_group] = sys_exit,
		[SYS_read] = sys_read,
		[SYS_pread] = sys_pread,
		[SYS_write] = sys_write,
		[SYS_openat] = sys_openat,
		[SYS_close] = sys_close,
		[SYS_fstat] = sys_fstat,
		[SYS_lseek] = sys_lseek,
		[SYS_fstatat] = sys_fstatat,
		[SYS_linkat] = sys_linkat,
		[SYS_unlinkat] = sys_unlinkat,
		[SYS_mkdirat] = sys_mkdirat,
		[SYS_renameat] = sys_renameat,
		[SYS_getcwd] = sys_getcwd,
		[SYS_brk] = sys_brk,
		[SYS_uname] = sys_uname,
		[SYS_getpid] = sys_getpid,
		[SYS_getuid] = sys_getuid,
		[SYS_geteuid] = sys_getuid,
		[SYS_getgid] = sys_getuid,
		[SYS_getegid] = sys_getuid,
		[SYS_mmap] = sys_mmap,
		[SYS_munmap] = sys_munmap,
		[SYS_mremap] = sys_mremap,
		[SYS_mprotect] = sys_mprotect,
		[SYS_prlimit64] = sys_stub_nosys,
		[SYS_rt_sigaction] = sys_rt_sigaction,
		[SYS_gettimeofday] = sys_gettimeofday,
		[SYS_times] = sys_times,
		[SYS_writev] = sys_writev,
		[SYS_faccessat] = sys_faccessat,
		[SYS_fcntl] = sys_fcntl,
		[SYS_ftruncate] = sys_ftruncate,
		[SYS_getdents] = sys_getdents,
		[SYS_dup] = sys_dup,
		[SYS_readlinkat] = sys_stub_nosys,
		[SYS_rt_sigprocmask] = sys_stub_success,
		[SYS_ioctl] = sys_stub_nosys,
		[SYS_clock_gettime] = sys_stub_nosys,
		[SYS_getrusage] = sys_stub_nosys,
		[SYS_getrlimit] = sys_stub_nosys,
		[SYS_setrlimit] = sys_stub_nosys,
		[SYS_chdir] = sys_chdir,
		[SYS_set_tid_address] = sys_stub_nosys,
		[SYS_set_robust_list] = sys_stub_nosys,
	};

	const static void* old_syscall_table[] = {
		[-OLD_SYSCALL_THRESHOLD + SYS_open] = sys_open,
		[-OLD_SYSCALL_THRESHOLD + SYS_link] = sys_link,
		[-OLD_SYSCALL_THRESHOLD + SYS_unlink] = sys_unlink,
		[-OLD_SYSCALL_THRESHOLD + SYS_mkdir] = sys_mkdir,
		[-OLD_SYSCALL_THRESHOLD + SYS_access] = sys_access,
		[-OLD_SYSCALL_THRESHOLD + SYS_stat] = sys_stat,
		[-OLD_SYSCALL_THRESHOLD + SYS_lstat] = sys_lstat,
		[-OLD_SYSCALL_THRESHOLD + SYS_time] = sys_time,
	};

	syscall_t f = 0;

	if (cmd < ARRAY_SIZE(syscall_table)) {
		f = syscall_table[cmd];
	} else if (cmd - OLD_SYSCALL_THRESHOLD < ARRAY_SIZE(old_syscall_table)) {
		f = old_syscall_table[cmd - OLD_SYSCALL_THRESHOLD];
	}

	if (!f) {
		bad_syscall(cmd);
	}

	return f(a0, a1, a2, a3, a4, a5, cmd);
}
