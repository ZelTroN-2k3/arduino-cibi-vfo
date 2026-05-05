#!/usr/bin/env python

'''
        /* definning a structure to store names of Cibi channels */
        typedef struct {
                uint16_t freq_khz;
                uint8_t channel;
                uint8_t band;
                bool bis;
        } cibi_channel_t;
'''

def print_channel( channel, band, freq_khz, bis):
    print('{.freq_khz = %d, .channel = %d, .band = %d, .bis = %s},' % (freq_khz, channel, band, bis))

bands = ['3Xinf', '2Xinf', '1Xinf', 'norm', '1Xsup', '2Xsup', '3Xsup']
for band_index, band_name in enumerate(bands):
    freq_khz = 25615 + band_index * 450
    for channel in xrange(40):
        channel = channel + 1
        print_channel(channel, band_index, freq_khz, 'false')
        freq_khz += 10
        if channel in [ 3, 7, 11, 15, 19 ]:
            print_channel(channel, band_index, freq_khz, 'true')
            freq_khz += 10



