#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>

char *whoami() {
    return getpwuid(getuid())->pw_name;
}

int main(void) {
    puts(whoami());
    return 0;
}
