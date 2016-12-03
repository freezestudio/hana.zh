#pragma once

#include <hana/config.hpp>
#include <hana/core/when.hpp>

namespace hana{
    template<typename S,typename=void>
    struct accessors_impl : accessors_impl<S,when<true>>
    {

    };

    template<typename S>
    constexpr accessors_t<S> accessors{};
}