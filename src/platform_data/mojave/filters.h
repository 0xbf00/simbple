#ifndef SBPL_PLATFORM_MOJAVE_FILTERS_H
#define SBPL_PLATFORM_MOJAVE_FILTERS_H

#include <sb/filters/types.h>

const size_t mojave_filters(const filter_info_t **infos,
                            filter_accept_type_func *acceptor);

#endif