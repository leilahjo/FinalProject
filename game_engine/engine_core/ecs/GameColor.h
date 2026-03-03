//
// Created by greg8 on 2/12/2026.
//

#ifndef ENGINE_CORE_COLOR_H
#define ENGINE_CORE_COLOR_H

namespace EngineCore
{
    struct GameColor
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    inline constexpr GameColor lightGray{200, 200, 200, 255};
    inline constexpr GameColor gray{130, 130, 130, 255};
    inline constexpr GameColor darkGray{80, 80, 80, 255};
    inline constexpr GameColor yellow{253, 249, 0, 255};
    inline constexpr GameColor gold{255, 203, 0, 255};
    inline constexpr GameColor orange{255, 161, 0, 255};
    inline constexpr GameColor pink{255, 109, 194, 255};
    inline constexpr GameColor red{230, 41, 55, 255};
    inline constexpr GameColor maroon{190, 33, 55, 255};
    inline constexpr GameColor green{0, 228, 48, 255};
    inline constexpr GameColor lime{0, 158, 47, 255};
    inline constexpr GameColor darkGreen{0, 117, 44, 255};
    inline constexpr GameColor skyBlue{102, 191, 255, 255};
    inline constexpr GameColor blue{0, 121, 241, 255};
    inline constexpr GameColor darkBlue{0, 82, 172, 255};
    inline constexpr GameColor purple{200, 122, 255, 255};
    inline constexpr GameColor violet{135, 60, 190, 255};
    inline constexpr GameColor darkPurple{112, 31, 126, 255};
    inline constexpr GameColor beige{211, 176, 131, 255};
    inline constexpr GameColor brown{127, 106, 79, 255};
    inline constexpr GameColor darkBrown{76, 63, 47, 255};

    inline constexpr GameColor white{255, 255, 255, 255};
    inline constexpr GameColor black{0, 0, 0, 255};
    inline constexpr GameColor blank{0, 0, 0, 0};
    inline constexpr GameColor magenta{255, 0, 255, 255};
    inline constexpr GameColor rayWhite{245, 245, 245, 255};
}

#endif //ENGINE_CORE_COLOR_H
