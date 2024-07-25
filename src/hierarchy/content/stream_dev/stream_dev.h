#ifndef STREAM_DEV_H
#define STREAM_DEV_H

struct stream_dev {
    virtual void read(void* buff, int cnt) = 0;

    virtual ~stream_dev() {}
};

#endif //STREAM_DEV_H
