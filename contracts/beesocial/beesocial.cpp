#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/time.hpp>

using eosio::key256;
using eosio::time_point_sec;

using eosio::multi_index;
using eosio::const_mem_fun;
using eosio::indexed_by;

using eosio::print;

using std::string;
using std::vector;
using std::array;

class beesocial: public eosio::contract {
public:
    beesocial(account_name self)
        : eosio::contract(self),
          skills(_self, _self),
          volounters(_self, _self) {
    }

    //@abi action
    void skill(string name) {
        print("bee_social::skill\n");
    }

    //@abi action
    void volounter(
        account_name account,
        string full_name,
        string location,
        time_point_sec birth_date,
        vector<uint64_t> skills
    ) {
        print("bee_social::volounter\n");
    }

    static key256 string_to_key256(const std::string& src) {
        array<uint64_t, 4> v;
        v.fill(0ULL);
        std::memcpy(v.data(), src.data(), std::min(src.size(), v.size() * sizeof(uint64_t)));
        return key256::make_from_word_sequence<uint64_t>(v[0], v[1], v[2], v[3]);
    }

private:
    //@abi table skills i64
    struct skill_t {
        uint64_t id;
        string title;

        uint64_t primary_key() const {
            return id;
        }

        key256 by_title() const {
            return beesocial::string_to_key256(title);
        }

        EOSLIB_SERIALIZE(skill_t, (id)(title));
    };

    using skill_index = multi_index<
        N(skills), skill_t,
        indexed_by<N(skill.names), const_mem_fun<skill_t, key256, &skill_t::by_title>>
    >;

    skill_index skills;

    //@abi table volounters i64
    struct volounter_t {
        uint64_t id;
        account_name account;
        string name;
        string location;
        time_point_sec birth_date;

        uint64_t primary_key() const {
            return id;
        }

        key256 by_name() const {
            return beesocial::string_to_key256(name);
        }

        key256 by_location() const {
            return beesocial::string_to_key256(location);
        }

        EOSLIB_SERIALIZE(volounter_t, (id)(account)(name)(location)(birth_date));
    };

    using volounter_index = multi_index<
        N(volounters), volounter_t,
        indexed_by<N(volounter.names), const_mem_fun<volounter_t, key256, &volounter_t::by_name>>,
        indexed_by<N(volounter.locations), const_mem_fun<volounter_t, key256, &volounter_t::by_location>>
    >;

    volounter_index volounters;
};

EOSIO_ABI(beesocial, (skill)(volounter))