#pragma once

#include <hana/config.hpp>

namespace hana{
    template<typename Tag,typename=void>
    struct make_impl;

    template<typename Tag>
    struct make_t
    {
        template<typename... X>
        constexpr decltype(auto) operator()(X&&... x) const
        {
            return make_impl<Tag>::apply(static_cast<X&&>(x)...);
        }
    };

    template<typename Tag>
    constexpr make_t<Tag> make{};
}