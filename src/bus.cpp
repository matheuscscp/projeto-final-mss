#include "bus.h"

void simple_bus::end_of_elaboration(void)
{
    printf("[DEBUG] BUS - Verificando mapeamento da memoria \n");

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

   printf("[DEBUG] BUS - Pulso de descida \n");
 
    if (!m_current_request){
        m_current_request = m_requests[0];
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

    m_current_request->status = BUS_WAIT;
    readwrite_if *slave = get_slave(m_current_request->address);

    if (!slave) {
        m_current_request->status = BUS_ERROR;
        m_current_request = (simple_bus_request *)0;
        m_current_request->transfer_done.notify();
        return;
    }

    if (m_current_request->do_write == WRITE)
        slave->write(m_current_request->address, m_current_request->qteBytes, m_current_request->data);
    else
        slave->read(m_current_request->address, m_current_request->qteBytes, m_current_request->data);

    m_current_request->transfer_done.notify();
    m_current_request->status = BUS_OK;

    //Limpa request
    m_current_request = (simple_bus_request *)0;

}

simple_bus_request * simple_bus::get_request(unsigned int priority)
{
    simple_bus_request *request = (simple_bus_request *)0;
    for (unsigned int i = 0; i < m_requests.size(); ++i)
    {
        request = m_requests[i];
        if (request && (request->priority == priority))
            return request;
    }
    request = new simple_bus_request;
    request->priority = priority;
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

bus_status simple_bus::get_status(unsigned int unique_priority)
{
    return get_request(unique_priority)->status;
}

//----------------------------------------------------------------------------
//-- Read / Write master
//----------------------------------------------------------------------------

void simple_bus::read(unsigned int unique_priority, uint32_t src, uint32_t bytes, void* dst)
{

    simple_bus_request *request = get_request(unique_priority);

    request->do_write           = READ;
    request->address            = src;
    request->qteBytes           = bytes;
    request->data               = dst;
    request->status             = BUS_REQUEST;

    //wait(request->transfer_done);
   // wait(clock->posedge_event());

}

void simple_bus::write(unsigned int unique_priority, uint32_t dst, uint32_t bytes, void* src)
{

   printf("[DEBUG] BUS - Chamou write do BUS \n");
    simple_bus_request *request = get_request(unique_priority);

    request->do_write           = WRITE;
    request->address            = uint32_t(src);
    request->qteBytes           = bytes;
    request->data               = src;
    request->status             = BUS_REQUEST;

   printf("[DEBUG] BUS - Terminou escrita BUS \n");
   // wait(request->transfer_done);
   // wait(clock->posedge_event());
}
