#ifndef FIR_H_
#define FIR_H_


template <typename FloatType, int num_coeffs, const FloatType* coeffs>
class FirFilter {
public:

  FirFilter():
      current_index_(0) {
    for(int i = 0; i < num_coeffs; ++i)
      history_[i] = 0.0;
  }

  void put(FloatType value) {
    history_[current_index_++] = value;
    if(current_index_ == num_coeffs)
      current_index_ = 0;
  }

  FloatType get() {
    FloatType output = 0.0;
    int index = current_index_;
    for(int i = 0; i < num_coeffs; ++i) {
      if(index != 0) {
        --index;
      } else {
        index = num_coeffs - 1;
      }
      output += history_[index] * coeffs[i];
    }
    return output;
  }

private:
	FloatType history_[num_coeffs];
	int current_index_;
};

#define SAMPLEFILTER_TAP_NUM 16
float filter_taps[SAMPLEFILTER_TAP_NUM] = {
  -0.02269202836060212,
  -0.02127378086787707,
  -0.013154789546574128,
  0.01444687008458383,
  0.06109427152473655,
  0.11794575338076808,
  0.16984744046119396,
  0.2008691392692861,
  0.2008691392692861,
  0.16984744046119396,
  0.11794575338076808,
  0.06109427152473655,
  0.014446870084583827,
  -0.013154789546574128,
  -0.02127378086787707,
  -0.02269202836060212
};


static FirFilter<float, SAMPLEFILTER_TAP_NUM, filter_taps> FIR;


#endif // FIR_H_
