//Arquivo que consta o vetor de requisicoes, necessario caso haja mais de um master
#ifndef __simple_bus_types_h
#define __simple_bus_types_h

#include <stdio.h>
#include "systemc.h"

struct simple_bus_request;
typedef std::vector<simple_bus_request *> simple_bus_request_vec;


#endif
