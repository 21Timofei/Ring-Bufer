#include <iostream>
#include <algorithm>
#include <vector>

template<typename T>
class RingBuffer {
private:
    T* buffer;
    int capacity, head, tail, size;
public:

    RingBuffer(int capacity) : capacity(capacity), head(0), tail(0), size(0)  {
        buffer = new T[capacity];
    }

    ~RingBuffer() {
        delete[] buffer;
    }

    const T& operator[](int index) const {
        return buffer[(head + index) % capacity];
    }

    class Iterator  {
    private:
        T* buffer;
        int index, capacity;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* buffer, int index, int capacity) : buffer(buffer), index(index), capacity(capacity) {}

        T& operator*() const {
            return buffer[index];
        }

        Iterator& operator++() {
            index = (index + 1) % capacity;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--() {
            index = (index - 1 + capacity) % capacity;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return index == other.index;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        Iterator& operator=(const Iterator& other) {
            if (this != &other) {
                buffer = other.buffer;
                capacity = other.capacity;
                index = other.index;
            }
            return *this;
        }

        Iterator& operator+=(difference_type n) {
            index = (index + n) % capacity;
            return *this;
        }

        Iterator operator+(difference_type n) const {
            Iterator tmp = *this;
            return tmp += n;
        }

        Iterator& operator-=(difference_type n) {
            index = (index - n + capacity) % capacity;
            return *this;
        }

        Iterator operator-(difference_type n) const {
            Iterator tmp = *this;
            return tmp -= n;
        }

        difference_type operator-(const Iterator& other) const {
            return (index - other.index + capacity) % capacity;
        }

        reference operator[](difference_type n) const {
            return buffer[(index + n) % capacity];
        }

        bool operator<(const Iterator& other) const {
            return (*this - other) < 0;
        }

        bool operator>(const Iterator& other) const {
            return (*this - other) > 0;
        }

        bool operator<=(const Iterator& other) const {
            return (*this - other) <= 0;
        }

        bool operator>=(const Iterator& other) const {
            return (*this - other) >= 0;
        }
    };

    void push_back(const T& value) {
        if (size == capacity) {
            head = (head + 1) % capacity;
        } else {
            size++;
        }
        buffer[tail] = value;
        tail = (tail + 1) % capacity;
    }

    void push_front(const T& value) {
        if (size == capacity) {
            tail = (tail - 1 + capacity) % capacity;
        } else {
            size++;
        }
        head = (head - 1 + capacity) % capacity;
        buffer[head] = value;
    }

    void pop_back() {
        if (size == 0)
            return;
        tail = (tail - 1 + capacity) % capacity;
        size--;
    }

    void pop_front() {
        if (size == 0) {
            return;
        }
        head = (head + 1) % capacity;
        size--;
    }

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

    void insert(Iterator index, T value) {
        if (size == capacity)
            return;

        Iterator it = end();
        for (auto i = it; i != index; --i)
            *i = *(i - 1);

        *index = value;
        size++;
    }

    T& front() {
        if (size == 0) {
            return;
        }
        return buffer[head];
    }

    T& back() {
        if (size == 0) {
            return;
        }
        return buffer[(tail - 1 + capacity) % capacity];
    }


    void erase(Iterator index) {
        if (size == 0)
            return;

        for (auto i = index; i != end(); ++i)
            *i = *(i + 1);

        size--;
    }

    Iterator begin() {
        return Iterator(buffer, head, capacity);
    }

    Iterator end() {
        return Iterator(buffer, tail, capacity);
    }

    int getSize(){
        return size;
    }

    void show() {
        Iterator it = begin();
        for (int i = 0; i < size; i++) {
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;
    }
};

int main() {
    RingBuffer<int> buffer(5);
    for(int i = 0; i < 10; i++){
        buffer.push_back(i);
    }
    //auto p = [](int a, int b){return a < b;};
    std::sort(buffer.begin(),buffer.end(),std::greater<int>());

    buffer.show();
}