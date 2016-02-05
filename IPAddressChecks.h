#ifndef IPADDRESS_CHECKS_H_
#define IPADDRESS_CHECKS_H_

#include <inttypes.h>


void initialize_ip_address_checks();

//guess distance to IP
static const unsigned ip_distance_ourselves = 0;
static const unsigned ip_distance_lan = 1;
static const unsigned ip_distance_nearby = 2;
//  >2 = internet
unsigned ip_distance(uint32_t ip/*network-order*/);

//Is the IP an internal IP as in "we control the hosts and allow more aggressive crawling"
bool is_internal_net_ip(uint32_t ip/*network-order*/);

#endif