namespace HAL {
bool timeTick();

namespace Display {
void setBacklight(float percent);
void setContrast(float percent);
void write(char array[2][16]);
};

namespace Tip {
void setHeating(float percent);
uint16_t readRaw();
bool inStand();
};

namespace Com {
void write(const char * data);
void setCallback(void callback(const char * data));
};

namespace Encoder {
int getCount();
void reset();
setButtonCallback(void callback());
};

namespace Memory {
void store(size_t addr, libs::array_view<const uint8_t> data);
void get(size_t addr, libs::array_view<uint8_t> data);
};

};
