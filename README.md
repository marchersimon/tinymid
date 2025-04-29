# What is `tinymid`?

`tinymid` is a command-line tool for simplifying MIDI files. It was originally developed to create simple-to-parse MIDI files for a microcontroller-based tesla coil interrupter, but can be used in any case, where a lightweight and minimalistic MIDI implenentation is needed.

Files with "single track file format" or "multiple song file format" aren't supported yet, but almost all music writing software (like Musescore) and therefore most MIDI files on the internet use the "multiple track file format" by default, which is supported.

The tool parses the original file and simplifies it by
* merging all tracks into a single one,
* moving all events to Channel 0,
* turning `NOTE_ON` events with a velocity of 0 into `NOTE_OFF` events, and
* removing all events (including meta and sysex events), except `NOTE_ON`, `NOTE_OFF`, `SET_TEMPO` and `END_OF_TRACK`.

With the `debug` and `dryrun` option, this tool can also be used to analyse MIDI files and show their contents.

Big thanks to Ichiro Fujinaga, who wrote a very detailed explanation of the MIDI file format ([https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html](https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html)).

## Usage

```
Usage: ./tinymid [options] <file>
Options:
  -d, --debug         Enable debug logging and print MIDI events
  -n, --no-color      Disable colored output
  -o, --output        Specify the name for the output file (defaults to "out.mid")
  -r, --dryrun        Don't write anything to a file. Used for debugging purposes
  -h, --help          Print this help message
```

## Example

Using the debug option to analyze a file can look like this:

```
File format 1: Multiple track file format

Track 1:

  Pos  |           Raw Event Source             |  AbsT  |  RelT  |        Event Type         |  Channel   |    Event Details
0x16   | 00 FF 03 05 50 69 61 6E 6F             | 0      | 0      | Sequence or track name    |            | Piano
0x1F   | 00 FF 58 04 03 02 18 08                | 0      | 0      | Time signature            |            | 
0x27   | 00 FF 59 02 00 00                      | 0      | 0      | Key signature             |            | 
0x2D   | 00 FF 51 03 0F 42 40                   | 0      | 0      | Tempo setting             |            | 1 seconds per quarter note
0x34   | 00 B0 79 00                            | 0      | 0      | Control change            | Channel 0  | 
0x54   | 00 5D 00                               | 0      | 0      | Control change            | Channel 0  | 
0x5C   | 00 90 4C 50                            | 0      | 0      | Note on                   | Channel 0  | Note E5  at velocity 80
0x60   | 00 B1 79 00                            | 0      | 0      | Control change            | Channel 1  | 
0x88   | 83 5F 90 4C 00                         | 479    | 479    | Note on                   | Channel 0  | Note E5  at velocity 0
0x8D   | 01 45 50                               | 480    | 1      | Note on                   | Channel 0  | Note A4  at velocity 80
0x90   | 81 6F 45 00                            | 719    | 239    | Note on                   | Channel 0  | Note A4  at velocity 0
0xA5   | 81 6F 4C 00                            | 1439   | 239    | Note on                   | Channel 0  | Note E5  at velocity 0
0xA9   | 01 45 50                               | 1440   | 1      | Note on                   | Channel 0  | Note A4  at velocity 80
0xAC   | 83 5F 45 00                            | 1919   | 479    | Note on                   | Channel 0  | Note A4  at velocity 0
0xB0   | 01 4C 50                               | 1920   | 1      | Note on                   | Channel 0  | Note E5  at velocity 80
0xB3   | 81 6F 4C 00                            | 2159   | 239    | Note on                   | Channel 0  | Note E5  at velocity 0
0xB7   | 01 4A 50                               | 2160   | 1      | Note on                   | Channel 0  | Note D5  at velocity 80
0xBA   | 81 6F 4A 00                            | 2399   | 239    | Note on                   | Channel 0  | Note D5  at velocity 0
...
```

`Pos` is the event's starting position in the file, `AbsT` is the absolute time and `RelT` is the relative time (delta time).

## Build

The tool can be built by running `make` or `make debug` for a debuggable executable. The only requirements should be `make` itself and the `g++` compiler (C++17 or later). So far, the tool has only been tested on Linux.
