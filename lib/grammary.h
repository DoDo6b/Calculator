#pragma once
#include "token.h"
#include "namespace.h"
#include "variables.h"
#include <stdexcept>

double expression();

double calc_sqrt();

double calc_pow();

double calc_sin();

double calc_cos();

double handle_variable(Token& token);

double primary();

double secondary();

double term();

double expression();

double declaration();