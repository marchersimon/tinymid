#pragma once

#include <exception>

class VLVException : public std::exception {
    private:
        int pos; 
    public:
        VLVException(int pos) {
            this->pos = pos;
        } 
        
        int getPos() {
            return pos;
        }
};