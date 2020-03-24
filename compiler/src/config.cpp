//
// Created by zhaoyiping on 2020/3/18.
//

#include "config.h"
#include "iostream"
#include "string"

Config::Config() {}
Config::Config(int argc, const char *argv[]) {
	program = argparse::ArgumentParser(argv[0]);
	configs();
	try {
		program.parse_args(argc, argv);
	} catch (std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << program << std::endl;
		exit(1);
	}
	init();
}
void Config::output() {
#define debugstr(x) std::cerr << #x << " " << x << std::endl;
	debugstr(inputFileName);
	debugstr(outputFileName);
	debugstr(debug);
#undef debugstr
}
void Config::init() {
	setValues();
	inputFile.open(inputFileName, std::ios_base::in);
	outputFile.open(outputFileName, std::ios_base::out);
	if (!inputFile.is_open()) {
		std::cerr << "file \"" << inputFileName << "\" not found" << std::endl;
		exit(1);
	}
}
void Config::configs() {
	program.add_argument("file")
			.help("input file name");
	program.add_argument("--debug")
			.help("open the debug mode")
			.default_value(false)
			.implicit_value(true);
	program.add_argument("-o", "--output")
			.help("output file name")
			.default_value((std::string)"a.asm");
}
void Config::setValues() {
	inputFileName = program.get<std::string>("file");
	debug = program.get<bool>("--debug");
	outputFileName = program.get<std::string>("--output");
}
