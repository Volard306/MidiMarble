// +++++++ MidiMarble v2.1 by Volard306 2026 +++++++
// Licensed under MIT license.  Not to be used for any commercial or in any device that is sold.
// You are free to use this code in any way but if you do use the whole or parts of it, please atrribute me in your comment block at the start of your sketch
// This code was written with a lot of help from ChatGPT's free coding help.  I am not a coder so this is my first attempt at making an actual Arduino sketch that does something more than the built-in
// examples.

// Usage:  This sketch is to create a 32 knob midi cc controller to be able to map vst parameters within a DAW (or standalone if your vst allows midi mapping)
// BOM: 1 Raspberry pi pico (2040 device), 2 CD74HC4067 analog multiplexers, 32 x 10k Linear (B type) potentiometers, wire, connectors if you want and breadboard, perfboard or even a custom pcb if you
// want.  (I may have one made at some point and if so please check back at my github for this project: https://github.com/Volard306/MidiMarble)
// Wiring: the 1st mux sig line goes to gp26 (A0) on my Pico Marble variant, the switch lines connected to S0-S3 go to digital io GP0-GP3, vcc to 3.3v on pico, ground to pico.  
// Left pin on pot is ground, Right pin is 3.3v and middle pin (wiper) goes to the inputs on the mux.  I just wired them in random orders as it doesn't matter as I'm mapping them by their location in
// physical space, not on their mux position.  Second Mux is wired to gp27 (A1) on pico marble variant.  Second mux connection pins (the s0-s3 are not connected to the IO on the pico.  They should be
// tied low to ground to prevent floating state (although i don't have mine grounded and see no ill effects.  I will probably update the wiring again when i do a more permanent job)
// I also have a 22nF box capacitor at the end of the signal line before it comes into the pico's ADC pins.  This helps a lot with jitter on my build.  On my permanent build i will probably ground the 
// case of the pots to also help with interferance as the code does have a threshold and averaging algorithm to reduce the extra pulses.  It is not perfect so the caps seem to help.

#include <Adafruit_TinyUSB.h>

// ===================== MIDI =====================
Adafruit_USBD_MIDI usb_midi;

const uint8_t MIDI_CHANNEL = 0;
const uint8_t BASE_CC = 20;

// ===================== MUX =====================
#define N_MUX 2
#define CHANNELS_PER_MUX 16
#define N_POTS (N_MUX * CHANNELS_PER_MUX)

const uint8_t muxS0 = 0;
const uint8_t muxS1 = 1;
const uint8_t muxS2 = 2;
const uint8_t muxS3 = 3;

const uint8_t muxSigPins[N_MUX] = { A0, A1 };

// ===================== FILTERING / FEEL =====================
const int AVG_DEPTH      = 8;   // smoothing
const int POT_THRESHOLD  = 6;   // ADC movement to wake pot

const uint8_t DEFAULT_MIDI_STEP = 2;  // normal pots
const uint8_t NOISY_MIDI_STEP   = 4;  // stubborn pot

// ===================== STATE =====================
int potAvg[N_POTS]    = {0};
int potPrev[N_POTS]   = {0};
int potStable[N_POTS] = {0};
int midiPrev[N_POTS]  = { -1 };

uint8_t midiStep[N_POTS];

int potMin[N_POTS];
int potMax[N_POTS];
bool invertPot[N_POTS];

// ===================== SETUP =====================
void setup() {

  pinMode(muxS0, OUTPUT);
  pinMode(muxS1, OUTPUT);
  pinMode(muxS2, OUTPUT);
  pinMode(muxS3, OUTPUT);

  analogReadResolution(12);

  usb_midi.begin();

  for (int i = 0; i < N_POTS; i++) {
    potMin[i]    = 200;
    potMax[i]    = 3900;
    invertPot[i] = true;

    potAvg[i]    = 0;
    potPrev[i]   = 0;
    potStable[i] = 0;
    midiPrev[i]  = -1;

    midiStep[i]  = DEFAULT_MIDI_STEP;
  }

  // 🎯 MIDI-OX shows this as "pot 2A" (hex)
  // CC 0x2A = 42 decimal
  // 42 - BASE_CC (20) = index 22
  midiStep[22] = 6;
}

// ===================== LOOP =====================
void loop() {

  for (uint8_t channel = 0; channel < CHANNELS_PER_MUX; channel++) {

    setMuxChannel(channel);

    for (uint8_t mux = 0; mux < N_MUX; mux++) {

      uint8_t i = mux * CHANNELS_PER_MUX + channel;

      int raw = analogRead(muxSigPins[mux]);

      // running average
      potAvg[i] =
        (potAvg[i] * (AVG_DEPTH - 1) + raw) / AVG_DEPTH;

      int delta = abs(potAvg[i] - potStable[i]);

      // wake-up movement
      if (delta > POT_THRESHOLD) {

        int norm = constrain(potAvg[i], potMin[i], potMax[i]);

        int midiVal = invertPot[i]
          ? map(norm, potMin[i], potMax[i], 127, 0)
          : map(norm, potMin[i], potMax[i], 0, 127);

        midiVal = constrain(midiVal, 0, 127);

        // MIDI-domain hysteresis (per pot)
        if (midiPrev[i] < 0 ||
            abs(midiVal - midiPrev[i]) >= midiStep[i]) {

          sendCC(BASE_CC + i, midiVal);
          midiPrev[i] = midiVal;
        }

        potStable[i] = potAvg[i];
      }

      potPrev[i] = potAvg[i];
    }
  }
}

// ===================== HELPERS =====================
void setMuxChannel(uint8_t channel) {
  digitalWrite(muxS0, (channel >> 0) & 0x01);
  digitalWrite(muxS1, (channel >> 1) & 0x01);
  digitalWrite(muxS2, (channel >> 2) & 0x01);
  digitalWrite(muxS3, (channel >> 3) & 0x01);
}

void sendCC(uint8_t cc, uint8_t value) {
  uint8_t msg[3] = {
    (uint8_t)(0xB0 | MIDI_CHANNEL),
    cc,
    value
  };
  usb_midi.write(msg, 3);
}
