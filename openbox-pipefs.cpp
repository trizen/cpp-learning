// Openbox file browser, in C++

/*
    Recursively browse filesystem through openbox3 pipe menus

    Author: Daniel "Trizen" Șuteu
    License: GPLv3
    Date: 09 January 2014
    http://trizenx.blogspot.com

    Compilation:
        g++ -std=c++11 -march=native -Ofast openbox-pipefs.cpp -o openbox-pipefs

    ---------------------------------------------------
    Add a new entry in your menu.xml:
        <menu id="openbox-pipefs" label="Disk" execute="openbox-pipefs ."/>
    ---------------------------------------------------

    ---------------------------------------------------
    If you are using the 'obmenu-generator' program, add in schema.pl:
        {pipe => ["openbox-pipefs .", "Disk", "drive-harddisk"]},
    ---------------------------------------------------

    See also: http://trizenx.blogspot.ro/2012/12/obbrowser.html
*/

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>

using namespace std;

// Command to lauch files with
#define CMD "pcmanfm"

// HTML/XML encoded quote
#define QUOT  "&quot;"
#define EQUOT "\\&quot;"

string encode(string data) {

    const size_t dataSize = data.size();

    string buffer;
    buffer.reserve(dataSize);

    for (size_t pos = 0; pos != dataSize; ++pos) {
        switch (data[pos]) {
            case '&':
                buffer.append("&amp;");
                break;

            case '\"':
                buffer.append("&quot;");
                break;

            case '\'':
                buffer.append("&apos;");
                break;

            case '<':
                buffer.append("&lt;");
                break;

            case '>':
                buffer.append("&gt;");
                break;

            default:
                buffer.append(&data[pos], 1);
                break;
        }
    }

    return buffer;
}

void searchReplace(string &data, const char *search, const char *replace) {

    const size_t dataSize = data.size();
    const size_t searchSize = strlen(search);

    string buffer;
    buffer.reserve(dataSize);

    for (size_t pos = 0; pos < dataSize; ++pos) {
        if (pos + searchSize <= dataSize) {
            size_t len = 0;

            for (size_t j = 0; j < searchSize; ++j) {
                if (data[pos + j] == search[j]) ++len;
                else break;
            }

            if (len == searchSize) {
                buffer.append(replace);
                pos += len - 1;
                continue;
            }
        }

        buffer.append(&data[pos], 1);
    }

    data.swap(buffer);
}

string make_dir_element(const string name, string path, const string cmd) {
    searchReplace(path, QUOT, EQUOT);

    return
        "<menu id=\"" + path + "\"" +
        " label=\"" + name + "\"" +
        " execute=\"" + cmd  + " " + QUOT + path + QUOT + "\"/>\n";
}

string make_file_element(const string name, string path, const string cmd) {
    searchReplace(path, QUOT, EQUOT);

    return
        "<item label=\"" + name + "\">" +
        "<action name=\"Execute\">" +
        "<execute>" + cmd + " " + QUOT + path + QUOT + "</execute>" +
        "</action>" +
        "</item>\n";
}

int main(int argc, char **argv) {

    if (argc != 2) {
        cerr << "usage: " << argv[0] << " [directory]" << endl;
        return 1;
    }

    const string programName(encode(argv[0]));

    const char *mainDir = argv[1];
    char path[4096];

    struct stat info;
    const struct dirent *entry;

    struct File {
        const string name;
        const string path;
    };

    vector <File> files;
    vector <File> dirs;

    DIR *dir;

    if ((dir = opendir(mainDir)) == NULL) {
        cerr << "opendir() error" << endl;
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        const char *fileName = entry->d_name;

        if (fileName[0] != '.') {   // it's not an hidden file
            sprintf(path, "%s/%s", mainDir, fileName);
            string pathString(path);
            string nameString(fileName);

            if (stat(path, &info) != 0) {
                // it's not readable
            }
            else if (S_ISDIR(info.st_mode)) {
                dirs.push_back({nameString, pathString});
            }
            else {
                files.push_back({nameString, pathString});
            }
        }
    }

    closedir(dir);

    string thisPath(mainDir);
    thisPath = encode(thisPath);
    searchReplace(thisPath, QUOT, EQUOT);

    cout << "<openbox_pipe_menu>"
         << "<item label=\"Browse here...\"><action name=\"Execute\">"
         << "<execute>" << CMD << " " << QUOT << thisPath << QUOT << "</execute>"
         << "</action></item><separator/>\n";

    for (const File file : files) {
        cout << make_file_element(
                 encode(file.name),
                 encode(file.path),
                 CMD
             );
    }

    for (const File directory : dirs) {
        cout << make_dir_element(
                 encode(directory.name),
                 encode(directory.path),
                 programName
             );
    }

    cout << "</openbox_pipe_menu>\n";
    return 0;
}
