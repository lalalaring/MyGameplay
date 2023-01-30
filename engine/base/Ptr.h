#ifndef PTR_H_
#define PTR_H_

namespace gameplay
{
/**
    * Intrusive smart pointer. the T must be extend from Ref
    */
template<typename T>
class Ptr {
    T* pointer;
public:
    Ptr() : pointer(nullptr) {
    }

    explicit Ptr(T* pointer) : pointer(pointer) {
        //pointer->addRef();
    }

    Ptr(const Ptr& other) : pointer(other.pointer) {
        if (other.pointer) {
            other.pointer->addRef();
        }
    }

    virtual ~Ptr() {
        clear();
    }

    Ptr& operator=(T* other) {
        if (other) {
            other->addRef();
        }
        if (pointer) {
            pointer->release();
        }
        pointer = other;
        return *this;
    }

    Ptr& operator=(const Ptr& other) {
        if (other.pointer) {
            other.pointer->addRef();
        }
        if (pointer) {
            pointer->release();
        }
        pointer = other.pointer;
        return *this;
    }

    T* operator->() { return pointer; }

    T& operator*() { return *pointer; }

    T* get() { return pointer; }

    void _set(T* p) { pointer = p; }

    bool isNull() { return pointer == nullptr; }

    void clear() {
        if (pointer) {
            pointer->release();
            pointer = nullptr;
        }
    }
};
}
#endif
