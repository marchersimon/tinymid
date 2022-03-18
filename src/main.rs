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

fn main() {

    let opts = cli_parse();

    println!("Infile: {}", opts.infile);
    println!("Debug: {}", opts.debug);
    
}