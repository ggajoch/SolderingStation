#ifndef AVG_H_
#define AVG_H_

template<typename Type, int elements>
class AVG {
private:
	Type history[elements];
	int current_index;
public:

	AVG() : current_index(0) {
		for (int i = 0; i < elements; ++i)
			history[i] = 0.0;
	}

	void put(Type value) {
		history[current_index++] = value;
		if (current_index == elements)
			current_index = 0;
	}

	Type get() {
		Type output = 0.0;
		for (int i = 0; i < elements; ++i) {
			output += history[i];
		}
		output /= static_cast<float>(elements);
		return output;
	}
};

#endif // AVG_H_
