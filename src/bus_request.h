//Arquivo com a estrutura da requisicao
//Essa estrutura sera preenchida no read / write do master, sendo que o bus ira encaminhas os dados para os slaves

#ifndef __simple_bus_request_h
#define __simple_bus_request_h

enum simple_bus_commands { READ = 0
                           , WRITE };

struct simple_bus_request
{

    // request parameters
    bool do_write;
    uint32_t address;
    uint32_t qteBytes;
    void *data;

    // request status
    sc_event transfer_done;

    // default constructor
    simple_bus_request();
};

inline simple_bus_request::simple_bus_request()
    : do_write(READ)
    , address(0)
    , qteBytes(4)
    , data((void *)0)
{}

#endif
