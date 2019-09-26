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
#include <string>
#include <sys/stat.h>
#include <algorithm>
#include <vector>

using namespace std;

#define CMD "pcmanfm"   // Command to lauch files with
#define QUOT  "&#34;"   // XML escaped quote

string xmlEscape(string data) {

    string buffer;
    buffer.reserve(data.size());

    for (char chr : data) {
        switch (chr) {
            case '&':
                buffer.append("&amp;");
                break;

            case '\"':
                buffer.append(QUOT);
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
                buffer.append(&chr, 1);
                break;
        }
    }

    return buffer;
}

string escapeQuot(string str) {
    size_t idx;
    size_t pos = 0;

    while ((idx = str.find(QUOT, pos)) != string::npos) {
        str.insert(idx, "\\");
        pos = idx + sizeof(QUOT);
    }

    return str;
}

struct DirContent {
    vector <string> dirs;
    vector <string> files;
};

DirContent ReadDir(string mainDir) {
    DirContent content;

    struct stat filestat;
    const struct dirent *entry;

    DIR *dir;
    if ((dir = opendir(mainDir.c_str())) == NULL) {
        cerr << "opendir() error" << endl;
        return content;
    }

    string filepath;
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] != '.') {                   // it's not an hidden file

            filepath = mainDir + "/" + entry->d_name;
            if (stat(filepath.c_str(), &filestat) != 0) continue;

            if (S_ISDIR(filestat.st_mode))
                content.dirs.push_back(string(entry->d_name));
            else if S_ISREG(filestat.st_mode)
                content.files.push_back(string(entry->d_name));
        }
    }

    closedir(dir);
    return content;
}

int main(int argc, char **argv) {

    if (argc != 2) {
        cerr << "usage: " << argv[0] << " [directory]" << endl;
        return 1;
    }

    string dir(argv[1]);
    DirContent content = ReadDir(dir);
    string thisDir = xmlEscape(dir);
    string qEscapedDir = escapeQuot(thisDir);
    string escapedProgramName = xmlEscape(argv[0]);

    cout << "<openbox_pipe_menu><item label=\"Browse here...\">" <<
         "<action name=\"Execute\"><execute>" << CMD << " " << QUOT << qEscapedDir << QUOT << "</execute></action>" <<
         "</item><separator/>";

    sort(content.files.begin(), content.files.end());
    for (string &name : content.files) {
        string escapedName =  xmlEscape(name);
        cout << "<item label=\"" << escapedName << "\">" <<
             "<action name=\"Execute\">" <<
             "<execute>" << CMD << " " << QUOT << qEscapedDir << "/" << escapeQuot(escapedName) << QUOT << "</execute>" <<
             "</action>" <<
             "</item>";
    }

    sort(content.dirs.begin(), content.dirs.end());
    for (string &name : content.dirs) {
        string escapedName =  xmlEscape(name);
        cout << "<menu id=\"" << thisDir << "/" << escapedName << "\"" <<
             " label=\"" << escapedName << "\"" <<
             " execute=\"" << escapedProgramName << " " << QUOT << qEscapedDir << "/" << escapeQuot(escapedName) << QUOT <<
             "\"/>";
    }

    cout << "</openbox_pipe_menu>\n";
    return 0;
}
