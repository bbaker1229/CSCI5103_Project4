#include "consumer.h"
#include "producer.h"

int main()
{
	consumer_init();
	producer_init();
	consumer_close();
	producer_close();
	return 0;
}
