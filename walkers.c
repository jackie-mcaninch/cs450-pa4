#include "types.h"
#include "defs.h"
#include "fs.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "mmu.h"
#include "proc.h"
#include "stat.h"


char *formatListItem(char *path, int inode) {
	return 0;
	char *buf;
	char *p = path+strlen(path);
	while (p>=path && *p!='/') {
		p--;
	}
	p++;
	
	if(strlen(p)>=DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));
	memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	memset(buf+DIRSIZ, inode, 1);
	return buf;
}

int directoryWalker(char *path) {
	/*
	int fd;
	struct stat st;
	struct dirent de;
	char *buf;
	char *p;
	*/
	
	return inodeTBWalker();
		
/*
	if((fd = open(path, 0))<0 || fstat(fd, &st)<0) {
		cprintf("Bad file path.\n");
		return -1;
	}
	
	if(st.type != T_DIR) {
		cprintf("Not a directory.\n");
		return -1;
	}
	
	strcpy(buf, path);
	p = buf+strlen(buf);
	*p++ = '/';
	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if(de.inum == 0)
			continue;
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buf, &st) < 0){
			cprintf("Cannot stat %s.\n", buf);
			continue;
		}
		cprintf("%s\n", formatListItem(buf, st.ino));
	}
	fileclose(fd);
*/
	return 0;
}

int inodeTBWalker() {
	struct superblock sb;
	readsb(ROOTDEV, &sb);
	//struct inode *i = 0;
	//struct file *f = 0;
	struct dinode *di;
	//struct stat *s = 0;
	//stat(".", s);
	//i = s.ino;
	struct buf *bp = 0;
	
	cprintf("made it\n");
	for (int i=1; i<10; i++){
		cprintf("beginning...\n");
		bp = bread(ROOTDEV, IBLOCK(i, sb));
		cprintf("after read...\n");
		di = (struct dinode *)bp;
		cprintf("dinode size: %d\n", di->size);
		//fileread(f, bp, sizeof(struct dinode));
		//seek to pos bn in file
		//read in sizeof(dinode) from pos
		//if dinode		
	}
	return 0;
}
