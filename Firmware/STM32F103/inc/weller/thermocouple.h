class Thermocouple {
	float gain;
	float intercept;
public:
	Thermocouple() {
		intercept = 0;
		gain = 0.165095;
	}
	float getTemp(uint16_t adc_reading) {
		float res = intercept + gain * adc_reading;
		return res;
	}
	void set_params(float gain_, float intercept_) {
		gain = gain_;
		intercept = intercept_;
	}
};
