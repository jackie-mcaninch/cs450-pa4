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

#define NINODES 50

int dirs[NINODES];
int inodes[NINODES];
int comp[NINODES];

void printListItem(char *path, int inode) {
	char buf[DIRSIZ+1];
	char *p = path+strlen(path);
	
	//move pointer to after last slash
	while (p>=path && *p!='/')
		p--;
	p++;
	
	//copy chars after slash from path to buf
	for (int i=0; i<DIRSIZ; i++) {
		if (i>=strlen(p))
			buf[i] = ' ';
		else
			buf[i] = p[i];
	}
	buf[DIRSIZ] = 0;
	
	//print file name followed by inode number
	cprintf("%s%d\n", buf, inode);
}


int directoryWalker(char *path) {
	struct inode *pathi = namei(path);
	struct inode *curri;
	struct dirent de;
		
	//check if user-specified path is valid
	if(pathi == 0) {
		cprintf("Bad file path.\n");
		return -1;
	}
	
	//check that user-specified path is a directory
	if(pathi->type != T_DIR) {
		cprintf("Not a directory.\n");
		return -1;
	}
	
	//iterate through list of dirents in directory
	cprintf("--------------------%s\n", path);
	for (int offset=0; offset<pathi->size; offset+=sizeof(de)) {
		
		//load the dirent into de
		if (readi(pathi, (char *)&de, offset, sizeof(de)) != sizeof(de)) {
			cprintf("Error reading file.\n");
			return -1;
		}
		
		//skip dirent if its inode is not allocated
		if (de.inum == 0) {
			continue;
		}
		
		//get inode and print out nicely
		curri = dirlookup(pathi, de.name, 0);
		printListItem(de.name, curri->inum);
		
		//save the allocated inode for later comparing
		dirs[curri->inum] = 1;
		
		//recursively handle files in directories
		if (curri->type==T_DIR && strncmp(de.name,".", 1) && strncmp(de.name,"..",2)) {
			//generate new path name
			char newPath[DIRSIZ];
			newPath[0] = '/';
			int j;
			for (j=0; j<strlen(de.name) && j<DIRSIZ; j++)
				newPath[j+1] = de.name[j];
			newPath[j+1] = 0;
			
			//get inodes for all files inside new directory
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
	
	//initialize allocated inodes array
	for (int i=0; i<sb.ninodes; i++)
		inodes[i] = 0;
	
	//skipping boot block and superblock, iterate through all available inodes
	for(int i=2; i<sb.ninodes; i++){
	
		//load the dinode from disk
		bp = bread(ROOTDEV, IBLOCK(i, sb));
		di = (struct dinode *)bp->data + (i % IPB);
		
		//found a non empty inode
		if (di->type != 0) {
			cprintf("INODE %d  ->  type: %d size: %d\n", i, di->type, di->size);
			inodes[i] = 1;
		}		
		brelse(bp);	
	}
	return 0;
}

int compareWalker(){
	int i;
	int dirArr = -1;
	int inodeArr = -1;
	
	//check if both arrays contain info
	for(i=0; i<NINODES; i++){
		if(dirs[i] == 1)
			dirArr = 1;
		if(inodes[i] == 1)
			inodeArr = 1;
	}
	
	//establish which arrays are missing
	if(dirArr == -1) {
		cprintf("Please call walkDir() first.\n");
		return -1;
	}
	if(inodeArr == -1){
		cprintf("Please run walkInodes() first\n");
		return -1;
	}

	//compare the arrays
	for(i=2; i<NINODES; i++){
		if((inodes[i] == 1) && (dirs[i] == 1)){
			cprintf("Inode %d found in both walkers.\n",i);
		}
		if((inodes[i] == 0) && (dirs[i] == 1)){
			cprintf("Inode %d found in Directory Walker but not in Inode Walker\n",i);
		}
		if((inodes[i] == 1) && (dirs[i] == 0)){
			cprintf("Inode %d found in Inode Walker but not in Directory Walker\n",i);
		}
		
		//flag the comp array if they do not agree
		comp[i] = inodes[i]^dirs[i];
	}
	return 0;
}

int eraseInf(int inode) {
	cprintf("\n\nDamaging inode directory: %d...\n\n", inode);
	return erase(inode, dirs);
}

int fixDmgFS() {
	cprintf("\n\nAttempting to fix inode directory...\n\n");
	return fix(comp);
}
