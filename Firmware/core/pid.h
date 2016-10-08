#ifndef PID_H_
#define PID_H_

#include <math.h>

class PID {
 public:
    struct PIDParams {
        float Kp, Ki, Kd;
    };

    PID() {
        params = {0, 0, 0};
        mid = 0;
        this->reset();
    }

    void setTarget(float value) {
        this->mid = value;
    }

    float getTarget() {
		return this->mid;
	}

    float tick(float temp) {
        float pwr = this->feed_error(mid - temp);
        if( pwr < 0 )
        	pwr = 0;
        if( pwr > 100 )
        	pwr = 100;
        return pwr;
    }

    void reset() {
    	this->integral = 0;
    	this->prev_error = 0;
    }

    void set_params(const PIDParams& params) {
        this->params = params;
    }

private:
    float feed_error(float error) {
    	if( fabs(error) > 25 ) {
    		integral = 0;
    	} else {
    		integral += error;
    	}

    	if( error > 20 ) {
    		return 100;
    	}
    	if( error < -20 ) {
			return 0;
		}

        float Ipart = params.Ki*integral;

        float diff = error - prev_error;
        prev_error = error;
        return (params.Kp * error +  Ipart + params.Kd * diff);
    }

    PIDParams params;
    float prev_error;
    float integral;
    float mid;
};

#endif  // PID_H_
