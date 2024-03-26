//
// Created by Sam Collins on 3/22/2024
//

#ifndef OASIS_CONSTANTS_HPP
#define OASIS_CONSTANTS_HPP

#include "Real.hpp"
#include <cmath>

namespace Oasis {
class Constant : Real {
public:
    /**
     * Gets the value of the real number.
     * @return The value of the real number.
     */
    [[nodiscard]] virtual auto GetValue() const -> double;

    [[nodiscard]] virtual auto ToString() const -> std::string override;
};
} // Oasis

#endif