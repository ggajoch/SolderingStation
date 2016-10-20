#ifndef SOLDER_LCDTRANSACTION_H
#define SOLDER_LCDTRANSACTION_H

#include <array>
#include "ArrayHolder.h"

class LCDTransaction {
public:
    LCDTransaction() {}

    template<size_t len>
    LCDTransaction(bool isCommand, ArrayHolder<uint8_t, len> array) :
            _isCommand(isCommand), array(array) {
        actual_position = 0;
    }

    uint8_t get() {
        return array[actual_position];
    }

    bool advance() {
        ++actual_position;
        if( actual_position == array.size() ) {
            return false;
        }
        return true;
    }

    bool isCommand() {
        return _isCommand;
    }

private:
    bool _isCommand;
    ArrayHolder<uint8_t, 16> array;
    uint8_t actual_position;
};
#endif //SOLDER_LCDTRANSACTION_H
