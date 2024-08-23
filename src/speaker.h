#ifndef SPEAKER_H
#define SPEAKER_H

#include <M5Cardputer.h>

void speakerInit();
void speakerSetVolume(uint8_t volume);
void speakerRingWebAccess();
void speakerRingOpen();
void speakerRingVulnerable();

#endif
