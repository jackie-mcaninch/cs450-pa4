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
#include "buf.h"


char *formatListItem(char *path, int inode) {
	return 0;
	
	//ignore this was taken from ls.c and doesn't work
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
	
	begin_op();
	//cprintf("size of dinode: %d\n", sizeof(struct dinode));
	//cprintf("size of buf: %d\n", sizeof(struct buf));
	int i=2; // skip over boot block and superblock
	while(i<sb.ninodes){
		//cprintf("iblock: %d...\n", IBLOCK(i,sb));
		bp = bread(ROOTDEV, IBLOCK(i, sb));
		di = (struct dinode *)bp->data + (i % IPB);
		if (di->type != 0) {
		//this inode is allocated for something
			cprintf("dinode type: %d\n", di->type);
			cprintf("dinode size: %d\n", di->size);
			cprintf("dinode first address: %d\n\n", di->addrs[0]);
		}		
		brelse(bp);
		i++;		
	}
	end_op();
	return 0;
}
