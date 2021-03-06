#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"
#include <boost/algorithm/string/trim.hpp>

#include "../core/helpers/stepTimer.h"
#include "../core/output/formats.h"

#include "../core/parsing/xmlDumpParserWrapper.h"
#include "../core/output/outputHelpers.h"

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

    cxxopts::Options options("grawitas_cli_xml", "Grawitas CLI xml parser.");
	options.add_options()
		("help", "Produce help message.")
		("input-xml-file", "The XML file that is part of a dump of Wikipedia.", cxxopts::value<string>())
		("output-folder", "The folder in which the results should be stored. WARNING: always has to end with / or \\.", cxxopts::value<string>())
		("article-list-file", "Path to a file containing the list of articles for which talk pages should be parsed.", cxxopts::value<string>())

		// network output
		("user-network-gml", "Flag indicating the output of the user network of the talk page in an .gml-format.")
		("user-network-graphml", "Flag indicating the output of the user network of the talk page in an .graphml-format.")
		("user-network-graphviz", "Flag indicating the output of the user network of the talk page in an .graphviz-format.")

		("comment-network-gml", "Flag indicating the output of the comment network of the talk page in an .gml-format.")
		("comment-network-graphml", "Flag indicating the output of the comment network of the talk page in an .graphml-format.")
		("comment-network-graphviz", "Flag indicating the output of the comment network of the talk page in an .graphviz-format.")

		("two-mode-network-gml", "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .gml-format.")
		("two-mode-network-graphml", "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .graphml-format.")
		("two-mode-network-graphviz", "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .graphviz-format.")

		// list output
		("comment-list-csv", "Flag indicating the output of a list of comments with user, date, and parent comment in .csv-format.")
		("comment-list-human-readable", "Flag indicating the output of a list of comments with user, date, and parent comment in an human readable format.")
		("comment-list-json", "Flag indicating the output of a list of comments with user, date, and parent comment in a .json-format.")

		// misc
		("show-timings", "Show the timings for the different parsing steps.")
		;

	options.parse(argc, argv);

	// display help if --help was specified
	if (options.count("help")) {
		cout << options.help() << endl;
		return 0;
	}

	if(!options.count("input-xml-file") || !options.count("output-folder"))
	{
		std::cerr << "Please specify the parameters --input-xml-file and --output-folder." << std::endl;
		return 1;
	}

	const std::string input_file = options["input-xml-file"].as<std::string>();
	const std::string output_folder = normalize_folder_path(options["output-folder"].as<std::string>());

	std::set<Format> formats;
	for (auto form_parameter : FormatParameterStrings) 
		if(options[form_parameter].as<bool>())
			formats.insert(parameter_to_format(form_parameter));

	try {
		xml_dump_parsing(input_file, output_folder, formats);
	}
	catch(const std::exception& exception) {
		std::cerr << "--------------------------------------------------" << std::endl;
		std::cerr << "FATAL ERROR: The application terminated with an exception:" << std::endl;
		std::cerr << exception.what() << std::endl;
		std::cerr << "--------------------------------------------------" << std::endl;
	}

	return 0;
}
