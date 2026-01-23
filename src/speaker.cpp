#include <speaker.h>

void speakerInit() {
    M5.Speaker.begin();
    M5.Mic.end();
    M5.Speaker.setVolume(180);
}

void speakerSetVolume(uint8_t volume) {
    M5.Speaker.setVolume(volume);
    M5.Speaker.tone(1000 + volume, 20);
} 

void speakerRingWebAccess() {
    // Kind of zelda chest loot
    M5.Speaker.tone(880, 200);
    delay(200);
    M5.Speaker.tone(933, 200); 
    delay(200);
    M5.Speaker.tone(988, 200);
    delay(200);
    M5.Speaker.tone(1047, 1300); 

}

void speakerRingOpen() {
    M5.Speaker.tone(1400, 30); 
    delay(30);
    M5.Speaker.tone(1200, 30); 
}

void speakerRingVulnerable() {
    M5.Speaker.tone(1024, 30); 
}

