#include <iostream>
#include <fstream>
#include <QtCore>
#include <QMetaType>

#include "../../libs/cxxopts/include/cxxopts.hpp"

#include "../core/output/outputWrapper.h"
#include "../core/output/formats.h"
#include "../core/parsing/coreTalkPageParsing.h"
#include "../crawler/models.h"
#include "../crawler/crawling.h"
#include "crawler_task.h"

#include "../core/output/outputHelpers.h"
#include <boost/algorithm/string/trim.hpp>

Q_DECLARE_METATYPE(std::string)

using namespace Grawitas;
using namespace std;

std::set<Grawitas::Format> formats_from_parameters(const cxxopts::Options& arguments)
{
	std::set<Grawitas::Format> formats;

	for (auto form_parameter : Grawitas::FormatParameterStrings) 
		if(arguments.count(form_parameter) > 0 && arguments[form_parameter].as<bool>())
			formats.insert(Grawitas::parameter_to_format(form_parameter));

	return formats;
}

std::vector<std::string> read_titles_from_file(std::string file_path)
{
    std::vector<std::string> titles;
    std::ifstream input_file(file_path);
    std::string line;
    while(std::getline(input_file,line))
    {
        boost::trim(line);
        titles.push_back(line);
    }

	return titles;
}


int main(int argc, char** argv)
{
    cxxopts::Options options("grawitas_cli_crawler", "Grawitas CLI crawler parser.");
    options.add_options()
                    ("h,help", "Produces this help message.")

                    ("i,talk-page-list-file", "Path to an input file containing titles of Wikipedia articles.", cxxopts::value<string>())
                    ("o,output-folder", "Path to an input file containing a talk page in the Wikipedia syntax.", cxxopts::value<string>())

                    // network output
                    ("user-network-gml", "Set this flag for exporting the user network in GML format.")
                    ("user-network-graphml", "Set this flag for exporting the user network in GraphML format.")
                    ("user-network-graphviz", "Set this flag for exporting the user network in GraphViz format.")

                    ("comment-network-gml", "Set this flag for exporting the comment network in GML format.")
                    ("comment-network-graphml", "Set this flag for exporting the comment network in GraphML format.")
                    ("comment-network-graphviz", "Set this flag for exporting the comment network in GraphViz format.")

                    ("two-mode-network-gml", "Set this flag for exporting the two-mode network in GML format.")
                    ("two-mode-network-graphml", "Set this flag for exporting the two-mode network in GraphML format.")
                    ("two-mode-network-graphviz", "Set this flag for exporting the two-mode network in GraphViz format.")

                    // list output
                    ("comment-list-csv", "Set this flag for exporting the list of comments as a CSV format.")
                    ("comment-list-human-readable", "Set this flag for exporting the list of comments as a human readable format.")
                    ("comment-list-json", "Set this flag for exporting the list of comments as a JSON format (recommended).")

                    ("keep-raw-talk-pages", "Set this flag if you want to keep the raw crawled talk pages.")
                    // misc
                    ("show-timings", "Show the timings for the different steps.")
                    ;

	options.parse(argc, argv);

    // show help and exit program if --help is set
    if (options.count("help")) {
		cout << options.help() << endl;
        return 0;
    }

    qRegisterMetaType<std::string>();


	const auto formats = formats_from_parameters(options);
	const auto input_file = options["talk-page-list-file"].as<std::string>();
	const auto output_folder = normalize_folder_path(options["output-folder"].as<std::string>());
	const auto titles = read_titles_from_file(input_file);

	QCoreApplication a(argc, argv);

	CrawlerTask task(&a,titles, output_folder, formats);

	QObject::connect(&task, &CrawlerTask::finished, &QCoreApplication::quit);

	// This will run the task from the application event loop.
	
	QTimer::singleShot(0, &task, &CrawlerTask::run);

	return a.exec();
}
