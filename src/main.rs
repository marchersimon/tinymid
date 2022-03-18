use std::fs::File;
use std::io::Read;

use clap::{arg, Command};

pub struct Options {
    infile: String,
    debug: bool,
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
}