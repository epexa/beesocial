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
using std::to_string;

class beesocial: public eosio::contract {
public:
    beesocial(account_name self)
        : eosio::contract(self),
          skills(_self, _self),
          workers(_self, _self),
          activities(_self, _self) {
    }

    //@abi action
    void skill(const string& title, bool enabled) {
        print("bee_social::skill\n");

        eosio_assert(title.size() > 0, "Title can't be empty");
        eosio_assert(title.size() < 512, "Title is too big");

        auto idx = skills.template get_index<N(skill.titles)>();
        auto it = find_key256<skill_t, &skill_t::title>(idx, title);

        if (it == idx.end()) {
            skills.emplace(_self, [&](auto& s) {
                s.id = skills.available_primary_key();
                s.title = title;
                s.enabled = enabled;
            });
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.title = title;
                s.enabled = enabled;
            });
        }
    }

    //@abi action
    void worker(
        account_name account,
        const string& full_name,
        const string& location,
        const uint8_t sex,
        time_point_sec birth_date,
        vector<uint64_t> worker_skills,
        bool enabled
    ) {
        print("bee_social::worker\n");

        eosio_assert(full_name.size() > 0, "Full name can't be empty");
        eosio_assert(full_name.size() < 128, "Full name is too big");
        eosio_assert(location.size() > 0, "Location can't be empty");
        eosio_assert(location.size() < 128, "Location is too big");

        for (auto sit = worker_skills.begin(); sit != worker_skills.end(); ++sit) {
            auto fit = skills.find(*sit);
            eosio_assert(fit != skills.end(), "Skill doesn't exists");
            eosio_assert(fit->enabled, "Skill is disabled");
        }

        auto idx = workers.template get_index<N(worker.names)>();
        auto it = idx.find(account);

        if (it == idx.end()) {
            workers.emplace(_self, [&](auto& s) {
                s.id = workers.available_primary_key();
                s.account = account;
                s.full_name = full_name;
                s.location = location;
                s.sex = sex;
                s.birth_date = birth_date;
                s.score = 0;
                s.skills = worker_skills;
                s.enabled = enabled;
            });

            it = idx.find(account);
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.full_name = full_name;
                s.location = location;
                s.sex = sex;
                s.birth_date = birth_date;
                s.skills = worker_skills;
                s.enabled = enabled;
            });
        }
    }

    //@abi action
    void activity(const string& title, bool enabled) {
        print("bee_social::activity\n");

        eosio_assert(title.size() > 0, "Title can't be empty");
        eosio_assert(title.size() < 512, "Title is too big");

        auto idx = activities.template get_index<N(activity.titles)>();
        auto it = find_key256<activity_t, &activity_t::title>(idx, title);

        if (it == idx.end()) {
            activities.emplace(_self, [&](auto& s) {
                s.id = activities.available_primary_key();
                s.title = title;
                s.enabled = enabled;
            });
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.title = title;
                s.enabled = enabled;
            });
        }
    }

    static key256 string_to_key256(const std::string& src) {
        array<uint64_t, 4> v;
        v.fill(0ULL);
        std::memcpy(v.data(), src.data(), std::min(src.size(), v.size() * sizeof(uint64_t)));
        return key256::make_from_word_sequence<uint64_t>(v[0], v[1], v[2], v[3]);
    }

private:
    template <class Class, string Class::* PtrToMember, typename Index>
    typename Index::const_iterator find_key256(const Index& idx, const string& value) const {
        auto et = idx.end();
        typename Index::secondary_extractor_type key_extractor;
        auto key = string_to_key256(value);
        auto it = idx.find(key);
        for (; et != it && key_extractor(*it) == key; ++it) {
            if (value == ((*it).*PtrToMember)) {
                return it;
            }
        }
        return et;
    }

    template <class Class, string Class::* PtrToMember, typename Index>
    typename Index::const_iterator has_key256(const Index& idx, const string& value) const {
        auto it = find_key256<Class, PtrToMember>(idx, value);
        return it != idx.end();
    }

    //@abi table skills i64
    struct skill_t {
        uint64_t id;
        string title;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        key256 by_title() const {
            return beesocial::string_to_key256(title);
        }

        EOSLIB_SERIALIZE(skill_t, (id)(title)(enabled));
    };

    using skill_index = multi_index<
        N(skills), skill_t,
        indexed_by<N(skill.titles), const_mem_fun<skill_t, key256, &skill_t::by_title>>
    >;

    skill_index skills;

    //@abi table workers i64
    struct worker_t {
        uint64_t id;
        account_name account;
        string full_name;
        string location;
        uint8_t sex;
        time_point_sec birth_date;
        vector<uint64_t> skills;
        uint32_t score;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        EOSLIB_SERIALIZE(worker_t, (id)(account)(full_name)(location)(sex)(birth_date)(skills)(score)(enabled));
    };

    using worker_index = multi_index<
        N(workers), worker_t,
        indexed_by<N(worker.names), const_mem_fun<worker_t, account_name, &worker_t::by_account>>
    >;

    worker_index workers;

    //@abi table activities i64
    struct activity_t {
        uint64_t id;
        string title;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        key256 by_title() const {
            return beesocial::string_to_key256(title);
        }

        EOSLIB_SERIALIZE(activity_t, (id)(title)(enabled));
    };

    using activity_index = multi_index<
        N(activities), activity_t,
        indexed_by<N(activity.titles), const_mem_fun<activity_t, key256, &activity_t::by_title>>
    >;

    activity_index activities;
};

EOSIO_ABI(beesocial, (skill)(worker))