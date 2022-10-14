#ifndef DEQUE_H
#define DEQUE_H
namespace DATA_STRUCTURE {
    template <typename T> class deque {
        //size of every individual array
        const int individualSize = 10;
        //total size of the deque
        int currSize = 0;
        //length of array of pointers
        int arrayLen = 0;
        //Array of pointers
        T** theDeque = nullptr;
        //Index of which array the front is in
        int frontArr = 0;
        //Index of which array the back is in
        int backArr = 0;
        //index of the front availiable spot in theDeque[frontArr]
        int front = 0;
        //index of the back availiable spot in theDeque[backArr]
        int back = 0;
        public:
        //default constructor
        deque(): arrayLen{2}, theDeque{new T* [2]}, frontArr{0}, backArr{1}, front{9}, back{0} {
            theDeque[0] = new T [individualSize];
            theDeque[1] = new T [individualSize];
        }

        //Copy constructor
        deque(const deque& other): 
            currSize {other.currSize}, arrayLen {other.arrayLen}, theDeque {new T* [arrayLen]}, frontArr {other.frontArr}, 
            backArr {other.backArr}, front {other.front}, back {other.back} {
                for (int i = 0; i < arrayLen; ++i) {
                    theDeque[i] = new T [individualSize];
                }
                for (int i = 0; i < currSize; ++i) {
                    at(i) = other.at(i);
                }
            }
        //Move constructor
        deque(deque&& other):
            currSize {other.currSize}, arrayLen {other.arrayLen}, theDeque {other.theDeque}, frontArr {other.frontArr}, 
            backArr {other.backArr}, front {other.front}, back {other.back} {
                other.theDeque = nullptr;
            }


        //helper swap
        void swap (deque& other) {
            using std::swap;
            swap(currSize, other.currSize);
            swap(arrayLen, other.arrayLen);
            swap(theDeque, other.theDeque);
            swap(frontArr, other.frontArr);
            swap(backArr, other.backArr);
            swap(front, other.front);
            swap(back, other.back);
        }

        
        //Copy and move assignment
        deque& operator=(deque other) {
            swap(other);
            return *this;
        }

        
        //destructor
        ~deque() {
            if(theDeque != nullptr){
                for(int i = 0; i < arrayLen; ++i) {
                    delete [] theDeque[i];
                }
                delete [] theDeque;
            }
        }

        T& operator[](int index) { return at(index); }

        const T& operator[](int index) const { return at(index); }

        T& at(int index) {
            index += 1;
            int targetArr = frontArr + index / 10;
            int target = front + index % 10;

            targetArr += target / 10;
            target %= 10;

            return theDeque[targetArr][target];
        }
        
        const T& at(int index) const {
            index += 1;
            int targetArr = frontArr + index / 10;
            int target = front + index % 10;

            targetArr += target / 10;
            target %= 10;

            return theDeque[targetArr][target];
        }

        //iterator implementation
        class iterator {
            deque<T>* currDeque;
            int index;
            iterator(deque<T>* currDeque, int index): currDeque{currDeque}, index{index} {}

            public:
            bool operator!=(const iterator& other) const { return index != other.index; }
            iterator &operator++(){
                index += 1;
                return *this;
            }
            T& operator*() { return (*currDeque).at(index); }
            friend class deque;
        };
        
        iterator begin() { return iterator{this, 0}; }
        iterator end() { return iterator{this, currSize}; }

        bool empty() { return currSize == 0; }
        int size() { return currSize; }

        //add the case where there needs to be deallocation of memory
        void push_front(T element) {
            theDeque[frontArr][front] = element;
            front -= 1;
            if (front < 0) {
                if (frontArr == 0) {
                    increaseCap();
                }
                frontArr -= 1;
                front = 9;
            }
            currSize += 1;
        }

        void pop_front() {
            front += 1;
            if(individualSize - 1 < front) {
                front = 0;
                frontArr += 1;
            }
            currSize -= 1;
        }

        void push_back(T element) {
            theDeque[backArr][back] = element;
            back += 1;
            if (back == individualSize) {
                if (backArr == arrayLen - 1) {
                    increaseCap();
                }
                backArr += 1;
                back = 0;
            }
            currSize += 1;
        }
        
        void pop_back() {
            back -= 1;
            if (back < 0) {
                back = individualSize - 1;
                backArr -= 1;
            }
            currSize -= 1;
        }

        private://private helper
        //increase the size
        void increaseCap() {    
            T** newDeque = new T* [arrayLen*2];
            for (int i = 0; i < arrayLen; ++i) {
                newDeque[i + arrayLen/2] = theDeque[i];
            }
            for (int i = 0; i < arrayLen/2; ++i) {
                newDeque[i] = new T [individualSize];
            }
            for (int i = arrayLen/2 + arrayLen; i < 2*arrayLen; ++i) {
                newDeque[i] = new T [individualSize];
            }
            delete [] theDeque;
            theDeque = newDeque;
            frontArr += arrayLen/2;
            backArr += arrayLen/2;
            arrayLen *= 2;
        }
    };
}
#endif
