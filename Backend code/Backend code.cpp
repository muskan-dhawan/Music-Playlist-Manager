#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <sstream>
using namespace std;

struct Song {
    string name, artist, album;
    int duration;
    Song(string n, string a, string al, int d)
        : name(n), artist(a), album(al), duration(d) {}
};

class Playlist {
private:
    vector<Song> songs;
    mutex mtx;

public:
    void addSong(const Song& song) {
        lock_guard<mutex> lock(mtx);
        songs.push_back(song);
    }

    void deleteSong(const string& name) {
        lock_guard<mutex> lock(mtx);
        songs.erase(remove_if(songs.begin(), songs.end(),
                              [&](const Song& s) { return s.name == name; }),
                    songs.end());
    }

    void shuffle() {
        lock_guard<mutex> lock(mtx);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(songs.begin(), songs.end(), default_random_engine(seed));
    }

    void saveToFile(const string& filename) {
        lock_guard<mutex> lock(mtx);
        ofstream out(filename);
        for (const auto& s : songs) {
            out << s.name << "|" << s.artist << "|" << s.album << "|" << s.duration << "\n";
        }
    }

    void loadFromFile(const string& filename) {
        lock_guard<mutex> lock(mtx);
        songs.clear();
        ifstream in(filename);
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string name, artist, album, durStr;
            getline(ss, name, '|');
            getline(ss, artist, '|');
            getline(ss, album, '|');
            getline(ss, durStr, '|');
            if (!name.empty()) {
                songs.emplace_back(name, artist, album, stoi(durStr));
            }
        }
    }

    void printAll() {
        lock_guard<mutex> lock(mtx);
        cout << "Playlist:\n";
        for (const auto& s : songs) {
            cout << " - " << s.name << " | " << s.artist 
                 << " | " << s.album 
                 << " | " << s.duration << "s\n";
        }
    }
};

int main() {
    Playlist playlist;

    // Add songs
    playlist.addSong(Song("Song1", "Artist1", "Album1", 200));
    playlist.addSong(Song("Song2", "Artist2", "Album2", 180));
    playlist.addSong(Song("Song3", "Artist3", "Album3", 220));

    cout << "Original playlist:\n";
    playlist.printAll();

    // Shuffle songs
    playlist.shuffle();
    cout << "\nAfter shuffle:\n";
    playlist.printAll();

    // Save to file
    playlist.saveToFile("songs.txt");

    // Clear and load again
    playlist.loadFromFile("songs.txt");
    cout << "\nAfter reloading from file:\n";
    playlist.printAll();

    return 0;
}