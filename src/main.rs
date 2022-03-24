use std::{fs::File, env};
use std::io::Read;
use clap::{arg, Command};
use log::{error};

mod midi;
use midi::*;


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

    opts
}

pub fn read_file(name: String) -> Result<Vec<u8>, std::io::Error>{
    let mut file = File::open(&name)?;
    let metadata = std::fs::metadata(&name)?;
    let mut buffer = vec![0; metadata.len() as usize];
    file.read_exact(&mut buffer)?;
    Ok(buffer)
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

    
    let buffer: Vec<u8>;
    match read_file(opts.infile) {
        Ok(buff) => buffer = buff,
        Err(err) => {
            error!("{}", err.to_string());
            std::process::exit(1);
        },
    };

    let _mid1;
    match MIDIFile::from_buffer(buffer) {
        Ok(mid) => _mid1 = mid,
        Err(str) => {
            error!("{}", str);
            std::process::exit(1);
        },
    }
}