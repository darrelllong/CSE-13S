#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int a, b;
  int p[2];

  pipe(p);

  if ((a = fork()) == 0) {
    close(1);
    dup(p[1]);
    close(p[1]);
    close(p[0]);
    execlp("ls", "ls", "-l", (char *)0);
  }

  if ((b = fork()) == 0) {
    close(0);
    dup(p[0]);
    close(p[1]);
    close(p[0]);
    execlp("wc", "wc", (char *)0);
  }

  close(p[0]);
  close(p[1]);

  waitpid(b, (void *)0, 0);
}
