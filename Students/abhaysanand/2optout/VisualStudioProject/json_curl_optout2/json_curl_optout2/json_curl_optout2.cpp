// json_curl_optout2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "HTTPDownloader.hpp"

//using namespace std;

uint write_data(void *ptr, uint size, uint nmemb, FILE *stream) {
    uint written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int main()
{
    HTTPDownloader downloader;
    std::string content = downloader.download("http://www.hbo.com/game-of-thrones/cast-and-crew/");
    std::string character, actor;
    const std::string quotes = "'";
    uint cBegin = 0, cEnd = 0, aBegin = 0, aEnd = 0;
    uint cat_appearances = 0;

    Json::Value obj;

    for (cBegin = content.find("0.0.1.0\">", 0); cBegin != std::string::npos; cBegin = content.find("0.0.1.0\">", aEnd))
    {
        cEnd = content.find("/h4", cBegin);
        character = content.substr(cBegin + 9, cEnd - (cBegin + 9) - 1);
        (void)replace(character, "&quot;", quotes);
        (void)replace(character, "&quot;", quotes);
        (void)replace(character, "&#x27;", "'");
        (void)replace(character, "\n", "");

        if (character == "David Benioff")
        {
            break;
        }
        aBegin = content.find(">played by ", cBegin);
        aEnd = content.find("/h5", aBegin);
        actor = content.substr(aBegin + 11, aEnd - (aBegin + 11) - 1);
        (void)replace(actor, "&quot;", quotes);
        (void)replace(actor, "&quot;", quotes);
        (void)replace(actor, "&#x27;", "'");
        (void)replace(actor, "\n", "");

        cat_appearances++;
        obj[character] = actor;
    }

    std::ofstream jsonFileWrite;

    jsonFileWrite.open("GoT.json");
    jsonFileWrite << obj << std::endl;
    jsonFileWrite.close();

    std::cout << "JSON data write complete" << std::endl;

    std::string line;
    std::ifstream jsonFileRead("GoT.json");
    std::ofstream mdFileWrite;

    mdFileWrite.open("GoT.md");

    mdFileWrite << "# Game of Thrones - Cast" << std::endl;

    if (jsonFileRead.is_open())
    {
        while (getline(jsonFileRead, line))
        {
            if ((line == "{") || (line == "}"))
            {
                mdFileWrite << std::endl;
                continue;
            }

            (void)replace(line, "\t", "");
            (void)replace(line, "\"", "**");
            (void)replace(line, "\"", "**");
            (void)replace(line, ":", "played by");
            (void)replace(line, "\"", "**");
            (void)replace(line, "\"", "**");
            (void)replace(line, ",", "\n");
            mdFileWrite << "* " << line << '\n';
        }
        jsonFileRead.close();
        mdFileWrite.close();
        std::cout << "MD data write complete" << std::endl;
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }

    _getch();
    return 0;
}
