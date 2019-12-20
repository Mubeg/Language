#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include "ionegin"

#ifndef __LOCATION__
#define __LOCATION__  __FILE__, __LINE__, __PRETTY_FUNCTION__
#endif

#define STACK_INIT(name, ...) stack_init((name), #name, ##__VA_ARGS__)

#define TESTING

#ifdef TESTING
#define NDEBUG
#endif
#include <assert.h>


typedef int Elem_t;
#define ELEM_PRINT "%i" 

typedef long long int Canary_t;
#define CANARY_PRINT "%lX"

typedef long long int hash_t;
#define HASH_PRINT "%li"

//#pragma pack(push, 8)
struct Stack_t {

	hash_t hash; // always comes first
	const char * name;
	Canary_t canary1;
	char * buff;
	Canary_t *canary_dynamic1;
	Elem_t *data;
	Canary_t *canary_dynamic2;
	size_t size;
	size_t maxsize;
	size_t false_poison;
	int errnum;

	Canary_t canary2;

};
//#pragma pack(pop)

#pragma pack(push, 1)
struct Torture_t {

	char exec1[1];
	Stack_t stk;
	char exec2[1];

};
#pragma pack(pop)

const int CHECK_ZERO_ = __LINE__;
const int CHECK_CANARY             = 1 <<  (__LINE__ - CHECK_ZERO_ - 1); //= 1;
const int CHECK_HASH               = 1 <<  1; //= 2;
const int CHECK_SIZE               = 1 <<  2; //= 4;
const int BUFF_NULLPTR             = 1 <<  3; //= 8;
const int NAME_NULLPTR             = 1 <<  4; //= 16;
const int CANARY_DYNAMIC1_NULLPTR  = 1 <<  5; //= 32;
const int CANARY_DYNAMIC2_NULLPTR  = 1 <<  6; //= 64;
const int DATA_NULLPTR             = 1 <<  7; //= 128;
const int NEG_SIZE_MAXSIZE         = 1 <<  8; //= 256;
const int CHECK_DYN_CAN_AND_DATA   = 1 <<  9; //= 512;
const int NEG_SIZE_FALSE_POISON    = 1 << 10; //= 1024;
const int CHECK_NUM_POISON         = 1 << 11; //= 2048;

const int MAX_ERROR_NO = 2048;


const bool DEBUG = 0;
const Canary_t CANARY_VALUE = 0xBEDAAAAAAAAAAAAA;
const Elem_t POISON = 'NIKITA';
const int STACK_INIT_SIZE = 5;
const int NO_CHECK_FUNCTION_FULLCHECK = -1;
const int START_FUNCTION = 1;
const char LOG_FILE[] = "log.txt";
const int END_FUNCTION = 0;


bool test_stack(void);


int get_test(str command_line);


bool stack_init(Stack_t *stk, const char name[], const size_t init_stack_size = STACK_INIT_SIZE);


bool stack_push(Stack_t *stk, Elem_t elem);


Elem_t stack_pop(Stack_t *stk, int *err = nullptr);


bool stack_deinit(Stack_t *stk);


bool stack_resize_if_needed(Stack_t *stk, bool is_increasing = false);


void set_dynamic_canaries_and_data(Stack_t * stk);


void * recalloc_safe(void * const data, const long long int prev_num, const long long int num, const size_t size);


void zero_data(void * const data, const int start, const int end);


bool check_stack(Stack_t *stk, const char called_from_file[], int line, const char func[], 
		 int cur_state_func = NO_CHECK_FUNCTION_FULLCHECK, bool print_all = false);


void stack_dump(Stack_t * const stk, const char called_from_file[], int line, const char func[], bool ok);


bool check_maxsize(Stack_t * const stk);


bool check_number_of_poison(Stack_t * const stk);


bool check_nullptr(const void * const stk);


bool check_canary(Stack_t * const stk);


bool check_size_not_neg(size_t const stk);


bool check_hash_All(Stack_t * const stk);


bool check_hashable(Stack_t * const stk);


bool check_dynamic_canaries_and_data(Stack_t * const stk);


bool fill_data_with_poison(Elem_t * const data, const size_t size);


bool is_equal_hash_t(const hash_t left, const hash_t right);


hash_t find_sum_hash(Stack_t * const stk, size_t size);


hash_t hash(const void * const stk, size_t size);


bool check_size(Stack_t * const stk);


bool test_stack(void){

	Torture_t torture = {};

	Buff_elem_t * buff = nullptr;
	int buff_size = 0;
	buff = read_file_to_created_buff("torture.txt", &buff_size);

	if(buff == nullptr){
		fprintf(stderr, "Not able to create buff in %s line %d in %s\n", __LOCATION__);
		return false;
	}

	int commands_size = 0;
	str_ptr commands = make_text_must_free(buff, buff_size, &commands_size);

	if(commands == nullptr){
		fprintf(stderr, "Not able to create commands in %s line %d in %s\n", __LOCATION__);
		return false;
	}

	for(int i = 0; i < (commands_size = 11); i++){

		int test_id = 0;
		bool passed = false;	

		test_id = get_test(commands[i]);
		test_id = i;
		switch (test_id) {
			case 0:
				STACK_INIT(&torture.stk);
				passed = !check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 1:
				STACK_INIT(&torture.stk);
				torture.exec1[5] = '\0';
				torture.stk.size = -1;
				torture.stk.maxsize = -200;
				torture.stk.false_poison = -100000;
				torture.stk.buff = nullptr;
				torture.stk.name = nullptr;
				torture.stk.data = nullptr;
				torture.stk.canary_dynamic1 = nullptr;
				torture.stk.canary_dynamic2 = nullptr;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 2:
				STACK_INIT(&torture.stk);
				torture.stk.size = -1;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 3:
				STACK_INIT(&torture.stk);
				torture.stk.maxsize = -200;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 4:
				STACK_INIT(&torture.stk);
				torture.stk.false_poison = -100000;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 5:
				STACK_INIT(&torture.stk);
				torture.stk.buff = nullptr;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 6:
				STACK_INIT(&torture.stk);
				torture.stk.name = nullptr;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 7:
				STACK_INIT(&torture.stk);
				torture.stk.data = nullptr;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 8:
				STACK_INIT(&torture.stk);
				torture.stk.canary_dynamic1 = nullptr;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 9:
				STACK_INIT(&torture.stk);
				torture.stk.canary_dynamic2 = nullptr;
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);
				break;
			case 10:
				STACK_INIT(&torture.stk);
				zero_data(torture.exec1, 0, 100);
				passed = check_stack(&torture.stk, __LOCATION__);
				stack_deinit(&torture.stk);	
				break;
			default:
				fprintf(stderr, "Unexpected return of get_test in %s line %d in %s\n", __LOCATION__);
				break;
		}

		if(passed){
			fprintf(stderr, "Test #%d passed. %s line %d in %s\n", test_id, __LOCATION__);
			return false;
		}
	}

	free(buff);
	free(commands);

	return true;
}


int get_test(str command_line){

	return 1;
}


bool stack_init(Stack_t *stk, const char name[], const size_t init_stack_size /*= STACK_INIT_SIZE*/){

	assert(check_nullptr((void *) stk));
	if(init_stack_size < 0){
		return false;	
	}

	stk->canary1 = CANARY_VALUE;
	
	stk->maxsize = init_stack_size;

	stk->buff = (char *) calloc(stk->maxsize*sizeof(Elem_t) + 2*sizeof(Canary_t), sizeof(char));
	if(stk->buff == nullptr){
		return false;
	}
	
	set_dynamic_canaries_and_data(stk);
	fill_data_with_poison(stk->data, stk->maxsize);
	stk->false_poison = 0;

	stk->size = 0;
	stk->errnum = 0;

	stk->name = name[0] == '&' ? name + 1: name;

	stk->canary2 = CANARY_VALUE;

	stk->hash = find_sum_hash(stk, sizeof(Stack_t));

	assert(check_stack(stk, __LOCATION__));
 
	return true;
}

bool stack_push(Stack_t *stk, Elem_t elem){
	
	assert(check_stack(stk, __LOCATION__));
	
	if(!stack_resize_if_needed(stk, /*is_increasing = true*/ true)){
		assert(check_stack(stk, __LOCATION__));
		return false;	
	}

	if(elem == POISON){
		stk->false_poison++;
	}

	stk->data[stk->size++] = elem;	
	stk->hash = find_sum_hash(stk, sizeof(Stack_t));
	
	assert(check_stack(stk, __LOCATION__));

	return true;
}

Elem_t stack_pop(Stack_t *stk, int *err/* = nullptr(*/){

	assert(check_stack(stk, __LOCATION__));

	if(!stack_resize_if_needed(stk, /*is_increasing = false*/ false)){
		assert(check_stack(stk, __LOCATION__));
		return false;	
	}
	
	if(stk->size == 0){
		if(err != nullptr){
			*err = 1;
		}
		assert(check_stack(stk, __LOCATION__));
		return POISON;	
	}


	Elem_t temp = stk->data[--stk->size];
	if(temp == POISON){
		stk->false_poison--;
	}
	stk->data[stk->size] = POISON;
	stk->hash = find_sum_hash(stk, sizeof(Stack_t));

	assert(check_stack(stk, __LOCATION__));

	return temp;
}

bool stack_deinit(Stack_t *stk){

	assert(check_stack(stk, __LOCATION__));
	
	if(stk->buff == nullptr) return false;	

	zero_data(stk->buff, 0, stk->maxsize*sizeof(Elem_t) + 2*sizeof(Canary_t));
	free(stk->buff);

	return true;
}

bool stack_resize_if_needed(Stack_t *stk, bool is_increasing/* = false*/){

	assert(check_stack(stk, __LOCATION__));
	
	if(is_increasing && stk->size == stk->maxsize){

		void * temp = recalloc_safe(stk->buff, stk->maxsize*sizeof(Elem_t) + sizeof(Canary_t), 
				       stk->maxsize*2*sizeof(Elem_t) + sizeof(Canary_t)*2, sizeof(char));
		if(temp == nullptr){
			return false;		
		}

		stk->buff = (char *)temp;
		stk->maxsize *= 2;
		set_dynamic_canaries_and_data(stk);
		fill_data_with_poison(stk->data + stk->size, stk->maxsize - stk->size);

		stk->hash = find_sum_hash(stk, sizeof(Stack_t));

	}
	if(!is_increasing && stk->size < stk->maxsize/4){

		void * temp = recalloc_safe(stk->buff, stk->maxsize*sizeof(Elem_t) + sizeof(Canary_t), 
				       stk->maxsize*sizeof(Elem_t)/2 + sizeof(Canary_t)*2, sizeof(char));
		if(temp == nullptr){
			fprintf(stderr, "realloc problem while making smaller ????\n");
			return false;
		}
		if(temp != stk->buff){
			fprintf(stderr, "WOW, realloc tryies to move data!\n!\n!\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
		
		stk->buff = (char *)temp;
		stk->maxsize /= 2;
		set_dynamic_canaries_and_data(stk);

		stk->hash = find_sum_hash(stk, sizeof(Stack_t));
	}
	

	assert(check_stack(stk, __LOCATION__));

	return true;
}

void set_dynamic_canaries_and_data(Stack_t * stk){

	if(stk == nullptr) return;

	stk->canary_dynamic1 = (Canary_t *) stk->buff;
	stk->canary_dynamic1[0] = CANARY_VALUE;

	stk->data = (Elem_t *)(stk->buff + sizeof(Canary_t));

	stk->canary_dynamic2 = (Canary_t *)(stk->buff + sizeof(Canary_t) + stk->maxsize*sizeof(Elem_t));
	stk->canary_dynamic2[0] = CANARY_VALUE;

}

bool fill_data_with_poison(Elem_t * const data, const size_t size){
	
	assert(data != nullptr);
	if(size < 0) return false;

	for(unsigned int i = 0; i < size; i++){
		data[i] = POISON;
	}

	return true;

}

void * recalloc_safe(void * const data, const long long int prev_num, const long long int num, const size_t size){
	
	assert(data != nullptr);
		
	if(data == nullptr) return nullptr;

	if(prev_num >= num){

		zero_data(data, num*size, prev_num*size);
		return realloc(data, num*size);
	}
	
	if(sizeof(char) != 1){
		assert('0xBEDA' != '0xBEDA');
	}
	
	char * new_data = (char *) calloc(num, size);
	if(new_data == nullptr){
		return nullptr;
	}

	for(int i = 0; i < prev_num*size; i++){
		new_data[i] = ((char *) data)[i];
	}

	zero_data(data, 0, prev_num*size);


	free(data);
	return (void *) new_data;
}

void zero_data(void * const data, const int start, const int end){

	assert(data != nullptr);
	assert(start < end);	

	for(int i = start; i < end; i++){
		((char *) data)[i] = '\0';
	}

}

bool check_stack(Stack_t *stk, const char called_from_file[], int line, const char func[], 
		 int cur_state_func/* = NO_CHECK_FUNCTION_FULLCHECK*/, bool print_all/* = true*/){

//Check for func checking correctly (from start to end)
	if(cur_state_func != NO_CHECK_FUNCTION_FULLCHECK){

		static int prev_state_func = END_FUNCTION;
		assert(prev_state_func != cur_state_func);
		prev_state_func = cur_state_func;

	}
//End check for func

	bool passed = true;
	bool hashable = true;

	if(!check_nullptr((void *) stk)){
		return false;
	}

	if(stk->errnum != 0){
		return false;	
	}

// A Lot of checks below
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_canary(stk)                     ? passed : (stk->errnum += CHECK_CANARY, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_hash_All(stk)		       ? passed : (stk->errnum += CHECK_HASH, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_size(stk)                       ? passed : (stk->errnum += CHECK_SIZE, false);
	if(DEBUG) printf("Passed: %d\n", passed);


	passed = check_nullptr(stk->buff)              ? passed : (stk->errnum += BUFF_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);
 
	passed = check_nullptr(stk->name)              ? passed : (stk->errnum += NAME_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_nullptr(stk->canary_dynamic1)   ? passed : (stk->errnum += CANARY_DYNAMIC1_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_nullptr(stk->canary_dynamic2)   ? passed : (stk->errnum += CANARY_DYNAMIC2_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_nullptr(stk->data)              ? passed : (stk->errnum += DATA_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);


	passed = check_maxsize(stk)	               ? passed : (stk->errnum += NEG_SIZE_MAXSIZE, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_dynamic_canaries_and_data(stk)  ? passed : (stk->errnum += CHECK_DYN_CAN_AND_DATA, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_size_not_neg(stk->false_poison) ? passed : (stk->errnum += NEG_SIZE_FALSE_POISON, false);
	if(DEBUG) printf("Passed: %d\n", passed);
	
	passed = check_number_of_poison(stk)	       ? passed : (stk->errnum += CHECK_NUM_POISON, false);
	if(DEBUG) printf("Passed: %d\n", passed);

/*	passed = check_canary(stk)? passed : (stk->errnum = 1, false);
	passed = check_canary(stk)? passed : (stk->errnum = 1, false);
	passed = check_canary(stk)? passed : (stk->errnum = 1, false);
	passed = check_canary(stk)? passed : (stk->errnum = 1, false);
	passed = check_canary(stk)? passed : (stk->errnum = 1, false);
	passed = check_canary(stk)? passed : (stk->errnum = 1, false);*/

	if(DEBUG) printf("\n");

// End of a Lot of checks
		
	if(print_all || !passed){
		stack_dump(stk, called_from_file, line, func, passed);
	}

	return passed;
}


void stack_dump(Stack_t * const stk, const char called_from_file[], int line, const char func[], bool ok){

	if(stk == nullptr) return;

	FILE *log_file = fopen(LOG_FILE, "a");
	if(log_file == nullptr){
		fprintf(stderr, "Not able to open %s in %s line %d in %s\n", LOG_FILE, __LOCATION__);
	}


	time_t cur_time = -1;
	time(&cur_time);

	fprintf(log_file, "\nTime and date: %s\n"
			  "Dumping stack from file %s, line %d, function %s [%s]\n"
			  "Stack [%p] %s {\n"
			  "\n\t\t hash    = " HASH_PRINT "\n"
			  "\n\t\t canary1 = " CANARY_PRINT "\n"
			  "\t\t size    = %d\n"
			  "\t\t maxsize = %d\n"
			  "\n\t\t buff = [%p]\n"
			  "\t\t canary_dynamic1 [%p] = " CANARY_PRINT "\n"
			  "\t\t data = [%p]{\n", 
		ctime(&cur_time),
		called_from_file, line, func, ok ? "ok" : "ERROR!!!",
		stk, stk->name,
		stk->hash,
		stk->canary1,
		stk->size, 
		stk->maxsize,
		stk->buff, 
		stk->canary_dynamic1, stk->canary_dynamic1 == nullptr ? 'NULLPTR' : stk->canary_dynamic1[0],
		stk->data);
	fflush(log_file);
	
	if(stk->data != nullptr){
		if(check_maxsize(stk))
			for(int i = 0; i < stk->maxsize; i++){

				fprintf(log_file, "\t\t\t %c data[%d] = " ELEM_PRINT " %s\n", 
					i < stk->size ? '*' : ' ', i, stk->data[i], stk->data[i] == POISON ? "POISON?" : "");
			}
	}
	fflush(log_file);
	
	fprintf(log_file, "\t\t }\n"
			  "\t\t canary_dynamic2 [%p] = " CANARY_PRINT "\n",  
		stk->canary_dynamic2, stk->canary_dynamic2 == nullptr ? 'NULLPTR' : stk->canary_dynamic2[0]);

	fprintf(log_file, "\n\t\t errnum   = ");
	for(int i = 1; i <= MAX_ERROR_NO; i*=2){
		fprintf(log_file, "%d", (stk->errnum/i) % 2);
	}
	fprintf(log_file, "\n");
	fflush(log_file); 

	fprintf(log_file, "\n\t\t canary2 = " CANARY_PRINT "\n\n\t}\n\n",
		stk->canary2);

	fflush(log_file);
	/*

	Time and date: ....
	Dumping stack from file main.cpp, line 123, function check() [ok] ([ERROR!!!])
	Stack [0x1124asda] stk1 {
	
		Canary_t canary1 = ...	

		hash_t hash    = ....
		size_t size    = ....
		size_t maxsize = ....

		char * buff = [0x231141]
		Canary_t * canary_dynamic1 [0x1425123] = ...
		Elem_t *data = [0x23123]{
			* data[0] = ..
			  data[1] = ...  POISON?
			....
		}
		Canary_t * canary_dynamic2 [0x2131331] = ... 

		int errnum      = ...

		Canary_t canary2 = ...
	
	}

	*/

	fclose(log_file);

}

bool check_maxsize(Stack_t * const stk){
	
	if(stk == nullptr) return false;

	return stk->maxsize == ((Elem_t *)stk->canary_dynamic2) - stk->data; 

}

bool check_number_of_poison(Stack_t * const stk){

	if(!check_hashable(stk)) return false;

	size_t counter = 0;

	if(stk->data == nullptr){
		return false;	
	}
	
	for(size_t i = 0; i < stk->maxsize; i++){
		counter += stk->data[i] == POISON ? 1 : 0;
	}
	
	return counter - stk->false_poison == stk->maxsize - stk->size;
}

bool check_nullptr(const void * const stk){
	
	if(stk == nullptr){
		return false;
	}
	
	return true;
}

bool check_canary(Stack_t * const stk){

	if(stk == nullptr) return false;
	return stk->canary1 == stk->canary2 && stk->canary2 == CANARY_VALUE;
}

bool check_size_not_neg(const size_t size){

	return size >= 0;
}

bool check_hash_All(Stack_t * const stk){

	if(!check_hashable(stk)) return false;
	return is_equal_hash_t(find_sum_hash(stk, sizeof(Stack_t)), stk->hash);
}

bool check_hashable(Stack_t * const stk){
	
	return check_size(stk) && check_maxsize(stk) && check_size_not_neg(stk->false_poison);
}

bool check_dynamic_canaries_and_data(Stack_t * const stk){

	if(stk == nullptr) return false;
	if(stk->canary_dynamic1 == stk->canary_dynamic2) return false;
	if(stk->canary_dynamic1 == nullptr) return false;

	return !((char *)(stk->canary_dynamic2) - (char *)(stk->canary_dynamic1) != sizeof(Canary_t) + stk->maxsize*sizeof(Elem_t) 
		|| (char *)stk->data - (char *)stk->canary_dynamic1 != sizeof(Canary_t) 
		|| stk->canary_dynamic2[0] != stk->canary_dynamic1[0] 
		|| stk->canary_dynamic2[0] != CANARY_VALUE);

}

bool is_equal_hash_t(const hash_t left, const hash_t right){
	
	return left == right;
}

hash_t find_sum_hash(Stack_t * const stk, size_t size){

	if(stk == nullptr) return false;

	hash_t hash_sum = 0;
	
	hash_sum += hash(stk + sizeof(hash_t), size - sizeof(hash_t));
	hash_sum += hash(stk->buff, stk->buff == nullptr ? 0 : sizeof(stk->canary_dynamic1) + sizeof(stk->canary_dynamic2)
							       + sizeof(stk->data[0])*stk->maxsize);
	hash_sum += hash(stk->name, stk->name == nullptr ? 0 : strlen(stk->name));

	return hash_sum;

}

hash_t hash(const void * const stk, size_t size){

	if(stk == nullptr) return false;

	hash_t hash_sum = 0;

	for(size_t counter = 0; counter < size; counter++){

		hash_sum = (hash_sum << 2) + (counter+1)*((*((char *) stk + counter))^0xff);
	}

	return hash_sum;

}

bool check_size(Stack_t * const stk){
	
	if(stk == nullptr) return false;

	if(stk->size < 0 || stk-> size > stk->maxsize){
		return 0;
	}

	return true;
}













