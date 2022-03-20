use std::{fs::File, env};
use std::io::Read;
use clap::{arg, Command};
use log::{debug, error};
use env_logger::Builder;

pub struct Options {
    infile: String,
    debug: bool,
}

pub struct MIDIFile {
    buffer: Vec<u8>,
    pos: usize,
    file_format: FileFormat,
    number_of_track_chunks: u16,
    division: i16,
}

enum FileFormat {
    SingleTrack,
    MultipleTrack,
    MultipleSong,
}

impl MIDIFile {
    
    fn read_header(&mut self) -> bool {
        // Identifier
        let identifier = self.get_string(4);
        if identifier != "MThd" {
            error!("Wrong identifier for header chunk");
            error!("Expected \"MThd\", but got \"{}\"", identifier);
            panic!("Panic");
        }
        
        // Header Length
        let header_lenght = self.get_dword();
        if header_lenght != 6 {
            error!("Wrong header chunk length");
            error!("Expected 6, but got {}", header_lenght);
            panic!("Panic");
        }
        
        // File format
        match self.get_word() {
            0 => {
                debug!("Single Track File Format");
                self.file_format = FileFormat::SingleTrack;
            },
            1 => {
                debug!("Multiple Track File Format");
                self.file_format = FileFormat::MultipleTrack;
            },
            2 => {
                debug!("Multiple Song File Format");
                self.file_format = FileFormat::MultipleSong;
            },
            file_format @ _ => {
                error!("Invalid file format: {}", file_format);
                panic!("Panic");
            }
        }
        
        // Number of Track Chunks
        self.number_of_track_chunks = self.get_word();
        if self.number_of_track_chunks == 0 {
            error!("MIDI File must have at least one track chunk");
            panic!("Panic");
        }
        
        // Division
        self.division = self.get_word() as i16;
        if self.division > 0 {
            debug!("Division given in ticks per beat");
        } else if self.division < 0 {
            debug!("Division given in SMPTE format");
        } else {
            error!("Division cannot be zero");
            panic!("Panic");
        }
        true
    }

    fn new(buffer: Vec<u8>) -> Option<MIDIFile> {
        let file = MIDIFile {
            buffer,
            pos: 0,
            file_format: FileFormat::SingleTrack,
            number_of_track_chunks: 0,
            division: 0,
        };
        Some(file)
    }
    
    fn get_byte(&mut self) -> u8 {
        if self.pos == self.buffer.len() {
            error!("File ended too early");
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
    
    if opts.debug {
        env::set_var("RUST_LOG", "debug");
    }

    env_logger::builder()
        .format_timestamp(None)
        .format_module_path(false)
        .init();

    
    let buffer = read_file(opts.infile);

    let mut mid1;
    match MIDIFile::new(buffer) {
        Some(mid) => mid1 = mid,
        None => panic!("aa"),
    }
    mid1.read_header();
}