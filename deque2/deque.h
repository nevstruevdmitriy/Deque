#pragma once
#include <iterator>

int max(const int x, const int y) {
    return x < y ? y : x;
}

template <class T>
class Deque {
private:
    const int MIN_SIZE_BUFFER;
    const double EXP_PUSH;
    const double EXP_POP;
    const double EXP;

    T* beginDeque;
    T* endDeque;
    T* buffer;
    int bufferSize;
    int sizeDeque;

    void errorPop() const {
        if (!sizeDeque) 
            throw 1;
    }
    void errorInd(int ind) const {
        if (ind >= sizeDeque)
            throw 1;
    }
    void get(int sizeNuw) {
        sizeNuw = max(sizeNuw, MIN_SIZE_BUFFER);
        T* newBuffer = new T[sizeNuw];
        T* indNewBuffer = newBuffer;
        T* ind = beginDeque;
        while (ind != endDeque) {
            *(indNewBuffer++) = *(ind++);
            ind = (ind == buffer + bufferSize ? buffer : ind);
        }
        delete[] buffer;
        bufferSize = sizeNuw;
        buffer = newBuffer;
        beginDeque = buffer;
        endDeque = indNewBuffer;
    }
    template <bool r, typename refer, typename point>
    class privateIterator : public std::iterator<std::random_access_iterator_tag, T> {
    private:
        int beginInd;
        int endInd;
        int iteratorInd;
        bool rev;
        point bufferIt;
        int bufferSizeIt;
        int posit() const {
            if (!rev) 
                return (beginInd <= iteratorInd) ? (iteratorInd - beginInd) : (iteratorInd + bufferSizeIt - beginInd);
            if ((iteratorInd + 1) % bufferSizeIt == beginInd) 
                return (beginInd <= endInd) ? (endInd - beginInd) : (bufferSizeIt - beginInd + endInd);
            return (iteratorInd <= endInd) ? (endInd - iteratorInd - 1) : (bufferSizeIt - iteratorInd + endInd - 1);
        }
    public:
        privateIterator():bufferIt(nullptr), bufferSizeIt(0) {
            iteratorInd = 0;
            rev = r;
        }
        privateIterator(const int ind, point bufferDeque, const int bufferSizeDeque, const int begin, const int end): bufferIt(bufferDeque), bufferSizeIt(bufferSizeDeque) {
            iteratorInd = ind;
            rev = r;
            beginInd = begin;
            endInd = end;
        }
        privateIterator(const privateIterator<r, refer, point> &iter):bufferIt(iter.bufferIt), bufferSizeIt(iter.bufferSizeIt) {
            iteratorInd = iter.iteratorInd;
            rev = iter.rev;
            beginInd = iter.beginInd;
            endInd = iter.endInd;
        }

        privateIterator<r, refer, point>& operator ++() {
            iteratorInd = (!rev ? (iteratorInd + 1) % bufferSizeIt : (iteratorInd - 1 + bufferSizeIt) % bufferSizeIt);
            return *this;
        }
        privateIterator<r, refer, point>& operator --() {
            rev ^= 1;
            operator ++();
            rev ^= 1;
            return *this;
        }
        privateIterator<r, refer, point>& operator ++(int) {
            return privateIterator((*this)++);
        }
        privateIterator<r, refer, point>& operator --(int) {
            return privateIterator((*this)--);
        }

        void operator =(const privateIterator &iter) {
            iteratorInd = iter.iteratorInd;
            rev = iter.rev;
            bufferIt = iter.bufferIt;
            bufferSizeIt = iter.bufferSizeIt;
            endInd = iter.endInd;
            beginInd = iter.beginInd;
        }

        bool operator ==(const privateIterator &iter) const {
            return iteratorInd == iter.iteratorInd && rev == iter.rev && bufferIt == iter.bufferIt && bufferSizeIt == iter.bufferSizeIt && beginInd == iter.beginInd && endInd == iter.endInd;
        }
        bool operator !=(const privateIterator &iter) const {
            return !((*this) == iter);
        }
        refer operator *() const {
            return *(bufferIt + iteratorInd);
        }
        point operator ->() {
            return (bufferIt + iteratorInd);
        }
        refer operator [](const int n){
            return (bufferSizeIt > n + iteratorInd) ? *((bufferIt + n) % bufferSize) : *(bufferIt + n - bufferSize);
        }
        privateIterator operator +(const int n) {
            return privateIterator((!rev) ? (iteratorInd + n) % bufferSizeIt : (iteratorInd - n + bufferSizeIt) % bufferSizeIt, bufferIt, bufferSizeIt, beginInd, endInd);
        }
        privateIterator operator -(const int n) {
            return privateIterator((!rev) ? (iteratorInd - n + bufferSizeIt) % bufferSizeIt : (iteratorInd + n) % bufferSizeIt, bufferIt, bufferSizeIt, beginInd, endInd);
        }
        int operator -(const privateIterator &a){   
            return posit() - a.posit();
        }
        void operator +=(const int n) {
            (*this) = (*this) + n;
        }
        void operator -=(const int n) {
            (*this) = (*this) - n;
        }
        bool operator <(const privateIterator<r, refer, point>& a) {
            return !rev ? posit() < a.posit() : posit() > a.posit();
        }
        bool operator >(const privateIterator<r, refer, point>& a) {
            return !((*this) < a);
        }
        bool operator <=(const privateIterator<r, refer, point>& a) {
            return (*this) < a || (*this) == a;
        }
        bool operator >=(const privateIterator<r, refer, point>& a) {
            return (*this) > a || (*this) == a;
        }
        void swap(privateIterator<r, refer, point>& a) {
            swap(bufferIt, a.bufferIt);
            swap(bufferSizeIt, a.bufferSizeIt);
            swap(iteratorInd, a.iteratorInd);
            swap(rev, a.rev);
            swap(beginInd, a.beginInd);
            swap(endInd, a.endInd);
        }
    };
public:
    typedef privateIterator<0, T&, T*> iterator;
    typedef privateIterator<0, const T&, const T*> const_iterator;
    typedef privateIterator<1, T&, T*> reverse_iterator;
    typedef privateIterator<1, const T&, const T*> const_reverse_iterator;
    
    ~Deque(){
        if (buffer != nullptr) 
            delete [] buffer;
    }
    Deque(const int minSize = 3, const double expensivePush = 2, const double expensivePop = 0.25, const double expensive = 2): MIN_SIZE_BUFFER(minSize), EXP_PUSH(expensivePush), EXP_POP(expensivePop), EXP(expensive) {
        buffer = new T[MIN_SIZE_BUFFER];
        beginDeque = buffer;
        endDeque = beginDeque;
        bufferSize = MIN_SIZE_BUFFER;
        sizeDeque = 0;
    }
    Deque(const Deque<T> &q): MIN_SIZE_BUFFER(q.MIN_SIZE_BUFFER), EXP_PUSH(q.EXP_PUSH), EXP_POP(q.EXP_POP), EXP(q.EXP) {
        bufferSize = q.bufferSize;
        sizeDeque = q.sizeDeque;
        buffer = new T[bufferSize];
        beginDeque = buffer;
        endDeque = beginDeque;
        for (int ind = 0; ind < q.size(); ++ind) {
            *(endDeque++) = q[ind];
        }
    }
    void push_back(const T& newT) {
        if (sizeDeque >= bufferSize - 1)
            get((int)((double)bufferSize * EXP));
        *endDeque = newT;
        ++endDeque;
        ++sizeDeque;

        if (endDeque == buffer + bufferSize) {
            endDeque = buffer;
        }
    }
    void pop_back() {
        errorPop();
        if ((double)sizeDeque <= (double)bufferSize * EXP_POP) {
            get((int)((double)bufferSize / EXP));
        }
        if (endDeque == buffer) {
            endDeque = buffer + bufferSize;
        }
        --sizeDeque;
        --endDeque;
    }
    void push_front(const T& newT) {
        if (sizeDeque >= bufferSize - 1) {
            get((int)((double)bufferSize * EXP));
        }
        if (beginDeque == buffer) {
            beginDeque = buffer + bufferSize;
        }
        --beginDeque;
        
        *beginDeque = newT;
        ++sizeDeque;
    }
    void pop_front() {
        errorPop();
        if ((double)sizeDeque <= (double)bufferSize * EXP_POP) {
            get((int)((double)bufferSize / EXP));
        }
        ++beginDeque;
        --sizeDeque;
        if (beginDeque == buffer + bufferSize) {
            beginDeque = buffer;
        }
    }

    T& front() {
        errorPop();
        return *beginDeque;
    }
    T& back() {
        errorPop();
        return endDeque == buffer ? *(buffer + bufferSize - 1) : *(endDeque - 1);
    }
    const T front() const {
        errorPop();
        return *beginDeque;
    }
    const T back() const {
        errorPop();
        return endDeque == buffer ? *(buffer + bufferSize - 1) : *(endDeque - 1);
    }

    T& operator[](const int ind)  {
        errorInd(ind);
        return (buffer + bufferSize > (int)ind + beginDeque) ? *(beginDeque + ind) : *(beginDeque + ind - bufferSize);
    }
    const T operator[](const int ind) const {
        errorInd(ind);
        return (buffer + bufferSize > (int)ind + beginDeque) ? *(beginDeque + ind) : *(beginDeque + ind - bufferSize);
    }

    int size() const{
        return sizeDeque;
    }
    bool empty() const{
        return sizeDeque == 0;
    }

    iterator begin()  {
        return iterator(beginDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    iterator end()  {
        return iterator(endDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const_iterator begin() const {
        return const_iterator(beginDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const_iterator end() const {
        return const_iterator(endDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }

    reverse_iterator rbegin() {
        return ++reverse_iterator(endDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    reverse_iterator rend() {
        return ++reverse_iterator(beginDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const reverse_iterator rbegin() const {
        return ++reverse_iterator(endDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const reverse_iterator rend() const {
        return ++reverse_iterator(beginDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }

    const_iterator cbegin() const {
        return const_iterator(beginDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const_iterator cend() const {
        return const_iterator(endDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const_reverse_iterator crbegin() const {
        return ++const_reverse_iterator(endDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
    const_reverse_iterator crend() const {
        return ++const_reverse_iterator(beginDeque - buffer, buffer, bufferSize, beginDeque - buffer, endDeque - buffer);
    }
};
