#ifndef RATE_H
#define RATE_H

struct Rate {
    int song_id;
    double score;

    Rate() : song_id(0), score(0.0) {}
    Rate(int song, double sc) : song_id(song), score(sc) {}
};

#endif
