#ifndef CRAWLERTHREAD_H
#define CRAWLERTHREAD_H


#include <QThread>
#include <set>
#include <string>
#include "../core/output/formats.h"


enum CrawlerOptions {
    KEEP_TALK_PAGE_FILES
};

class CrawlerThread : public QThread
{
    Q_OBJECT
public:

    CrawlerThread(QString _input_file_path, QString _output_folder, std::set<Grawitas::Format> _formats, std::set<CrawlerOptions> _options);

    void run();

private:
	std::string input_file_path;
	std::string output_folder;
    std::set<Grawitas::Format> formats;
    std::set<CrawlerOptions> options;
    std::map<std::string, std::ofstream*> raw_talk_page_files;

public slots:
	void start_raw_talk_page_file(std::string normalized_title, std::string, std::string content);

signals:
    void write_status(std::string status_message);
};

#endif // CRAWLERTHREAD_H
