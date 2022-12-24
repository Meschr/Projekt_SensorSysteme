[Markdown Cheatsheet](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#tables)

# Pin Usage 

|         |     |      |             |                                                                                                      |          |                       |                       |           |
| ------- | --- | ---- | ----------- | ---------------------------------------------------------------------------------------------------- | -------- | --------------------- | --------------------- | --------- |
| PIN NUM | Typ | GPIO |  Name       | Funktion                                                                                             | Belegung | Verwendung im Projekt | Baugruppe             | Bemerkung |
| 1       | P   | \-   | GND         | Ground                                                                                               |          |                       |                       |           |
| 2       | P   | \-   | 3V3         | Power supply                                                                                         |          |                       |                       |           |
| 3       | I   | \-   | EN          | High: On; enables the chipLow: Off; the chip powers offNote: Do not leave the CHIP\_PU pin floating. |          | Reset                 | Button(Reset)         |           |
| 4       | I   | 36   | SENSOR\_VP  | ADC1\_CH0,  RTC\_GPIO0                                                                               |          |                       |                       |           |
| 5       | I   | 39   | SENSOR\_VN  | ADC1\_CH3,  RTC\_GPIO3                                                                               |          |                       |                       |           |
| 6       | I   | 34   | VDET\_1     | ADC1\_CH6,  RTC\_GPIO4                                                                               |          |                       |                       |           |
| 7       | I   | 35   | VDET\_2     | ADC1\_CH7,  RTC\_GPIO5                                                                               |          |                       |                       |           |
| 8       | I/O | 32   | 32K\_XP     | ADC1\_CH4,  RTC\_GPIO9,  TOUCH9,   32K\_XP (32.768 kHz crystal oscillator input)                     |          | GPIO32                | ADS8028(Chip Select)  |           |
| 9       | I/O | 33   | 32K\_XN     | ADC1\_CH5,  RTC\_GPIO8,  TOUCH8,   32K\_XN (32.768 kHz crystal oscillator output)                    |          | GPIO33                | LED (Status Logging)  |           |
| 10      | I/O | 25   | GPIO25      | ADC2\_CH8,  RTC\_GPIO6,  DAC\_1,  EMAC\_RXD0                                                         |          |                       |                       |           |
| 11      | I/O | 26   | GPIO26      | ADC2\_CH9,  RTC\_GPIO7,  DAC\_2,  EMAC\_RXD1                                                         |          |                       |                       |           |
| 12      | I/O | 27   | GPIO27      | ADC2\_CH7,  RTC\_GPIO17, TOUCH7,  EMAC\_RX\_DV                                                       |          | GPIO17                | Piezo Buzzer          |           |
| 13      | I/O | 14   | MTMS        | ADC2\_CH6,  RTC\_GPIO16, TOUCH6,   EMAC\_TXD2,  HSPICLK,  HS2\_CLK,      SD\_CLK,       MTMS         |          | SD\_CLK               | SD                    |           |
| 14      | I/O | 12   | MTDI        | ADC2\_CH5,  RTC\_GPIO15, TOUCH5,   EMAC\_TXD3,  HSPIQ,     HS2\_DATA2, SD\_DATA2,  MTDI              |          | SD\_DATA2             | SD                    |           |
| 15      | P   | \-   | GND         | Ground                                                                                               |          |                       |                       |           |
| 16      | I/O | 13   | MTCK        | ADC2\_CH4,  RTC\_GPIO14, TOUCH4,   EMAC\_RX\_ER, HSPID,     HS2\_DATA3, SD\_DATA3,  MTCK             |          | SD\_DATA3             | SD                    |           |
| 17      | I/O | 9    | SD\_DATA\_2 | HS1\_DATA2,  U1RXD,  SD\_DATA2,  SPIHD                                                               |          |                       |                       |           |
| 18      | I/O | 10   | SD\_DATA\_3 | HS1\_DATA3,  U1TXD,  SD\_DATA3,  SPIWP                                                               |          |                       |                       |           |
| 19      | I/O | 11   | SD\_CMD     | HS1\_CMD,     U1RTS,   SD\_CMD,     SPICS0                                                           |          |                       |                       |           |
| 20      | I/O | 6    | SD\_CLK     | HS1\_CLK,       U1CTS,   SD\_CLK,       SPICLK                                                       |          |                       |                       |           |
| 21      | I/O | 7    | SD\_DATA\_0 | HS1\_DATA0,  U2RTS,   SD\_DATA0, SPIQ                                                                |          |                       |                       |           |
| 22      | I/O | 8    | SD\_DATA\_1 | HS1\_DATA1,  U2CTS,SD\_DATA1,  SPID                                                                  |          |                       |                       |           |
| 23      | I/O | 15   | MTDO        | ADC2\_CH3,  RTC\_GPIO13, TOUCH3,   EMAC\_RXD3,  HSPICS0,  HS2\_CMD,    SD\_CMD,     MTDO             |          | SD\_CMD               | SD                    |           |
| 24      | I/O | 2    | GPIO2       | ADC2\_CH2,  RTC\_GPIO12, TOUCH2,                           HSPIWP,   HS2\_DATA0, SD\_DATA0           |          | SD\_DATA0             | SD                    |           |
| 25      | I/O | 0    | GPIO0       | ADC2\_CH1,  RTC\_GPIO11, TOUCH1,   EMAC\_TX\_CLK, CLK\_OUT1                                          |          | Flashing              | Auto-Flash-Circuit    |           |
| 26      | I/O | 4    | GPIO4       | ADC2\_CH0,  RTC\_GPIO10, TOUCH0,   EMAC\_TX\_ER, HSPIHD,   HS2\_DATA1, SD\_DATA1,                    |          | SD\_DATA1             | SD                    |           |
| 27      | I/O | 16   | GPIO16      | HS1\_DATA4,  U2RXD,                           EMAC\_CLK\_OUT                                         |          | GPIO16                | Button (Logging)      |           |
| 28      | I/O | 17   | GPIO17      | HS1\_DATA5,  U2TXD,                           EMAC\_CLK\_OUT\_180                                    |          | GPIO17                | Button(Marker)        |           |
| 29      | I/O | 5    | GPIO5       | HS1\_DATA6,  VSPICS0,   EMAC\_RX\_CLK                                                                |          | VSPICS0               | LS7366R (Chip Select) |           |
| 30      | I/O | 18   | GPIO18      | HS1\_DATA7,   VSPICLK                                                                                |          | VSPICLK               | SPI                   |           |
| 31      | I/O | 19   | GPIO19      | U0CTS,VSPIQ,                 EMAC\_TXD0                                                              |          | VSPIQ (MISO)          | SPI                   |           |
| 32      | \-  | \-   | NC          | not connected                                                                                        |          |                       |                       |           |
| 33      | I/O | 21   | GPIO21      | VSPIHD, EMAC\_TX\_EN                                                                                 |          |                       |                       |           |
| 34      | I/O | 3    | RXDO        | U0RXD, CLK\_OUT2                                                                                     |          |                       |                       |           |
| 35      | I/O | 1    | TXDO        | U0TXD, CLK\_OUT3, EMAC\_RXD2                                                                         |          |                       |                       |           |
| 36      | I/O | 22   | GPIO22      | U0RTS,VSPIWP,              EMAC\_TXD1                                                                |          |                       |                       |           |
| 37      | I/O | 23   | GPIO23      | HS1\_STROBE, VSPID                                                                                   |          | VSPID (MOSI)          | SPI                   |           |
| 38      | P   | \-   | GND         | Ground                                                                                               |          |                       |                       |           |
| 39      | P   | \-   | GND         | Ground (Chipmitte)                                                                                   |          |                       |                       |           |
