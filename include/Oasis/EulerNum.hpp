//
// Created by Sam Collins on 3/22/2024
//

#ifndef OASIS_EULER_HPP
#define OASIS_EULER_HPP

#include "Constant.hpp"
#include <cmath>

namespace Oasis {
class EulerNum : Constant {
public:
    EulerNum() = default;

    /**
     * Gets the value of the real number.
     * @return The value of the real number.
     */
    [[nodiscard]] auto GetValue() const -> double
    {
        return std::exp(1.0);
    }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return "e";
    }
};
} // Oasis

#endif