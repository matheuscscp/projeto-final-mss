//Arquivo com a estrutura da requisicao
//Essa estrutura sera preenchida no read / write do master, sendo que o bus ira encaminhas os dados para os slaves

#ifndef __simple_bus_request_h
#define __simple_bus_request_h

enum simple_bus_commands { READ = 0
                           , WRITE };

enum bus_status { BUS_OK = 0
                  , BUS_REQUEST
                  , BUS_WAIT
                  , BUS_ERROR };

struct simple_bus_request
{

    unsigned int priority;  //Prioridade do master
    bool do_write;          //Seleciona read/write
    uint32_t address;       //Endereco acessado
    uint32_t qteBytes;      //Quantidade em bytes a ser enviado
    void *data;             //Ponteiro onde sera lido/salvo a informacao do slave

    bus_status status;      //Status do barramento

    // request status
    sc_event transfer_done;

    // default constructor
    simple_bus_request();
};

inline simple_bus_request::simple_bus_request()
    : priority(0)
    , do_write(READ)
    , address(0)
    , qteBytes(4)
    , data((void *)0)
    , status (BUS_OK)
{}

#endif
