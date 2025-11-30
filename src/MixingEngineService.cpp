#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    decks(nullptr, nullptr);
    active_deck(0);
    std::cout << "[MixingEngineService] Initialized with 2 empty decks. \n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks.... \n";
    for (AudioTrack* deck : decks){
        delete deck;
        deck = nullptr;
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===\n";

    PointerWrapper<AudioTrack> track_clone = track.clone();
    
    if(!track_clone.get()) {
        std::cerr << "[ERROR] Track: " << track.get_title()  << " failed to clone\n";
        return -1;
    }

    track_clone->load();
    track_clone->analyze_beatgrid();

    if(!decks[0] && !decks[1]) {
        decks[0] = track_clone.release();
        return active_deck;
    }

    int bpm_difference = decks[active_deck]->get_bpm() - track_clone->get_bpm();

    if (bpm_difference < 0) {
        bpm_difference = - bpm_difference;
    }

    size_t target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: "<< target_deck <<"\n";
    if (decks[target_deck]) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    if(auto_sync && bpm_difference > bpm_tolerance){
        sync_bpm(track_clone);
    }

    decks[target_deck] = track_clone.release();
    std::cout << "[Load Complete] ’<title>’ is now loaded on deck "<< target_deck <<"\n"; 
    std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() <<")\n";
    delete decks[active_deck];
    decks[active_deck] = nullptr;
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck "<< target_deck <<"\n";
     
    return target_deck; // Placeholder
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
}
