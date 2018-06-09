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
          activities(_self, _self),
          sponsors(_self, _self),
          npos(_self, _self),
          categorities(_self, _self) {
    }

    // @abi action
    void skill(const string& title, bool enabled) {
        print("bee_social::skill\n");

        validate_title(title);

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

    // @abi action
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
        validate_location(location);

        for (auto sit = worker_skills.begin(); sit != worker_skills.end(); ++sit) {
            auto fit = skills.find(*sit);
            eosio_assert(fit != skills.end(), "Skill doesn't exists");
            eosio_assert(fit->enabled, "Skill is disabled");
        }

        auto idx = workers.template get_index<N(worker.accounts)>();
        auto it = idx.find(account);

        if (it == idx.end()) {
            workers.emplace(_self, [&](auto& s) {
                s.id = workers.available_primary_key();
                s.account = account;
                s.full_name = full_name;
                s.location = location;
                s.sex = sex;
                s.birth_date = birth_date;
                s.positive = 0;
                s.negative = 0;
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

    // @abi action
    void activity(const string& title, bool enabled) {
        print("bee_social::activity\n");

        validate_title(title);

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

    // @abi action
    void npo(
        account_name account,
        string title,
        string description,
        string location,
        bool enabled
    ) {
        print("bee_social::npo\n");

        validate_title(title);
        validate_description(description);
        validate_location(location);

        auto idx = npos.template get_index<N(npo.accounts)>();
        auto it = idx.find(account);

        if (it == idx.end()) {
            npos.emplace(_self, [&](auto& s) {
                s.id = workers.available_primary_key();
                s.account = account;
                s.title = title;
                s.description = description;
                s.location = location;
                s.positive = 0;
                s.negative = 0;
                s.enabled = enabled;
            });
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.title = title;
                s.description = description;
                s.location = location;
                s.enabled = enabled;
            });
        }
    }

    // @abi action
    void sponsor(
        account_name account,
        string title,
        string description,
        string location,
        vector<uint64_t> activities,
        bool enabled
    ) {
        print("bee_social::sponsor\n");

        validate_title(title);
        validate_description(description);

        auto idx = sponsors.template get_index<N(sponsor.accounts)>();
        auto it = idx.find(account);

        if (it == idx.end()) {
            sponsors.emplace(_self, [&](auto& s) {
                s.id = sponsors.available_primary_key();
                s.account = account;
                s.title = title;
                s.description = description;
                s.location = location;
                s.positive = 0;
                s.negative = 0;
                s.activities = activities;
                s.enabled = enabled;
            });
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.title = title;
                s.description = description;
                s.location = location;
                s.activities = activities;
                s.enabled = enabled;
            });
        }
    }

    // @abi action
    void category(const string& title, const uint8_t type, bool enabled) {
        print("bee_social::categority\n");

        validate_title(title);

        auto idx = categorities.template get_index<N(categority.titles)>();
        auto it = find_key256<categority_t, &categority_t::title>(idx, title);

        if (it == idx.end()) {
            categorities.emplace(_self, [&](auto& s) {
                s.id = categorities.available_primary_key();
                s.title = title;
                s.type = type;
                s.enabled = enabled;
            });
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.title = title;
                s.type = type;
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

    void validate_title(const string& title) const {
        eosio_assert(title.size() > 0, "Title can't be empty");
        eosio_assert(title.size() < 128, "Title is too big");
    }

    void validate_location(const string& location) const {
        eosio_assert(location.size() > 0, "Location can't be empty");
        eosio_assert(location.size() < 128, "Location is too big");
    }

    void validate_description(const string& description) const {
        eosio_assert(description.size() < 4096, "Description is too big");
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
        uint32_t positive;
        uint32_t negative;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        EOSLIB_SERIALIZE(worker_t, (id)(account)(full_name)(location)(sex)(birth_date)(skills)(positive)(negative)(enabled));
    };

    using worker_index = multi_index<
        N(workers), worker_t,
        indexed_by<N(worker.accounts), const_mem_fun<worker_t, account_name, &worker_t::by_account>>
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

    //@abi table sponsors i64
    struct sponsor_t {
        uint64_t id;
        account_name account;
        string title;
        string location;
        string description;
        vector<uint64_t> activities;
        uint32_t positive;
        uint32_t negative;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        EOSLIB_SERIALIZE(sponsor_t, (id)(account)(title)(location)(description)(activities)(positive)(negative)(enabled));
    };

    using sponsor_index = multi_index<
        N(sponsors), sponsor_t,
        indexed_by<N(sponsor.accounts), const_mem_fun<sponsor_t, account_name, &sponsor_t::by_account>>
    >;

    sponsor_index sponsors;

    //@abi table npos i64
    struct npo_t {
        uint64_t id;
        account_name account;
        string title;
        string description;
        string location;
        uint32_t positive;
        uint32_t negative;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        EOSLIB_SERIALIZE(npo_t, (id)(account)(title)(description)(location)(positive)(negative)(enabled));
    };

    using npo_index = multi_index<
        N(npos), npo_t,
        indexed_by<N(npo.accounts), const_mem_fun<npo_t, account_name, &npo_t::by_account>>
    >;

    npo_index npos;

    //@abi table categorities i64
    struct categority_t {
        uint64_t id;
        string title;
        uint8_t type;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        key256 by_title() const {
            return beesocial::string_to_key256(title);
        }

        EOSLIB_SERIALIZE(categority_t, (id)(title)(type)(enabled));
    };

    using categority_index = multi_index<
    N(categorities), categority_t,
        indexed_by<N(categority.titles), const_mem_fun<categority_t, key256, &categority_t::by_title>>
    >;

    categority_index categorities;
};

EOSIO_ABI(beesocial, (skill)(worker)(activity)(sponsor)(npo)(category))