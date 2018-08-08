#ifndef SBPL_PLATFORM_SIERRA_FILTERS_H
#define SBPL_PLATFORM_SIERRA_FILTERS_H

#include <sb/filters/types.h>

const size_t sierra_filters(const filter_info_t **infos,
                            filter_accept_type_func *acceptor);

#endif