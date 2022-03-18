use std::{fs::File};
use std::io::Read;

use clap::{arg, Command};

pub struct Options {
    infile: String,
    debug: bool,
}

pub struct MIDIFile {
    buffer: Vec<u8>,
    pos: usize,
    file_format: u16,
    number_of_track_chunks: u16,
    division: i16,
}

impl MIDIFile {
    fn check_header(&mut self) -> bool {
        // Identifier
        let identifier = self.get_string(4);
        if identifier != "MThd" {
            println!("Wrong identifier for header chunk");
            println!("Expected \"MThd\", but got \"{}\"", identifier);
            panic!("Panic");
        }
        
        // Header Length
        let header_lenght = self.get_dword();
        if header_lenght != 6 {
            println!("Wrong header chunk length");
            println!("Expected 6, but got {}", header_lenght);
            panic!("Panic");
        }

        // File format
        self.file_format = self.get_word();
        match self.file_format {
            0 => println!("Single Track File Format"),
            1 => println!("Multiple Track File Format"),
            2 => println!("Multiple Song File Format"),
            _ => {
                println!("Invalid file format: {}", self.file_format);
                panic!("Panic");
            }
        }

        // Number of Track Chunks
        self.number_of_track_chunks = self.get_word();
        if self.number_of_track_chunks == 0 {
            println!("MIDI File must have at least one track chunk");
            panic!("Panic");
        }

        // Division
        self.division = self.get_word() as i16;
        if self.division > 0 {
            println!("Division given in ticks per beat");
        } else if self.division < 0 {
            println!("Division given in SMPTE format");
        } else {
            println!("Division cannot be zero");
            panic!("Panic");
        }
        true
    }

    fn get_byte(&mut self) -> u8 {
        if self.pos == self.buffer.len() {
            println!("File ended too early");
            panic!("Panic");
        }
        let byte = self.buffer[self.pos];
        self.pos += 1;
        byte

    }
    
    fn get_string(&mut self, len: usize) -> String {
        let mut str = String::new();
        for _i in 0..len {
            str.push_str(&(self.get_byte() as char).to_string());
        }
        str
    }

    fn get_word(&mut self) -> u16 {
        (self.get_byte() as u16) << 8  |
        (self.get_byte() as u16)
    }

    fn get_dword(&mut self) -> u32{
        (self.get_byte() as u32) << 24 |
        (self.get_byte() as u32) << 16 |
        (self.get_byte() as u32) << 8  |
        (self.get_byte() as u32)
    }
}

pub fn cli_parse() -> Options {
    let matches = Command::new("tinymid")
        .version("0.1")
        .about("A command line MIDI viewer")
        .arg(arg!(<infile> "File to read"))
        .arg(arg!(
            -d --debug ... "Turn debugging information on"
        ))
        .get_matches();

    let opts = Options{infile: matches.value_of("infile").unwrap().to_string(), debug: matches.is_present("debug")};

    return opts;
}

pub fn read_file(name: String) -> Vec<u8>{
    let mut file = File::open(&name).expect("Could not open file");
    let metadata = std::fs::metadata(&name).expect("Could not read metadata of file");
    let mut buffer = vec![0; metadata.len() as usize];
    file.read(&mut buffer).expect("buffer overflow");
    buffer
}

fn main() {

    let opts = cli_parse();

    println!("Infile: {}", opts.infile);
    println!("Debug: {}", opts.debug);
    
    let buffer = read_file(opts.infile);

    println!("File is {} Bytes long", buffer.len());

    let mut mid1 = MIDIFile{buffer, pos: 0, file_format: 4, number_of_track_chunks: 0, division: 0};
    mid1.check_header();
}