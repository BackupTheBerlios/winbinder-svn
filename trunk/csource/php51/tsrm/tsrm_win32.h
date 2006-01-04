/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2004 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.0 of the PHP license,       |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_0.txt.                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Daniel Beulshausen <daniel@php4win.de>                      |
   +----------------------------------------------------------------------+
*/

/* $Id: tsrm_win32.h,v 1.18 2004/01/08 17:31:46 sniper Exp $ */

#ifndef TSRM_WIN32_H
#define TSRM_WIN32_H

#include "TSRM.h"
#include <windows.h>

struct ipc_perm {
	int			key;
	unsigned short	uid;
	unsigned short	gid;
	unsigned short	cuid;
	unsigned short	cgid;
	unsigned short	mode;
	unsigned short	seq;
};

struct shmid_ds {
	struct	ipc_perm	shm_perm;
	int				shm_segsz;
	time_t			shm_atime;
	time_t			shm_dtime;
	time_t			shm_ctime;
	unsigned short	shm_cpid;
	unsigned short	shm_lpid;
	short			shm_nattch;
};

typedef struct {
	FILE	*stream;
	HANDLE	prochnd;
} process_pair;

typedef struct {
	void	*addr;
	HANDLE	info;
	HANDLE	segment;
	struct	shmid_ds	*descriptor;
} shm_pair;

typedef struct {
	process_pair	*process;
	shm_pair		*shm;
	int				process_size;
	int				shm_size;
	char			*comspec;
} tsrm_win32_globals;

#ifdef ZTS
# define TWG(v) TSRMG(win32_globals_id, tsrm_win32_globals *, v)
#else
# define TWG(v) (win32_globals.v)
#endif

#define IPC_PRIVATE	0
#define IPC_CREAT	00001000
#define IPC_EXCL	00002000
#define IPC_NOWAIT	00004000

#define IPC_RMID	0
#define IPC_SET		1
#define IPC_STAT	2
#define IPC_INFO	3

#define SHM_R		PAGE_READONLY
#define SHM_W		PAGE_READWRITE

#define	SHM_RDONLY	FILE_MAP_READ
#define	SHM_RND		FILE_MAP_WRITE
#define	SHM_REMAP	FILE_MAP_COPY


TSRM_API void tsrm_win32_startup(void);
TSRM_API void tsrm_win32_shutdown(void);

TSRM_API FILE *popen_ex(const char *command, const char *type, const char *cwd, char *env);
TSRM_API FILE *popen(const char *command, const char *type);
TSRM_API int pclose(FILE *stream);
TSRM_API int tsrm_win32_access(const char *pathname, int mode);

TSRM_API int shmget(int key, int size, int flags);
TSRM_API void *shmat(int key, const void *shmaddr, int flags);
TSRM_API int shmdt(const void *shmaddr);
TSRM_API int shmctl(int key, int cmd, struct shmid_ds *buf);

TSRM_API char *realpath(char *orig_path, char *buffer);
#endif
