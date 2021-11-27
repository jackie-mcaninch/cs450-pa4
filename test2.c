#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
	if (atoi(argv[1])==1) {
	  // tests vanilla countTraps... increments once each time
	  countTraps();
	  countTraps();
	  countTraps(); // expected result: 2 more than original
	}
	else if (atoi(argv[1])==2) {
	  // tests the write() system call... executed 30x
	  countTraps();
	  printf(0, "printing 30 new characters...\n");
	  countTraps(); // expected result: 31 more than original
	}
	else if (atoi(argv[1])==3) {
	  // tests the open() and close() system call
	  countTraps();
	  int fd = open("temp.txt", 'r');
	  close(fd);
	  countTraps(); // expected result: 3 more than original
	}
	else if (atoi(argv[1])==4) {
	  // tests the fork(), exit(), and wait() system call
	  countTraps();
	  if (fork()==0) {
	    if (fork()==0) {
	      exit();
	    }
	    wait();
	    exit();
	  }
	  wait();
	  countTraps(); // expected result: 7 more than original
	}
	else if (atoi(argv[1])==5) {
	  // tests mkdir() and chdir()
	  countTraps();
	  mkdir("temp");
	  chdir("temp");
	  chdir("..");
	  countTraps(); // expected result: 4 more than original
	}
	else {
	  countTraps(); // simplest call
	}
	exit();
}
