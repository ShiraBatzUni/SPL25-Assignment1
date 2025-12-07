#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for (auto info : library_tracks) {
        AudioTrack* new_track;
        if (info.type == "MP3") {
            new_track = new MP3Track(info.title, info.artists, info.duration_seconds, info.bpm, info.extra_param1, info.extra_param2);
        }
        else {
            new_track = new WAVTrack(info.title, info.artists, info.duration_seconds, info.bpm, info.extra_param1, info.extra_param2);
        }
        library.push_back(std::move(new_track));
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n" ;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title); // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    
    Playlist* new_playlist = new Playlist(playlist_name);
    for (int index : track_indices) {
        if (index < 1 || index > (int)(library.size())) {
            std::cout << "[WARNING] Invalid track index: " << index << "\n";
            continue;
        }
        int actual_index = index - 1;
        AudioTrack* track_to_add = library[actual_index];
        PointerWrapper<AudioTrack> cloned = track_to_add->clone();
        if (!cloned.get()) {
            std::cerr << "[ERROR] Track: " << track_to_add->get_title() << " failed to clone\n";
            continue;
        }
        cloned->load();
        cloned->analyze_beatgrid();
        new_playlist->add_track(cloned.release());
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << new_playlist->get_track_count() << " tracks)\n";

    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles_vector;
    std::vector<AudioTrack*> playlist_tracks = playlist.getTracks();
    for (AudioTrack* track : playlist_tracks) {
        titles_vector.push_back(track->get_title());
    }
    return std::vector<std::string>(); // Placeholder
}
