#ifndef INPUT_H
#define INPUT_H

#include<iosfwd>
#include<set>

#define INPUT_TYPES \
	X(JOYSTICK) \
	X(DIGITAL_IO) \
	X(ENCODER) \
	X(ANALOG) \
	X(TALON_SRX) \
	X(DRIVER_STATION) \
	X(CURRENT)\
	X(PUMP)
//	X(CAN_JAGUAR) 

class Input{
	public:
	enum class Type{
		#define X(NAME) NAME,
		INPUT_TYPES
		#undef X
	};

	private:
	Type type_;
	unsigned index_;

	explicit Input(Type);
	explicit Input(Type,unsigned);

	public:
	Type type()const;
	unsigned index()const;

	static Input joystick(unsigned);
	static Input digital_io(unsigned);
	static Input encoder(unsigned);
	static Input analog(unsigned);
	static Input talon_srx(unsigned);
	//static Input can_jaguar(unsigned);
	static Input driver_station();
	static Input current(unsigned);
	static Input pump();
};

std::set<Input> inputs();
bool operator<(Input const&,Input const&);
std::ostream& operator<<(std::ostream&,Input const&);

#endif
