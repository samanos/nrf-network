## Building

Make sure `tinytemplate` symlink is pointing to a directory which contains checked out [tinytemplate](https://github.com/thegaragelab/tinytemplate) repository. By default symlink points to a sibling directory of this repository. Commit id of the repository is specified in the contents of symplink.

Makefile accepts the following arguments:

|Argument|Possible Values|Notes                                                              |
|--------|---------------|-------------------------------------------------------------------|
|name    |ptx&#124;prx            |Binary (primary transmitter or primary receiver) to build |
|avrType |attiny85&#124;attiny2313|Controller type. Tested only with attiny85                |
|dudeHost|alarmpi&#124;...        |When specified runs `avrdude` on the specified host       |

Most common make commands are `elf` (compile) and `flash` (compile and write to MCU). Flashing is done using a Raspberry Pi which has MCU connected via SPI.

For example, the following command would compile primary transmitter and flash it to ATtiny85 connected to Raspberry PI running as alarmpi2:

``` bash
make name=ptx avrType=attiny85 dudeHost=alarmpi2 flash
```

## Hardware

Code assumes [three wire](http://nerdralph.blogspot.com/2014/01/nrf24l01-control-with-3-attiny85-pins.html) setup where CSN clock is held by a capacitor and is controlled by SCK pin before and after transfers.

Logging is enabled via UART on `PB4`.
