//
// Created by Sam Collins on 3/22/2024
//

#ifndef OASIS_CONSTANTS_HPP
#define OASIS_CONSTANTS_HPP

#include "Real.hpp"
#include <cmath>

namespace Oasis {
class Constant : public Real {
public:
    /**
     * Gets the value of the real number.
     * @return The value of the real number.
     */
    [[nodiscard]] virtual auto GetValue() const -> double = 0;

    [[nodiscard]] virtual auto ToString() const -> std::string override = 0;
};
} // Oasis

#endif