#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    const std::string& title = track.get_title();
    if (cache.contains(title)) {
        cache.get(title);
        return 1;  
    }
      PointerWrapper<AudioTrack> clone = track.clone();
     if (!clone.get()) {
        std::cerr << "[ERROR] Track: \"" << title << "\" failed to clone\n";
        return -2;
    }
    clone.get()->load();
    clone.get()->analyze_beatgrid();
    bool evicted = cache.put(std::move(clone));
    if (evicted) {
        return -1;   
    }
    return 0;       
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    AudioTrack* track = cache.get(track_title);
    return track; // Placeholder
}
