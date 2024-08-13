#ifndef RANDOM_STREAM_DEV_H
#define RANDOM_STREAM_DEV_H

#include "src/comp/content/stream_dev/stream_dev.h"

class random_stream_dev : public stream_dev {
public:
    void read(void *buff, int cnt) override;
};



#endif //RANDOM_STREAM_DEV_H
