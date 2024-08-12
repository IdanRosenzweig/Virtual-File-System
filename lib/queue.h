#ifndef QUEUE_H
#define QUEUE_H

template<typename TYPE>
struct queue {
private:
    struct item {
        TYPE val;
        struct item *next = nullptr;
        struct item *prev = nullptr;
    };

    struct item *q_front = nullptr;
    struct item *q_back = nullptr;
    int sz = 0;

public:
    void push_back(const TYPE &val) noexcept {
        struct item *alloc = new item{val, nullptr};

        if (sz == 0) q_front = q_back = alloc;
        else {
            q_back->prev = alloc;
            alloc->next = q_back;

            q_back = alloc;
        }

        ++sz;
    }

    void push_front(const TYPE &val) noexcept {
        struct item *alloc = new item{val, nullptr};

        if (sz == 0) q_front = q_back = alloc;
        else {
            q_front->next = alloc;
            alloc->prev = q_front;

            q_front = alloc;
        }

        ++sz;
    }

    void pop_front() noexcept {
        if (sz == 0) return;
        else if (sz == 1) {
            delete q_front;
            q_front = q_back = nullptr;
        } else {
            q_front = q_front->prev;

            delete q_front->next;
            q_front->next = nullptr;
        }

        --sz;
    }

    void pop_back() noexcept {
        if (sz == 0) return;
        else if (sz == 1) {
            delete q_back;
            q_front = q_back = nullptr;
        } else {
            q_back = q_back->next;

            delete q_back->prev;
            q_back->prev = nullptr;
        }

        --sz;
    }

    TYPE &front() noexcept {
        return q_front->val;
    }

    TYPE &back() noexcept {
        return q_back->val;
    }

    int size() const noexcept { return sz; }

    bool empty() const noexcept { return sz == 0; }
};

#endif //QUEUE_H
