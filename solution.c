#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
    int in1, in2;
    int sum = 0;
    fd_set readfds;
    char buf[1024];

    in1 = open("in1", O_RDONLY);
    in2 = open("in2", O_RDONLY);

    if (in1 == -1 || in2 == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
      FD_ZERO(&readfds);
      FD_SET(in1, &readfds);
      FD_SET(in2, &readfds);

      int max_fd = MAX(in1, in2) + 1;

      int ready = select(max_fd, &readfds, NULL, NULL, NULL);

      if (ready == -1) {
          perror("select");
          exit(EXIT_FAILURE);
      }

      if (FD_ISSET(in1, &readfds)) {
          ssize_t bytes_read = read(in1, buf, sizeof(buf));
          if (bytes_read <= 0) {
              close(in1);
              FD_CLR(in1, &readfds);
          } else {
              sum += atoi(buf);
          }
      }

      if (FD_ISSET(in2, &readfds)) {
          ssize_t bytes_read = read(in2, buf, sizeof(buf));
          if (bytes_read <= 0) {
              close(in2);
              FD_CLR(in2, &readfds);
          } else {
              sum += atoi(buf);
          }
      }

      if (!FD_ISSET(in1, &readfds) && !FD_ISSET(in2, &readfds)) {
          printf("%d\n", sum);
          break;
      }
    }

    return 0;
}

