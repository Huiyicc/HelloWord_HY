//
// Created by 19254 on 2024/1/26.
//

#ifndef HELLOWORD_DYNAMIC_FW_COLOR_HPP
#define HELLOWORD_DYNAMIC_FW_COLOR_HPP

namespace HYSDK::Color {

/**
 * @brief Combines the RGB color components into a single integer.
 *
 * This function takes the red, green, and blue color components as input, each represented as a char.
 * It then combines these components into a single integer by shifting the bits of each component to its
 * appropriate position in the integer. The red component is shifted 16 bits to the left, the green component
 * is shifted 8 bits to the left, and the blue component is not shifted. The resulting integer represents
 * the combined color.
 *
 * @param r The red color component.
 * @param g The green color component.
 * @param b The blue color component.
 * @return The combined color as an integer.
 */
int CombineColors(char r, char g, char b);

} // HYSDK

#endif //HELLOWORD_DYNAMIC_FW_COLOR_HPP
