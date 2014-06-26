#include "bus.h"

void simple_bus::end_of_elaboration(void)
{
    // check por overlapping na area de memoria dos slaves
    bool no_overlap;
    for (int i = 1; i < slave_port.size(); ++i) {
        readwrite_if *slave1 = slave_port[i];
        for (int j = 0; j < i; ++j) {
            readwrite_if *slave2 = slave_port[j];
            no_overlap = ((slave1->start_address()+slave1->size()) < slave2->start_address() ) ||
                    ( slave1->start_address() > (slave2->start_address()+slave2->size()) );
            if ( !no_overlap ) {
                printf("------------------------------------------------------------\n");
                printf("Error: dois slaves utilizando mesmo mapeamento da memoria : \n");
                printf("slave %i : %0X..%0X\n",i,slave1->start_address(),(slave1->start_address()+slave1->size()));
                printf("slave %i : %0X..%0X\n",j,slave2->start_address(),(slave2->start_address()+slave2->size()));
                printf("------------------------------------------------------------\n");
                exit(0);
            }
        }
    }
}

//----------------------------------------------------------------------------
//-- process
//----------------------------------------------------------------------------

void simple_bus::main_action()
{

    if (!m_current_request){
        m_current_request = m_requests;
    }
    else{
        printf("Tempo: %g   Endereco: [%d]\n", sc_time_stamp().to_double(), m_current_request->address);
    }
    if (m_current_request)
        handle_request();

}

//----------------------------------------------------------------------------
//-- BUS methods:
//
//     handle_request()
//     get_request()
//----------------------------------------------------------------------------

void simple_bus::handle_request(void)
{
    readwrite_if *slave = get_slave(m_current_request->address);

    if (m_current_request->do_write == WRITE)
        slave->write(m_current_request->address, m_current_request->qteBytes, m_current_request->data);
    else
        slave->read(m_current_request->address, m_current_request->qteBytes, m_current_request->data);

    //    m_current_request->transfer_done.notify(); //sera necessario? e bloqueante?

    //Limpa request
    m_current_request = (simple_bus_request *)0;

}

simple_bus_request * simple_bus::get_request(void)
{
    simple_bus_request *request = (simple_bus_request *)0;
    for (unsigned int i = 0; i < m_requests.size(); ++i)
    {
        request = m_requests[i];
        if (request)
            return request;
    }
    request = new simple_bus_request;
    m_requests.push_back(request);
    return request;
}

readwrite_if *simple_bus::get_slave(uint32_t address)
{
    for (int i = 0; i < slave_port.size(); ++i)
    {
        readwrite_if *slave = slave_port[i];
        if ((slave->start_address() <= address) &&
                (address <= (slave->start_address()+slave->size())))
            return slave;
    }
    //Se nao tiver slave
    return (readwrite_if *)0;
}

//----------------------------------------------------------------------------
//-- Read / Write master
//----------------------------------------------------------------------------

uint32_t simple_bus::read(uint32_t src, uint32_t bytes, void* dst)
{

    simple_bus_request *request = get_request();

    request->do_write           = READ;
    request->address            = src;
    request->qteBytes           = bytes;
    request->data               = dst;

}

void simple_bus::write(uint32_t dst, uint32_t bytes, void* src)
{

    simple_bus_request *request = get_request();

    request->do_write           = WRITE;
    request->address            = src;
    request->qteBytes           = bytes;
    request->data               = src;

}
