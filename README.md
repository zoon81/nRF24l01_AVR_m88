### Intro
This project was made as an experiment with nRF24l01+ radios.This is a powerfull and cheap wireless interface for any microcontroller project.<br />
I trying to design and make a radio controllerd r/c car.I have everything exept the controllers (receiver and transmitter).
I going to use an ATMEGA88PA micro in the trasmitter and in the receiver too.<br />
### Features
  PWM controlled speed, left/right turn, front lights<br />
  Handbreak<br />
  Out of range detection<br />
  Battery status indicator<br />
### Source code structure
  HAL       --- Hardware Abstraction Layer, low level peripherial drivers<br />
  Drivers   --- External device driver like the nRF24l01 radio<br />
  Utils     --- Everything else<br />
