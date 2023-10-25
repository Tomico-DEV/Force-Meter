/*******************************************
 * 
 *  Four
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*******************************************/


#include "button.cpp"
#include "forceMeter.hpp"

template fourEyed::Button<fourEyed::ForceMeterFn, fourEyed::ForceMeter>::Button (uint8_t buttonPin, bool prevState);
template void fourEyed::Button<fourEyed::ForceMeterFn, fourEyed::ForceMeter>::setup (bool prevState);
template void fourEyed::Button<fourEyed::ForceMeterFn, fourEyed::ForceMeter>::loop ();
