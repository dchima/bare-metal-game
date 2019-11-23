/*
 * WM8731_AudioEngine.c
 *
 * Created on: Mar 20, 2018
 * Author: Azeem Oguntola
 * -------------------------------------------------------------------
 * Description
 * -------------------------------------------------------------------
 * This is an Audio Driver for the WM8731 Audio Codec.
 * It provides functions for changing the volume level,
 */

#include "WM8731_AudioEngine.h"
#include "../HPS_I2C/HPS_I2C.h"

//
// Driver global static variables (visible only to this .c file)
//

//Driver Base Address
volatile unsigned int *wm8731_base_ptr = 0x0;
//Driver Initialised
bool wm8731_initialised = false;

// Volume levels
unsigned int Audio_volumeMap [11] = {0x00, 0x37, 0x3F, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x77, 0x7F};
//
// Useful Defines
//

//WM8731 ARM Address Offsets
#define WM8731_CONTROL    (0x0/sizeof(unsigned int))
#define WM8731_FIFOSPACE  (0x4/sizeof(unsigned int))
#define WM8731_LEFTFIFO   (0x8/sizeof(unsigned int))
#define WM8731_RIGHTFIFO  (0xC/sizeof(unsigned int))

//I2C Register Address Offsets
#define WM8731_I2C_LEFTINCNTRL   (0x00/sizeof(unsigned short))
#define WM8731_I2C_RIGHTINCNTRL  (0x02/sizeof(unsigned short))
#define WM8731_I2C_LEFTOUTCNTRL  (0x04/sizeof(unsigned short))
#define WM8731_I2C_RIGHTOUTCNTRL (0x06/sizeof(unsigned short))
#define WM8731_I2C_ANLGPATHCNTRL (0x08/sizeof(unsigned short))
#define WM8731_I2C_DGTLPATHCNTRL (0x0A/sizeof(unsigned short))
#define WM8731_I2C_POWERCNTRL    (0x0C/sizeof(unsigned short))
#define WM8731_I2C_DATAFMTCNTRL  (0x0E/sizeof(unsigned short))
#define WM8731_I2C_SMPLINGCNTRL  (0x10/sizeof(unsigned short))
#define WM8731_I2C_ACTIVECNTRL   (0x12/sizeof(unsigned short))

//Initialise Audio Controller
signed int Audio_initialise (void) {
    // If not Initialised
    if (!WM8731_isInitialised()) {
        signed int status;
        //Set the local base address pointer
        wm8731_base_ptr = (unsigned int *) 0xFF203040;
        //Ensure I2C Controller "I2C1" is initialised
        if (!HPS_I2C_isInitialised(0)) {
            status = HPS_I2C_initialise(0);
            if (status != HPS_I2C_SUCCESS) return status;
        }
        //Initialise the WM8731 codec over I2C. See Page 46 of datasheet
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_POWERCNTRL   <<9) | 0x12); //Power-up chip. Leave mic off as not used.
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_LEFTINCNTRL  <<9) | 0x17); //+4.5dB Volume. Unmute.
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_RIGHTINCNTRL <<9) | 0x17); //+4.5dB Volume. Unmute.
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_LEFTOUTCNTRL <<9) | 0x5F); //-26dB Volume. Unmute.
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_RIGHTOUTCNTRL<<9) | 0x5F); //-26dB Volume. Unmute.
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_ANLGPATHCNTRL<<9) | 0x12); //Use Line In. Disable Bypass. Use DAC
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_DGTLPATHCNTRL<<9) | 0x06); //Enable High-Pass filter. 48kHz sample rate.
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_DATAFMTCNTRL <<9) | 0x4E); //I2S Mode, 24bit, Master Mode (do not change this!)
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_SMPLINGCNTRL <<9) | 0x00); //Normal Mode, 48Khz sample rate /
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_ACTIVECNTRL  <<9) | 0x01); //Enable Codec
        if (status != HPS_I2C_SUCCESS) return status;
        status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_POWERCNTRL   <<9) | 0x02); //Power-up output.
        if (status != HPS_I2C_SUCCESS) return status;
        //Mark as initialised so later functions know we are ready
        wm8731_initialised = true;
    }
    //Clear the audio FIFOs
    return Audio_clearFIFO(true,true);
}

//Check if driver initialised
bool WM8731_isInitialised() {
    return wm8731_initialised;
}

//Clears FIFOs
// - returns 0 if successful
signed int Audio_clearFIFO( bool adc, bool dac) {
    unsigned int cntrl;
    if (!WM8731_isInitialised()) return WM8731_ERRORNOINIT; //not initialised
    //Read in current control value
    cntrl = wm8731_base_ptr[WM8731_CONTROL];
    //Calculate new value - with corresponding bits for clearing adc and/or dac FIFOs
    if (adc) {
        cntrl |= (1<<2);
    }
    if (dac) {
        cntrl |= (1<<3);
    }
    //Assert reset flags
    wm8731_base_ptr[WM8731_CONTROL] = cntrl;
    //Clear the flags
    if (adc) {
        cntrl &= ~(1<<2);
    }
    if (dac) {
        cntrl &= ~(1<<3);
    }
    //Then clear reset flags
    wm8731_base_ptr[WM8731_CONTROL] = cntrl;
    //And done.
    return WM8731_SUCCESS; //success
}


// Check the Output FIFO space.
// Use this function to check if audio files can be written
// We can only write audio if this function returns a true
bool Audio_writeSpace (void) {
    // This register indicates the number of words available to be written.
    // must be greater than zero before we can write audio files
    volatile unsigned char* fifospace_ptr = (unsigned char*)&wm8731_base_ptr[WM8731_FIFOSPACE]; // Get the fifo space pointer    
    // Check if the left and right output spaces are free.
    // Return true if free. Otherwise, return false
    if ((fifospace_ptr[2] > 0) && (fifospace_ptr[3] > 0)) return true;
    else return false;    
}
/* We may not be needing this unless we are doing a karaoke
 * Leave this commented until more research has been done. I'm not sure and have no way to test this until i get a line in cable
 * and/or a microphone. 
// Check if the Input FIFO space.
// Use this function to check if audio files are available to be read
// We can only read audio if this function returns a true
bool Audio_readSpace (void) {
    volatile unsigned char* fifospace_ptr = (unsigned char*)&wm8731_base_ptr[WM8731_FIFOSPACE]; // Get the fifo space pointer    
    // Check if the left and right data are available.
    // Return true if free. Otherwise, return false
    if ((fifospace_ptr[0] > 0) && (fifospace_ptr[1] > 0)) return true;
    else return false;    

}
*/
// Function to write audio to the left channel
// Check Audio_writeSpace before calling this function
signed int Audio_writeToLeft (signed int audio_data) {
    volatile unsigned int* audio_left_ptr = &wm8731_base_ptr[WM8731_LEFTFIFO];    // Let Audio Channel pointer
    *audio_left_ptr = audio_data;
    return WM8731_SUCCESS; //success
}

// Function to write audio to the right channel
// Check Audio_writeSpace before calling this function
signed int Audio_writeToRight (signed int audio_data) {
    volatile unsigned int* audio_right_ptr = &wm8731_base_ptr[WM8731_RIGHTFIFO];    // Let Audio Channel pointer
    *audio_right_ptr = audio_data;    
    return WM8731_SUCCESS; //success
}

// Function to set output volume
// 0 = mute. Volume levels are 1 to 10
// Just call the function and pass the desired volume level
signed int Audio_outVolume (unsigned int level) {
    signed int status;
    if (!WM8731_isInitialised()) return WM8731_ERRORNOINIT; //not initialised

    if (level > 10) level = 10; // ensure it doesn't exceed the max
    // Initialise the I2C if not initialised
    // Initialise the I2C driver
    if (!HPS_I2C_isInitialised(0)) {
        status = HPS_I2C_initialise(0);
        if (status != HPS_I2C_SUCCESS) return status;
    }
    // Set the volume by writing the values to the volume register
    status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_LEFTOUTCNTRL <<9) | Audio_volumeMap[level]); // Set left volume
    if (status != HPS_I2C_SUCCESS) return status;
    status = HPS_I2C_write16b(0, 0x1A, (WM8731_I2C_RIGHTOUTCNTRL<<9) | Audio_volumeMap[level]); // Set right volume.
    if (status != HPS_I2C_SUCCESS) return status;
    return WM8731_SUCCESS; //success
}

