#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
	int num_tests = 8;
	int fd[num_tests];
	char temp_name[10];
	memmove(temp_name, "testX.txt\0", 10);
	
	mkdir("testfolder");
	chdir("testfolder");
	for (int i=0; i<num_tests; i++){
		memset(temp_name+4, i+48, 1);
		//printf(1, "%s\n", temp_name);
		if((fd[i] = open(temp_name, O_CREATE|O_RDWR))<0){
			printf(2, "ERROR OPENING FILE!");
		}
		if (write(fd[i], "asdf", 4) != 4) {
			printf(2, "ERROR WRITING!\n");
		}
		
	}
	walkDir(argv[1]);
	walkInodes();
	for (int i=0; i<num_tests; i++) {
		close(fd[i]);
	}
	
	compareWalkers();
	eraseInfo(20);
	walkDir(argv[1]);
	walkInodes();
	fixDamagedFS();
	exit();
}
