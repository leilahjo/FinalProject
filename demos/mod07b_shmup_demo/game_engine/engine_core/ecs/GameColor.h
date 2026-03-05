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

    constexpr GameColor lightGray{200, 200, 200, 255};
    constexpr GameColor gray{130, 130, 130, 255};
    constexpr GameColor darkGray{80, 80, 80, 255};
    constexpr GameColor yellow{253, 249, 0, 255};
    constexpr GameColor gold{255, 203, 0, 255};
    constexpr GameColor orange{255, 161, 0, 255};
    constexpr GameColor pink{255, 109, 194, 255};
    constexpr GameColor red{230, 41, 55, 255};
    constexpr GameColor maroon{190, 33, 55, 255};
    constexpr GameColor green{0, 228, 48, 255};
    constexpr GameColor lime{0, 158, 47, 255};
    constexpr GameColor darkGreen{0, 117, 44, 255};
    constexpr GameColor skyBlue{102, 191, 255, 255};
    constexpr GameColor blue{0, 121, 241, 255};
    constexpr GameColor darkBlue{0, 82, 172, 255};
    constexpr GameColor purple{200, 122, 255, 255};
    constexpr GameColor violet{135, 60, 190, 255};
    constexpr GameColor darkPurple{112, 31, 126, 255};
    constexpr GameColor beige{211, 176, 131, 255};
    constexpr GameColor brown{127, 106, 79, 255};
    constexpr GameColor darkBrown{76, 63, 47, 255};

    constexpr GameColor white{255, 255, 255, 255};
    constexpr GameColor black{0, 0, 0, 255};
    constexpr GameColor blank{0, 0, 0, 0};
    constexpr GameColor magenta{255, 0, 255, 255};
    constexpr GameColor rayWhite{245, 245, 245, 255};
}

#endif //ENGINE_CORE_COLOR_H
