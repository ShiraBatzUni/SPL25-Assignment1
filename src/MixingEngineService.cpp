#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks{nullptr, nullptr},
      active_deck(0),
      auto_sync(false),
      bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
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

    size_t target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: "<< target_deck <<"\n";
    if (decks[target_deck]) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    int bpm_difference = decks[active_deck]->get_bpm() - track_clone->get_bpm();

    if (bpm_difference < 0) {
        bpm_difference = - bpm_difference;
    }

    if(auto_sync && bpm_difference > bpm_tolerance){
        sync_bpm(track_clone);
    }

    decks[target_deck] = track_clone.release();
    std::cout << "[Load Complete] " << track.get_title() << " is now loaded on deck " << target_deck << "\n"; 
    std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() <<")\n";
    delete decks[active_deck];
    decks[active_deck] = nullptr;
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << "\n";
     
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
    if (!decks[active_deck] || !track) {
        return false; 
    }

    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_track_bpm = track->get_bpm();

    int bpm_difference = active_deck_bpm - new_track_bpm;

    if (bpm_difference < 0) {
        bpm_difference = -bpm_difference;
    }

    return (bpm_difference <= bpm_tolerance);
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (!decks[active_deck] || !track) {
        return; 
    }
    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_track_bpm = track->get_bpm();
    int average_bpm = (active_deck_bpm + new_track_bpm) / 2;
    track->set_bpm(average_bpm);
    std::cout << "[Sync BPM] Syncing BPM from " << decks[active_deck]->get_title() << " to " << track->get_title() << "\n";
}
