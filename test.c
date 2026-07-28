#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

/*int get_time(struct timeval start) {
  struct timeval now;
  usleep(500);
  gettimeofday(&now, NULL);
  int val, val1, val2;
  val1 = (start.tv_sec - now.tv_sec);
  printf("%i\n", val1);
  val2 = (start.tv_usec - now.tv_usec);
  printf("%i\n", val2);
  val = val1 - val2;
  return (val);
}
*/
long get_time(struct timeval start) {
  struct timeval now;
  usleep(500 * 1000);
  gettimeofday(&now, NULL);

  return (now.tv_sec - start.tv_sec) * 1000 +
         (now.tv_usec - start.tv_usec) / 1000;
}
int main() {
  struct timeval start;

  gettimeofday(&start, NULL);
  int val;
  val = get_time(start);
  printf("%i", val);
  return 0;
}
