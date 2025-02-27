/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2022 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#ifndef WC4_HPP
#define WC4_HPP

#include "utils/utils.hpp"
#include "wcx/PGT.hpp"

namespace pksm
{
    class WC4 : public PGT
    {
    public:
        explicit WC4(u8* wc4);

        [[nodiscard]] std::string_view extension() const override { return ".wc4"; }

        static constexpr int length = 856;
        [[nodiscard]] std::string title(void) const override { return name; };

        [[nodiscard]] u16 ID(void) const override { return id; }

    private:
        std::string name;
        u16 id;
    };
}

#endif
