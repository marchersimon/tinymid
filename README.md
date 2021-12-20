This program takes a MIDI file and simplifies it by:

* merging all tracks into a single one,
* moving all events to Channel 0,
* removing all events, except `NOTE_ON`, `NOTE_OFF`, `SET_TEMPO` and `END_OF_TRACK`.

In this form, the MIDI file is very easy to use with MIDI-Interrupters for tesla coils.

This is still work in progress.