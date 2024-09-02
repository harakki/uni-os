#include <stdio.h>
#include <sys/sysinfo.h>

int main(void) {
  struct sysinfo info;
  int func_return = 0;

  // Get info about RAM and SWAP
  func_return = sysinfo(&info);

  // Exit program if any error returned
  if (func_return != 0) {
    perror("sysinfo:EFAULT");
    return 1;
  }

  // Output collected data
  printf("%16s %16s %16s %16s %16s %16s\n", "RAM-SIZE", "FREE-RAM",
         "SHARED-RAM", "BUFFERED-RAM", "SWAP-SIZE", "FREE-SWAP");

  printf("%16lu %16lu %16lu %16lu %16lu %16lu\n", info.totalram, info.freeram,
         info.sharedram, info.bufferram, info.totalswap, info.freeswap);

  return 0;
}
