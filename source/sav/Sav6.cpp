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

#include "sav/Sav6.hpp"
#include "pkx/PK6.hpp"
#include "utils/endian.hpp"
#include "utils/i18n.hpp"
#include "utils/random.hpp"
#include "utils/utils.hpp"
#include "wcx/WC6.hpp"

namespace pksm
{
    u16 Sav6::TID(void) const
    {
        return LittleEndian::convertTo<u16>(&data[TrainerCard]);
    }
    void Sav6::TID(u16 v)
    {
        LittleEndian::convertFrom<u16>(&data[TrainerCard], v);
    }

    u16 Sav6::SID(void) const
    {
        return LittleEndian::convertTo<u16>(&data[TrainerCard + 2]);
    }
    void Sav6::SID(u16 v)
    {
        LittleEndian::convertFrom<u16>(&data[TrainerCard + 2], v);
    }

    GameVersion Sav6::version(void) const
    {
        return GameVersion(data[TrainerCard + 4]);
    }
    void Sav6::version(GameVersion v)
    {
        data[TrainerCard + 4] = u8(v);
    }

    Gender Sav6::gender(void) const
    {
        return Gender{data[TrainerCard + 5]};
    }
    void Sav6::gender(Gender v)
    {
        data[TrainerCard + 5] = u8(v);
    }

    u8 Sav6::subRegion(void) const
    {
        return data[TrainerCard + 0x26];
    }
    void Sav6::subRegion(u8 v)
    {
        data[TrainerCard + 0x26] = v;
    }

    u8 Sav6::country(void) const
    {
        return data[TrainerCard + 0x27];
    }
    void Sav6::country(u8 v)
    {
        data[TrainerCard + 0x27] = v;
    }

    u8 Sav6::consoleRegion(void) const
    {
        return data[TrainerCard + 0x2C];
    }
    void Sav6::consoleRegion(u8 v)
    {
        data[TrainerCard + 0x2C] = v;
    }

    Language Sav6::language(void) const
    {
        return Language(data[TrainerCard + 0x2D]);
    }
    void Sav6::language(Language v)
    {
        data[TrainerCard + 0x2D] = u8(v);
    }

    std::string Sav6::otName(void) const
    {
        return StringUtils::transString67(
            StringUtils::getString(data.get(), TrainerCard + 0x48, 13));
    }
    void Sav6::otName(const std::string_view& v)
    {
        StringUtils::setString(data.get(), StringUtils::transString67(v), TrainerCard + 0x48, 13);
    }

    u32 Sav6::money(void) const
    {
        return LittleEndian::convertTo<u32>(&data[Trainer2 + 0x8]);
    }
    void Sav6::money(u32 v)
    {
        LittleEndian::convertFrom<u32>(&data[Trainer2 + 0x8], v);
    }

    u32 Sav6::BP(void) const
    {
        return LittleEndian::convertTo<u32>(&data[Trainer2 + (game == Game::XY ? 0x3C : 0x30)]);
    }
    void Sav6::BP(u32 v)
    {
        LittleEndian::convertFrom<u32>(&data[Trainer2 + (game == Game::XY ? 0x3C : 0x30)], v);
    }

    u8 Sav6::badges(void) const
    {
        u8& badgeBits = data[Trainer2 + 0xC];
        u8 ret        = 0;
        for (size_t i = 0; i < sizeof(badgeBits) * 8; i++)
        {
            ret += (badgeBits & (1 << i)) ? 1 : 0;
        }
        return ret;
    }

    u16 Sav6::playedHours(void) const
    {
        return LittleEndian::convertTo<u16>(&data[PlayTime]);
    }
    void Sav6::playedHours(u16 v)
    {
        LittleEndian::convertFrom<u16>(&data[PlayTime], v);
    }

    u8 Sav6::playedMinutes(void) const
    {
        return data[PlayTime + 2];
    }
    void Sav6::playedMinutes(u8 v)
    {
        data[PlayTime + 2] = v;
    }

    u8 Sav6::playedSeconds(void) const
    {
        return data[PlayTime + 3];
    }
    void Sav6::playedSeconds(u8 v)
    {
        data[PlayTime + 3] = v;
    }

    u8 Sav6::currentBox(void) const
    {
        return data[LastViewedBox];
    }
    void Sav6::currentBox(u8 v)
    {
        data[LastViewedBox] = v;
    }

    u8 Sav6::unlockedBoxes(void) const
    {
        return data[LastViewedBox - 1];
    }
    void Sav6::unlockedBoxes(u8 v)
    {
        data[LastViewedBox - 1] = v;
    }

    u32 Sav6::boxOffset(u8 box, u8 slot) const
    {
        return Box + PK6::BOX_LENGTH * 30 * box + PK6::BOX_LENGTH * slot;
    }

    u32 Sav6::partyOffset(u8 slot) const
    {
        return Party + PK6::PARTY_LENGTH * slot;
    }

    std::unique_ptr<PKX> Sav6::pkm(u8 slot) const
    {
        return PKX::getPKM<Generation::SIX>(&data[partyOffset(slot)], PK6::PARTY_LENGTH);
    }

    void Sav6::pkm(const PKX& pk, u8 slot)
    {
        if (pk.generation() == Generation::SIX)
        {
            auto pk6 = pk.partyClone();
            pk6->encrypt();
            std::ranges::copy(pk6->rawData(), &data[partyOffset(slot)]);
        }
    }

    std::unique_ptr<PKX> Sav6::pkm(u8 box, u8 slot) const
    {
        return PKX::getPKM<Generation::SIX>(&data[boxOffset(box, slot)], PK6::BOX_LENGTH);
    }

    void Sav6::pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade)
    {
        if (pk.generation() == Generation::SIX)
        {
            auto pkm = pk.clone();
            if (applyTrade)
            {
                trade(*pkm);
            }

            std::ranges::copy(
                pkm->rawData().subspan(0, PK6::BOX_LENGTH), &data[boxOffset(box, slot)]);
        }
    }

    void Sav6::trade(PKX& pk, const Date& date) const
    {
        if (pk.generation() == Generation::SIX)
        {
            PK6& pk6 = static_cast<PK6&>(pk);
            if (pk6.egg())
            {
                if (otName() != pk6.otName() || TID() != pk6.TID() || SID() != pk6.SID() ||
                    gender() != pk6.otGender())
                {
                    pk6.metLocation(30002);
                    pk6.metDate(date);
                }
            }
            else if (otName() == pk6.otName() && TID() == pk6.TID() && SID() == pk6.SID() &&
                     gender() == pk6.otGender())
            {
                pk6.currentHandler(0);

                if (!pk6.untraded() &&
                    (country() != pk6.geoCountry(0) || subRegion() != pk6.geoRegion(0)))
                {
                    for (int i = 4; i > 0; i--)
                    {
                        pk6.geoCountry(pk6.geoCountry(i - 1), i);
                        pk6.geoRegion(pk6.geoRegion(i - 1), i);
                    }
                    pk6.geoCountry(0, country());
                    pk6.geoRegion(0, subRegion());
                }
            }
            else
            {
                if (otName() != pk6.htName() || gender() != pk6.htGender() ||
                    (pk6.geoCountry(0) == 0 && pk6.geoRegion(0) == 0 && !pk6.untradedEvent()))
                {
                    for (int i = 4; i > 0; i--)
                    {
                        pk6.geoCountry(pk6.geoCountry(i - 1), i);
                        pk6.geoRegion(pk6.geoRegion(i - 1), i);
                    }
                    pk6.geoCountry(0, country());
                    pk6.geoRegion(0, subRegion());
                }

                if (pk6.htName() != otName())
                {
                    pk6.htFriendship(pk6.baseFriendship());
                    pk6.htAffection(0);
                    pk6.htName(otName());
                }
                pk6.currentHandler(1);
                pk6.htGender(gender());

                if (pk6.htMemory() == 0)
                {
                    pk6.htMemory(4);
                    pk6.htTextVar(9);
                    pk6.htIntensity(1);

                    /*static constexpr u32 memoryBits[70] = {
                        0x000000, 0x04CBFD, 0x004BFD, 0x04CBFD, 0x04CBFD, 0xFFFBFB, 0x84FFF9,
                    0x47FFFF, 0xBF7FFA, 0x7660B0, 0x80BDF9, 0x88FB7A, 0x083F79, 0x0001FE, 0xCFEFFF,
                    0x84EBAF, 0xB368B0, 0x091F7E, 0x0320A0, 0x080DDD, 0x081A7B, 0x404030, 0x0FFFFF,
                    0x9A08BC, 0x089A7B, 0x0032AA, 0x80FF7A, 0x0FFFFF, 0x0805FD, 0x098278, 0x0B3FFF,
                    0x8BBFFA, 0x8BBFFE, 0x81A97C, 0x8BB97C, 0x8BBF7F, 0x8BBF7F, 0x8BBF7F, 0x8BBF7F,
                    0xAC3ABE, 0xBFFFFF, 0x8B837C, 0x848AFA, 0x88FFFE, 0x8B0B7C, 0xB76AB2, 0x8B1FFF,
                    0xBE7AB8, 0xB77EB8, 0x8C9FFD, 0xBF9BFF, 0xF408B0, 0xBCFE7A, 0x8F3F72, 0x90DB7A,
                    0xBCEBFF, 0xBC5838, 0x9C3FFE, 0x9CFFFF, 0x96D83A, 0xB770B0, 0x881F7A, 0x839F7A,
                    0x839F7A, 0x839F7A, 0x53897F, 0x41BB6F, 0x0C35FF, 0x8BBF7F, 0x8BBF7F
                    };*/

                    u32 bits = 0x04CBFD; // memoryBits[pk6.htMemory()];
                    while (true)
                    {
                        u32 feel = pksm::randomNumber(0, 19);
                        if ((bits & (1 << feel)) != 0)
                        {
                            pk6.htFeeling(feel);
                            break;
                        }
                    }
                }
            }
        }
    }

    void Sav6::cryptBoxData(bool crypted)
    {
        for (u8 box = 0; box < maxBoxes(); box++)
        {
            for (u8 slot = 0; slot < 30; slot++)
            {
                std::unique_ptr<PKX> pk6 = PKX::getPKM<Generation::SIX>(
                    &data[boxOffset(box, slot)], PK6::BOX_LENGTH, true);
                if (!crypted)
                {
                    pk6->encrypt();
                }
            }
        }
    }

    int Sav6::dexFormIndex(int species, int formct) const
    {
        if (formct < 1 || species < 0)
            return -1; // invalid

        if (game == Game::ORAS)
        {
            switch (species)
            {
                case 25:
                    return 189; // 7 Pikachu
                case 720:
                    return 196; // 2 Hoopa
                case 15:
                    return 198; // 2 Beedrill
                case 18:
                    return 200; // 2 Pidgeot
                case 80:
                    return 202; // 2 Slowbro
                case 208:
                    return 204; // 2 Steelix
                case 254:
                    return 206; // 2 Sceptile
                case 260:
                    return 208; // 2 Swampert
                case 302:
                    return 210; // 2 Sableye
                case 319:
                    return 212; // 2 Sharpedo
                case 323:
                    return 214; // 2 Camerupt
                case 334:
                    return 216; // 2 Altaria
                case 362:
                    return 218; // 2 Glalie
                case 373:
                    return 220; // 2 Salamence
                case 376:
                    return 222; // 2 Metagross
                case 384:
                    return 224; // 2 Rayquaza
                case 428:
                    return 226; // 2 Lopunny
                case 475:
                    return 228; // 2 Gallade
                case 531:
                    return 230; // 2 Audino
                case 719:
                    return 232; // 2 Diancie
                case 382:
                    return 234; // 2 Kyogre
                case 383:
                    return 236; // 2 Groudon
                case 493:
                    return 238; // 18 Arceus
                case 649:
                    return 256; // 5 Genesect
                case 676:
                    return 261; // 10 Furfrou
            }
        }

        switch (species)
        {
            case 666:
                return 83; // 20 Vivillion
            case 669:
                return 103; // 5 Flabébé
            case 670:
                return 108; // 6 Floette
            case 671:
                return 114; // 5 Florges
            case 710:
                return 119; // 4 Pumpkaboo
            case 711:
                return 123; // 4 Gourgeist
            case 681:
                return 127; // 2 Aegislash
            case 716:
                return 129; // 2 Xerneas
            case 3:
                return 131; // 2 Venusaur
            case 6:
                return 133; // 3 Charizard
            case 9:
                return 136; // 2 Blastoise
            case 65:
                return 138; // 2 Alakazam
            case 94:
                return 140; // 2 Gengar
            case 115:
                return 142; // 2 Kangaskhan
            case 127:
                return 144; // 2 Pinsir
            case 130:
                return 146; // 2 Gyarados
            case 142:
                return 148; // 2 Aerodactyl
            case 150:
                return 150; // 3 Mewtwo
            case 181:
                return 153; // 2 Ampharos
            case 212:
                return 155; // 2 Scizor
            case 214:
                return 157; // 2 Heracros
            case 229:
                return 159; // 2 Houndoom
            case 248:
                return 161; // 2 Tyranitar
            case 257:
                return 163; // 2 Blaziken
            case 282:
                return 165; // 2 Gardevoir
            case 303:
                return 167; // 2 Mawile
            case 306:
                return 169; // 2 Aggron
            case 308:
                return 171; // 2 Medicham
            case 310:
                return 173; // 2 Manetric
            case 354:
                return 175; // 2 Banette
            case 359:
                return 177; // 2 Absol
            case 380:
                return 179; // 2 Latias
            case 381:
                return 181; // 2 Latios
            case 445:
                return 183; // 2 Garchomp
            case 448:
                return 185; // 2 Lucario
            case 460:
                return 187; // 2 Abomasnow
            case 646:
                return 72; // 3 Kyurem
            case 647:
                return 75; // 2 Keldeo
            case 642:
                return 77; // 2 Thundurus
            case 641:
                return 79; // 2 Tornadus
            case 645:
                return 81; // 2 Landorus
            case 201:
                return 0; // 28 Unown
            case 386:
                return 28; // 4 Deoxys
            case 492:
                return 32; // 2 Shaymin
            case 487:
                return 34; // 2 Giratina
            case 479:
                return 36; // 6 Rotom
            case 422:
                return 42; // 2 Shellos
            case 423:
                return 44; // 2 Gastrodon
            case 412:
                return 46; // 3 Burmy
            case 413:
                return 49; // 3 Wormadam
            case 351:
                return 52; // 4 Castform
            case 421:
                return 56; // 2 Cherrim
            case 585:
                return 58; // 4 Deerling
            case 586:
                return 62; // 4 Sawsbuck
            case 648:
                return 66; // 2 Meloetta
            case 555:
                return 68; // 2 Darmanitan
            case 550:
                return 70; // 2 Basculin
            default:
                return -1;
        }
    }

    void Sav6::dex(const PKX& pk)
    {
        if (!(availableSpecies().count(pk.species()) > 0) || pk.egg())
            return;

        const int brSize = 0x60;
        int bit          = u16(pk.species()) - 1;
        int lang         = u8(pk.language()) - 1;
        if (lang > 5)
            lang--;                         // 0-6 language vals
        int gender   = u8(pk.gender()) % 2; // genderless -> male
        int shiny    = pk.shiny() ? 1 : 0;
        int shiftoff = brSize * (1 + gender + 2 * shiny); // after the Owned region
        int bd       = bit >> 3;                          // div8
        int bm       = bit & 7;                           // mod8
        u8 mask      = (u8)(1 << bm);
        int ofs      = PokeDex + 0x8 + bd;

        // Owned quality flag
        if (pk.version() < GameVersion::X && bit < 649 &&
            game !=
                Game::ORAS) // Species: 1-649 for X/Y, and not for ORAS; Set the Foreign Owned Flag
            data[ofs + 0x644] |= mask;
        else if (pk.version() >= GameVersion::X ||
                 game == Game::ORAS) // Set Native Owned Flag (should always happen)
            data[ofs + (brSize * 0)] |= mask;

        // Set the [Species/Gender/Shiny] Seen Flag
        data[ofs + shiftoff] |= mask;

        // Set the Display flag if none are set
        bool displayed = false;
        displayed |= (data[ofs + brSize * 5] & mask) != 0;
        displayed |= (data[ofs + brSize * 6] & mask) != 0;
        displayed |= (data[ofs + brSize * 7] & mask) != 0;
        displayed |= (data[ofs + brSize * 8] & mask) != 0;
        if (!displayed) // offset is already biased by brSize, reuse shiftoff but for the display
                        // flags.
            data[ofs + brSize * 4 + shiftoff] |= mask;

        // Set the Language
        if (lang < 0)
            lang = 1;
        data[PokeDexLanguageFlags + (bit * 7 + lang) / 8] |= (u8)(1 << ((bit * 7 + lang) % 8));

        // Set DexNav count (only if not encountered previously)
        if (game == Game::ORAS &&
            LittleEndian::convertTo<u16>(&data[EncounterCount + (u16(pk.species()) - 1) * 2]) == 0)
            LittleEndian::convertFrom<u16>(&data[EncounterCount + (u16(pk.species()) - 1) * 2], 1);

        // Set Form flags
        int fc = PersonalXYORAS::formCount(u16(pk.species()));
        int f  = dexFormIndex(u16(pk.species()), fc);
        if (f < 0)
            return;

        int formLen = game == Game::XY ? 0x18 : 0x26;
        int formDex = PokeDex + 0x8 + brSize * 9;
        bit         = f + pk.alternativeForm();

        // Set Form Seen Flag
        data[formDex + formLen * shiny + bit / 8] |= (u8)(1 << (bit % 8));

        // Set Displayed Flag if necessary, check all flags
        for (int i = 0; i < fc; i++)
        {
            bit = f + i;
            if ((data[formDex + formLen * 2 + bit / 8] & (u8)(1 << (bit % 8))) != 0) // Nonshiny
                return;                                                              // already set
            if ((data[formDex + formLen * 3 + bit / 8] & (u8)(1 << (bit % 8))) != 0) // Shiny
                return;                                                              // already set
        }
        bit = f + pk.alternativeForm();
        data[formDex + formLen * (2 + shiny) + bit / 8] |= (u8)(1 << (bit % 8));
    }

    int Sav6::dexSeen(void) const
    {
        int ret = 0;
        for (const auto& spec : availableSpecies())
        {
            u16 i        = u16(spec);
            int bitIndex = (i - 1) & 7;
            for (int j = 0; j < 4; j++) // All seen flags: gender & shinies
            {
                int ofs = PokeDex + (0x68 + (j * 0x60)) + ((i - 1) >> 3);
                if ((data[ofs] >> bitIndex & 1) != 0)
                {
                    ret++;
                    break;
                }
            }
        }
        return ret;
    }

    int Sav6::dexCaught(void) const
    {
        int ret = 0;
        for (const auto& spec : availableSpecies())
        {
            u16 i        = u16(spec);
            int bitIndex = (i - 1) & 7;
            int ofs      = PokeDex + 0x8 + ((i - 1) >> 3);
            if ((data[ofs] >> bitIndex & 1) != 0)
            {
                ret++;
            }
        }
        return ret;
    }

    void Sav6::mysteryGift(const WCX& wc, int& pos)
    {
        if (wc.generation() == Generation::SIX)
        {
            data[WondercardFlags + wc.ID() / 8] |= 0x1 << (wc.ID() % 8);
            std::copy(wc.rawData(), wc.rawData() + WC6::length,
                &data[WondercardData + WC6::length * pos]);
            if (game == Game::ORAS && wc.ID() == 2048 && wc.object() == 726)
            {
                static constexpr u32 EON_MAGIC = 0x225D73C2;
                LittleEndian::convertFrom<u32>(&data[0x319B8], EON_MAGIC);
                LittleEndian::convertFrom<u32>(&data[0x319DE], EON_MAGIC);
            }
            pos = (pos + 1) % 24;
        }
    }

    std::string Sav6::boxName(u8 box) const
    {
        return StringUtils::transString67(
            StringUtils::getString(data.get(), PCLayout + 0x22 * box, 17));
    }
    void Sav6::boxName(u8 box, const std::string_view& name)
    {
        StringUtils::setString(
            data.get(), StringUtils::transString67(name), PCLayout + 0x22 * box, 17);
    }

    u8 Sav6::boxWallpaper(u8 box) const
    {
        return data[0x4400 + 1054 + box];
    }
    void Sav6::boxWallpaper(u8 box, u8 v)
    {
        data[0x4400 + 1054 + box] = v;
    }

    u8 Sav6::partyCount(void) const
    {
        return data[Party + 6 * PK6::PARTY_LENGTH];
    }
    void Sav6::partyCount(u8 v)
    {
        data[Party + 6 * PK6::PARTY_LENGTH] = v;
    }

    std::unique_ptr<PKX> Sav6::emptyPkm() const
    {
        return PKX::getPKM<Generation::SIX>(nullptr, PK6::BOX_LENGTH);
    }

    int Sav6::currentGiftAmount(void) const
    {
        u8 t;
        // 24 max wonder cards
        for (t = 0; t < 24; t++)
        {
            bool empty = true;
            for (u32 j = 0; j < WC6::length; j++)
            {
                if (data[WondercardData + t * WC6::length + j] != 0)
                {
                    empty = false;
                    break;
                }
            }

            if (empty)
            {
                break;
            }
        }

        return t;
    }

    std::unique_ptr<WCX> Sav6::mysteryGift(int pos) const
    {
        return std::make_unique<WC6>(&data[WondercardData + pos * WC6::length]);
    }

    void Sav6::item(const Item& item, Pouch pouch, u16 slot)
    {
        Item6 inject = static_cast<Item6>(item);
        auto write   = inject.bytes();
        switch (pouch)
        {
            case Pouch::NormalItem:
                std::copy(write.begin(), write.end(), &data[PouchHeldItem + slot * 4]);
                break;
            case Pouch::KeyItem:
                std::copy(write.begin(), write.end(), &data[PouchKeyItem + slot * 4]);
                break;
            case Pouch::TM:
                std::copy(write.begin(), write.end(), &data[PouchTMHM + slot * 4]);
                break;
            case Pouch::Medicine:
                std::copy(write.begin(), write.end(), &data[PouchMedicine + slot * 4]);
                break;
            case Pouch::Berry:
                std::copy(write.begin(), write.end(), &data[PouchBerry + slot * 4]);
                break;
            default:
                return;
        }
    }

    std::unique_ptr<Item> Sav6::item(Pouch pouch, u16 slot) const
    {
        switch (pouch)
        {
            case Pouch::NormalItem:
                return std::make_unique<Item6>(&data[PouchHeldItem + slot * 4]);
            case Pouch::KeyItem:
                return std::make_unique<Item6>(&data[PouchKeyItem + slot * 4]);
            case Pouch::TM:
                return std::make_unique<Item6>(&data[PouchTMHM + slot * 4]);
            case Pouch::Medicine:
                return std::make_unique<Item6>(&data[PouchMedicine + slot * 4]);
            case Pouch::Berry:
                return std::make_unique<Item6>(&data[PouchBerry + slot * 4]);
            default:
                return nullptr;
        }
    }

    std::vector<std::pair<Sav::Pouch, int>> Sav6::pouches(void) const
    {
        return {{Pouch::NormalItem, game == Game::XY ? 286 : 305},
            {Pouch::KeyItem, game == Game::XY ? 31 : 47}, {Pouch::TM, game == Game::XY ? 105 : 107},
            {Pouch::Medicine, game == Game::XY ? 51 : 54}, {Pouch::Berry, 67}};
    }
}
