#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <atomic>
using namespace std;
using namespace std::chrono;
#define USE_ATOMIC false


unsigned long long int add_ullong_standard(unsigned long long int a, unsigned long long int b){
  return a+b;
}

// traditional "by-hand" binary addition calculation
unsigned long long int add_ullong_slow(unsigned long long int a, unsigned long long int b){
  unsigned long long int ans = 0;
  unsigned long long int carry = 0;
  unsigned long long int a_xor_b = 0;
  int count = 0;
  unsigned long long int i = 0x1;
  while(true){
    unsigned long long int a_i = a&i;
    unsigned long long int b_i = b&i;
    unsigned long long int c_i = carry&i;
    ans |= ((a_i & b_i & c_i) | (~a_i & ~b_i & c_i) | (~a_i & b_i & ~c_i) | (a_i & ~b_i & ~c_i));
    carry |= ((((a_i)&(b_i)) | ((a_i)&(c_i)) | ((b_i)&(c_i))) << 1);
    count++;
    if(count == 64) break;
    i<<=1;
  }
  return ans;
}

// TODO: Implementation
unsigned long long int add_ullong_quick(unsigned long long int a, unsigned long long int b){
  unsigned long long int ans = 0;
  unsigned long long int carry = 0;

  return ans;
}

// Found on https://stackoverflow.com/questions/14893264/how-to-check-conversion-from-c-string-to-unsigned-int
bool isLegallong( string const& input ){
    char * end;
    errno = 0;
    unsigned long long int v = strtoul( input.c_str(), &end, 10 );
    return errno == 0 && *end == '\0' && end != input.c_str() && v <= ULLONG_MAX;
}


void valid_args(unsigned long long int argc, char *argv[]){
  if(argc == 2 && strcmp("help",argv[1]) == 0){
    cout << "----------------------" << endl;
    cout << "ENCM 517 Addition Test" << endl;
    cout << "----------------------" << endl;
    cout << "Usage: ./add.out \'a\' \'b\'" << endl;
    cout << "\'a\' and \'b\' are the positive unsigned long long integers to be added" << endl;
    cout << "The max value for \'a\' + \'b\' = " << ULLONG_MAX << "\n" << endl;
    exit(1);
  }

  if(argc != 3){
    cerr << "Error: incorrect usage" << endl;
    cerr << "Please use: \"./add.out a b\" where a and b are the positive unsigned long long integers to be added" << endl;
    exit(1);
  }

  if(!(isLegallong(argv[1]) & isLegallong(argv[2]))){
    cerr << "Error: integer arguments not given" << endl;
    cerr << "please ensure your arguments are positive integers" << endl;
    exit(1);
  }
}


int main(int argc, char *argv[]){
  valid_args(argc, argv);
  cout << "Starting Test " << argv[1] << "+" << argv[2] << ":" << endl;

  // convert
  #if USE_ATOMIC
  atomic<unsigned long long int> a;
  atomic_init(&a,stoull(argv[1]));
  atomic<unsigned long long int> b;
  atomic_init(&b,stoull(argv[2]));
  #else
  unsigned long long int a = stoull(argv[1]);
  unsigned long long int b = stoull(argv[2]);
  #endif


  // run test for standard
  #if USE_ATOMIC
  atomic<unsigned long long int> ans;
  #else
  unsigned long long int ans;
  #endif

  auto start = high_resolution_clock::now();
  #if USE_ATOMIC
  atomic_init(&ans, add_ullong_standard(a, b));
  #else
  ans = add_ullong_standard(a, b);
  #endif
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(stop - start);

  #if USE_ATOMIC
  printf("%llu + %llu = %llu\n",atomic_load(&a),atomic_load(&b), atomic_load(&ans));
  #else
  printf("%llu + %llu = %llu\n", a, b, ans);
  #endif
  cout << "The standard lasted " << duration.count() << "ns" << endl;

  // run test for slow algorithm
  // run test for standard

  start = high_resolution_clock::now();
  #if USE_ATOMIC
  atomic_init(&ans, add_ullong_slow(a, b));
  #else
  ans = add_ullong_slow(a, b);
  #endif
  stop = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(stop - start);

  #if USE_ATOMIC
  printf("%llu + %llu = %llu\n",atomic_load(&a),atomic_load(&b), atomic_load(&ans));
  #else
  printf("%llu + %llu = %llu\n", a, b, ans);
  #endif
  cout << "The slow lasted " << duration.count() << "ns" << endl;




  // TODO: run test for optimized algorithm



}
