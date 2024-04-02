#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

template<typename T>
class RingBuffer {
private:
    T* buffer;
    int capacity, head, tail, size;

    int index(int idx) const {
        int adjusted_idx = head + idx;
        if (adjusted_idx >= capacity) adjusted_idx -= capacity;
        return adjusted_idx;
    }

public:
    class iterator {
        friend class RingBuffer;
    private:
        RingBuffer* buf;
        int index;

    public:
        iterator(RingBuffer* buf, int index) : buf(buf), index(index) {}

        T& operator*() const {
            return buf -> buffer[buf -> index(index)];
        }

        iterator& operator++() {
            index++;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return index == other.index && buf == other.buf;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    RingBuffer(int cap) : capacity(cap), head(0), tail(0), size(0) {
        buffer = new T[capacity];
    }

    ~RingBuffer() {
        delete[] buffer;
    }

    void push_back(const T& value) {
        if (size == capacity)
            reallocate(capacity * 2);
        buffer[tail] = value;
        tail = (tail + 1) % capacity;
        if (size < capacity) size++;
    }

    void push_front(const T& value) {
        if (size == capacity)
            reallocate(capacity * 2);
        head = (head - 1 + capacity) % capacity;
        buffer[head] = value;
        if (size < capacity) size++;
    }

    // Переаллокация с сохранением структуры кольцевого буфера
    void reallocate(int new_capacity) {
        T* new_buffer = new T[new_capacity];
        for (int i = 0; i < size; ++i) {
            new_buffer[i] = (*this)[i];
        }
        delete[] buffer;
        buffer = new_buffer;
        capacity = new_capacity;
        head = 0;
        tail = size % capacity;
    }

    T& operator[](int idx) {
        return buffer[index(idx)];
    }

    const T& operator[](int idx) const {
        return buffer[index(idx)];
    }

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size);
    }

    int getSize() const {
        return size;
    }

};

int main() {
    RingBuffer<int> buf(5);
    std::vector<int> v;

    for(int i = 0; i < 6; ++i) {
        buf.push_front(i);
        v.push_back(i);
    }
    std::for_each(buf.begin(), buf.end(), [](int& value) {std::cout << value << ' ';});
    std::cout <<"\n";
    std::for_each(v.begin(), v.end(), [](int& value) {std::cout << value << ' ';});

    std::cout << std::endl;
    if(std::find(buf.begin(), buf.end(), 1) != buf.end()) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}
