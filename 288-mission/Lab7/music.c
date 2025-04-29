#include "open_interface.h"
#include "Timer.h"

#include <stdio.h>
#include <stdlib.h>

void playMusic() {
    unsigned char notes[]={69,69,69,69,31,69,69,69,69,72};
    unsigned char durations[]={24,24,24,24,32,12,12,12,12,32};

    oi_loadSong(0, 10, notes, durations);

    unsigned char notes2[]={72,72,72,72,72,72,74,72,69,69};
    unsigned char durations2[]={12,12,12,12,24,12,12,24,24,32};
    oi_loadSong(1, 10, notes2, durations2);

    oi_play_song(0);
    timer_waitMillis(3500);
    oi_play_song(1);
}
