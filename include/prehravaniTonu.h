#ifndef PREHRAVANI_TONU_H_
#define PREHRAVANI_TONU_H_

void PrehravaniTonuMain(void);

int PaCallbackPrehravani(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

#endif