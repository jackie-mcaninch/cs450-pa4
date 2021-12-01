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


void printListItem(char *path, int inode) {
	char buf[DIRSIZ+1];
	char *p = path+strlen(path);
	while (p>=path && *p!='/') {
		p--;
	}
	p++;
	
	for (int i=0; i<DIRSIZ; i++) {
		if (i>=strlen(p))
			buf[i] = ' ';
		else
			buf[i] = p[i];
	}
	buf[DIRSIZ] = 0;
	cprintf("%s%d\n", buf, inode);
}

int directoryWalker(char *path) {
	struct inode *pathi = namei(path);
	struct inode *curri;
	struct dirent de;
			
	if(pathi == 0) {
		cprintf("Bad file path.\n");
		return -1;
	}
	
	if(pathi->type != T_DIR) {
		cprintf("Not a directory.\n");
		return -1;
	}
	
	cprintf("--------------------%s\n", path);
	for (int offset=0; offset<pathi->size; offset+=sizeof(de)) {
		if (readi(pathi, (char *)&de, offset, sizeof(de)) != sizeof(de)) {
			cprintf("Error reading file.\n");
			return -1;
		}
		
		if (de.inum == 0) {
			continue;
		}
		
		curri = dirlookup(pathi, de.name, 0);
		printListItem(de.name, curri->inum);
		if (curri->type == T_DIR && strncmp(de.name, ".", 5) && strncmp(de.name, "..",5)) {
			char newPath[DIRSIZ];
			newPath[0] = '/';
			int j;
			for (j=0; j<strlen(de.name); j++) {
				newPath[j+1] = de.name[j];
			}
			newPath[j+1] = 0;
			directoryWalker(newPath);
		}
	}

	return 0;
}

int inodeTBWalker() {
	struct superblock sb;
	readsb(ROOTDEV, &sb);
	struct dinode *di;
	struct buf *bp = 0;
	
	begin_op();
	int i=2; // skip over boot block and superblock
	while(i<sb.ninodes){
		bp = bread(ROOTDEV, IBLOCK(i, sb));
		di = (struct dinode *)bp->data + (i % IPB);
		if (di->type != 0) {
		//this inode is allocated for something
			cprintf("CURRENT INODE: %d\n", i);
			cprintf("dinode type: %d\n", di->type);
			cprintf("dinode size: %d\n", di->size);
			cprintf("\n");
		}		
		brelse(bp);
		i++;		
	}
	end_op();
	return 0;
}
