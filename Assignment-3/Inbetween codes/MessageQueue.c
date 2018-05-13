struct data_item_type {
    u32_t field1;
    u32_t field2;
    u32_t field3;
};

char __aligned(4) my_msgq_buffer[10 * sizeof(data_item_type)];
struct k_msgq my_msgq;

k_msgq_init(&my_msgq, my_msgq_buffer, sizeof(data_item_type), 10);



void producer_thread(void)
{
    struct data_item_t data;

    while (1) {
        /* create data item to send (e.g. measurement, timestamp, ...) */
        data = ...

        /* send data to consumers */
        while (k_msgq_put(&my_msgq, &data, K_NO_WAIT) != 0) {
            /* message queue is full: purge old data & try again */
            k_msgq_purge(&my_msgq);
        }

        /* data item was successfully added to message queue */
    }
}


void consumer_thread(void)
{
    struct data_item_t data;

    while (1) {
        /* get a data item */
        k_msgq_get(&my_msgq, &data, K_FOREVER);

        /* process data item */
        ...
    }
}