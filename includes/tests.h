#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "utils.h"


void key_pair_to_file(const KeyPair *key_pair, const char *filename);
void test_somewhat_bit_encrypt(int n, int t);
void test_somewhat_bit_op(int n, int t);
void test_somewhat_int_encrypt(int n, int t, int p);

#endif