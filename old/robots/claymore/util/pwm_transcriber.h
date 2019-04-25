#ifndef PWM_TRANSCRIBER_H
#define PWM_TRANSCRIBER_H

#include <map>
#include <set>
#include <iostream>
#include <cassert>
#include <cmath>

class PWM_transcriber{ //This class is used to manage conversion of a data set to unique PWM values for transmission
	private:
	class Transcriber_base{//This class is used to represent a transcriber of any type
		private:
		virtual int operator()(const void*)const = 0;
		
		public:
		virtual void print(std::ostream&)const = 0;
		virtual ~Transcriber_base(){}
	};
	
	public:
	template<typename T>
	class Transcriber: public Transcriber_base{//This class is used to define transcription for a specific subset of data
		private:
		std::map<T,int> transcriptions;
		
		int operator()(const void* key_ptr)const{//converts a value to an integer value that can later easily be mapped to a PWM value
			T* typed_key_ptr = (T*)key_ptr;
			
			T key = *typed_key_ptr;
			
			if(transcriptions.find(key) == transcriptions.end()){
				std::cout<<__FILE__<<":"<<__LINE__<<": Warning: no transcriber found with name \""<<key<<"\""<<std::endl;
			}
			
			return transcriptions.at(key);
		}
		
		public:
		int operator()(const T& key)const{//converts a value to an integer value that can later easily be mapped to a PWM value
			const T* key_ptr = &key;
		
			return (*this)(key_ptr);
		}

		void print(std::ostream& o)const{
			for(auto i = transcriptions.begin(); i != transcriptions.end(); i++){
				o<<"["<<i->first<<" => "<<i->second<<"]";
				
				auto second_to_last = transcriptions.end();
				second_to_last--;
				if(i != second_to_last){
					o<<" ";
				}
			}	
		}
			
		Transcriber(unsigned& highest_unassigned_value, std::set<T> keys){
			for(T k: keys){
				transcriptions.insert(std::pair<T,int>(k,highest_unassigned_value));
				highest_unassigned_value++;
			}
		}
	};
	
	private:
	unsigned highest_unassigned_value;//used to track values assigned to each value in a data subset and ensure that each is unique
	std::map<std::string, Transcriber_base*> all_transcriptions; //stores and manages all defined transcriptions

	public: 
	template<typename T> 
	int map(const std::string NAME, const T& KEY)const{//maps a value of a given subset to its assigned integer value
		if(all_transcriptions.find(NAME) == all_transcriptions.end()){
			std::cout<<__FILE__<<":"<<__LINE__<<": Warning: no transcriber found with name \""<<NAME<<"\""<<std::endl;
		}
		const Transcriber<T>* transcriber_ptr = (Transcriber<T>*)all_transcriptions.at(NAME);
		Transcriber<T> transcriber = *transcriber_ptr;
			
		return transcriber(KEY);	
	}
	
	template<typename T> 
	double transcribe(const std::string NAME, const T& KEY)const{//transcribes a given value of a specific subset to a PWM value
		int value = map(NAME, KEY);
		
		const double PWM_RANGE = 2.0;
		const double PWM_INCREMENT = PWM_RANGE / (double)highest_unassigned_value;
		const double STARTING_PWM_VALUE = -1.0;

	        return STARTING_PWM_VALUE + PWM_INCREMENT * value; //convert the integer transmission value to a pwm valu
	};

	template<typename T>
	void add(const std::string NAME,const std::set<T>& KEYS){//used to define a new subset of values given its name and a set of all possible values
		if(all_transcriptions.find(NAME) != all_transcriptions.end()){
			std::cout<<__FILE__<<":"<<__LINE__<<":"<<" Error: transcriber already set with name \""<<NAME<<"\""<<std::endl;
			exit(0);
		}
		
		all_transcriptions.insert(std::pair<std::string,Transcriber_base*>(NAME, (Transcriber_base*)(new Transcriber<T>(highest_unassigned_value,KEYS))));
	}

	friend std::ostream& operator<<(std::ostream& o, const PWM_transcriber& a) {
		o<<"{";
		for(auto i = a.all_transcriptions.begin(); i != a.all_transcriptions.end(); i++){
			o<<i -> first<<": ";
			auto t = i -> second;
			t -> print(o);
			
			auto second_to_last = a.all_transcriptions.end();
			second_to_last--;
			if(i != second_to_last){
				o<<",   ";
			}
		}
		o<<"}";
		return o;
	}

	PWM_transcriber():highest_unassigned_value(0){}
};

template<typename T>
std::ostream& operator<<(std::ostream& o, const PWM_transcriber::Transcriber<T>& a){
	a.print(o);
	return o;
}

#endif
