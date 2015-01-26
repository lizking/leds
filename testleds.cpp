
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "LedControllerRequest.h"
#include "GeneratedTypes.h"

int main(int argc, const char **argv)
{
  LedControllerRequestProxy *device = new LedControllerRequestProxy(IfcNames_LedControllerRequestPortal);

  printf("Starting LED test");
  FILE *pipe = popen("ifconfig eth0", "r");
  char buf[256];
  // read the first line and discard it
  fgets(buf, sizeof(buf), pipe);
  // read the second line
  fgets(buf, sizeof(buf), pipe);
  printf("address line: %s", buf);
  // done with the pipe, close it
  fclose(pipe);
  
  int addr[5];
  memset(addr, 0, sizeof(addr));
  int status = sscanf(buf, "          inet addr:%d.%d.%d.%d", &addr[0], &addr[1], &addr[2], &addr[3]);
  printf("eth0 addr %d.%d.%d.%d\n", addr[0], addr[1], addr[2], addr[3]);
  
  for (int i = 0; i < 20; i++) {
    printf("led value %x\n", addr[i % 5]);
    device->setLeds(addr[i % 5], blinkinterval);
    sleep(sleepinterval);
  }
  
#ifdef BSIM
  // BSIM does not run very many cycles per second
  int blinkinterval = 10;
#else
  int blinkinterval = 100000000; // 100MHz cycles
#endif
  int blinkon = 10; // 1010
  int blinkoff = 5; // 0101
  int sleepinterval = 1; // seconds
  for (int i = 0; i < 20; i++) {
    printf("blink %d", blinkon);
    device->setLeds(blinkon, blinkinterval);
    sleep(sleepinterval);
    printf("blink off %d", blinkoff);
    device->setLeds(blinkoff, blinkinterval);
    sleep(sleepinterval);
  }
  printf("Done.\n");
}
