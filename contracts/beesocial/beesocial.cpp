#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/time.hpp>

#include <cmath>

using eosio::key256;
using eosio::time_point_sec;
using eosio::asset;
using eosio::name;

using eosio::multi_index;
using eosio::const_mem_fun;
using eosio::indexed_by;

using eosio::print;

using std::string;
using std::vector;
using std::array;
using std::to_string;
using std::set;
using std::map;

#define BEESOCIAL_SYMBOL (eosio::string_to_symbol(4, "BEOS"))

enum project_status {
    project_created = 0,
    project_started = 1,
    project_canceled = 2,
    project_complete = 3,
    project_rewarded = 4

};

enum request_status {
    request_enabled = 0,
    request_disabled = 1,
    request_failed = 2,
    request_complete = 3
};

class beesocial: public eosio::contract {
public:
    beesocial(account_name self)
        : eosio::contract(self),
          skills(_self, _self),
          workers(_self, _self),
          activities(_self, _self),
          sponsors(_self, _self),
          npos(_self, _self),
          categorities(_self, _self),
          resources(_self, _self),
          projects(_self, _self),
          requests(_self, _self) {
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
        validate_skills(worker_skills);

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
                s.reward = asset(0, BEESOCIAL_SYMBOL);
                s.enabled = enabled;
            });
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
                s.id = npos.available_primary_key();
                s.account = account;
                s.title = title;
                s.description = description;
                s.location = location;
                s.positive = 0;
                s.negative = 0;
                s.reward = asset(0, BEESOCIAL_SYMBOL);
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
                s.reward = asset(0, BEESOCIAL_SYMBOL);
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

    // @abi action
    void resource(
        account_name account,
        string title,
        uint8_t status,
        uint64_t category,
        string description,
        string how_get,
        string contacts,
        asset price
    ) {
        print("bee_social::resource\n");

        validate_title(title);
        validate_description(description);

        eosio_assert(how_get.size() > 0, "how_get can't be empty");
        eosio_assert(how_get.size() < 4096, "how_get is too big");
        eosio_assert(contacts.size() > 0, "Contacts can't be empty");
        eosio_assert(contacts.size() < 4096, "Contacts is too big");

        auto cit = categorities.find(category);
        eosio_assert(cit != categorities.end(), "Category doesn't exists");
        eosio_assert((*cit).enabled, "Category is disabled");

        auto key = name{account}.to_string() + ":" + title;
        auto idx = resources.template get_index<N(resource.keys)>();
        auto it = find_key256_fun<resource_t, &resource_t::get_key>(idx, key);

        if (it == idx.end()) {
            resources.emplace(_self, [&](auto& s) {
                s.id = resources.available_primary_key();
                s.account = account;
                s.title = title;
                s.status = status;
                s.category = category;
                s.description = description;
                s.how_get = how_get;
                s.contacts = contacts;
                s.price = price;
            });
        } else {
            idx.modify(it, 0, [&](auto& s){
                s.status = status;
                s.category = category;
                s.description = description;
                s.how_get = how_get;
                s.contacts = contacts;
                s.price = price;
            });
        }
    }

    // @abi action
    void project(
        account_name npo,
        string title,
        string description,
        vector<uint64_t> skills,
        time_point_sec date_from,
        time_point_sec date_to,
        asset price,
        uint32_t required
    ) {
        print("bee_social::project\n");

        validate_title(title);
        validate_description(description);
        validate_skills(skills);
        validate_asset(price);

        eosio_assert(required > 0, "Required workers should be more than zero");

        price.print();
        eosio::print("\n");

        auto nidx = npos.template get_index<N(npo.accounts)>();
        auto nit = nidx.find(npo);
        eosio_assert(nit != nidx.end(), "NPO doesn't exists");
        eosio_assert((*nit).enabled, "NPO is disabled");


        auto key = to_string((*nit).id) + ":" + title;
        auto idx = projects.template get_index<N(project.keys)>();
        auto it = find_key256_fun<project_t, &project_t::get_key>(idx, key);

        if (it == idx.end()) {
            projects.emplace(_self, [&](auto& s) {
                s.id = projects.available_primary_key();
                s.npo = (*nit).id;
                s.title = title;
                s.description = description;
                s.skills = skills;
                s.created = time_point_sec(now());
                s.done = time_point_sec::min();
                s.date_from = date_from;
                s.date_to = date_to;
                s.status = project_created;
                s.price = price;
                s.required = required;
                s.hired = 0;
                s.rewarded = 0;
            });
        } else {
            eosio_assert(it->status == project_created, "Project can't be modified after started/canceled/complete.");
            eosio_assert(it->hired <= required, "Project has hired more workers, than required");

            idx.modify(it, 0, [&](auto& s){
                s.description = description;
                s.skills = skills;
                s.date_from = date_from;
                s.date_to = date_to;
                s.price = price;
                s.required = required;
            });
        }
    }

    // @abi action
    void request(const uint64_t project, const uint64_t worker, const uint8_t status) {
        print("bee_social::request\n");

        auto pit = projects.find(project);
        eosio_assert(pit != projects.end(), "Project doesn't exist");
        eosio_assert((*pit).status == project_started, "Project isn't started");

        auto wit = workers.find(worker);
        eosio_assert(wit != workers.end(), "Worker doesn't exist");
        eosio_assert((*wit).enabled, "Worker is disabled");

        eosio_assert(status == request_enabled || status == request_disabled, "Invalid status");

        auto key = (static_cast<uint128_t>(project) << 64) + worker;
        auto idx = requests.template get_index<N(request.keys)>();
        auto it = idx.find(key);

        if (it == idx.end()) {
            set<uint64_t> skills((*pit).skills.begin(), (*pit).skills.end());
            bool has_skill = false;

            for (auto sit = (*wit).skills.begin(); !has_skill && sit != (*wit).skills.end(); ++sit) {
                has_skill = skills.count(*sit);
            }

            eosio_assert(has_skill, "Worker doesn't have required skills");
            eosio_assert(
                status == request_disabled || (*pit).required > (*pit).hired,
                "Project has hired the required number of workers");

            requests.emplace(_self, [&](auto& s) {
                s.id = requests.available_primary_key();
                s.project = project;
                s.worker = worker;
                s.status = status;
                s.created = time_point_sec(now());
                s.done = time_point_sec::min();
                s.reward = asset(0, BEESOCIAL_SYMBOL);
            });

            if (status == request_enabled) {
                projects.modify(pit, 0, [&](auto& p){
                    p.hired++;
                });
            }
        } else {
            eosio_assert(it->status != request_failed, "Request is disabled by NPO");

            if (it->status == request_enabled && status == request_disabled) {
                projects.modify(pit, 0, [&](auto& p){
                    p.hired--;
                });
            } else if (it->status == request_disabled && status == request_enabled) {
                eosio_assert((*pit).hired < (*pit).required, "Project has hired the required number of workers");
                projects.modify(pit, 0, [&](auto& p){
                    p.hired++;
                });
            }

            idx.modify(it, 0, [&](auto& s){
                s.status = status;
            });
        }
    }

    // @abi action
    void deposit(const uint64_t project) {
        print("bee_social::fail\n");

        auto pit = projects.find(project);
        eosio_assert(pit != projects.end(), "Project doesn't exist");
        eosio_assert((*pit).status == project_created, "Project can't start after started/canceled/complete.");

        // TRANSFER from npo

        projects.modify(pit, 0, [&](auto& p){
           p.status = project_started;
        });
    }

    // @abi action
    void fail(const uint64_t project, const uint64_t worker) {
        print("bee_social::fail\n");

        auto pit = projects.find(project);
        eosio_assert(pit != projects.end(), "Project doesn't exist");
        eosio_assert((*pit).status == project_started, "Project isn't started");

        auto wit = workers.find(worker);
        eosio_assert(wit != workers.end(), "Worker doesn't exist");

        auto key = (static_cast<uint128_t>(project) << 64) + worker;
        auto idx = requests.template get_index<N(request.keys)>();
        auto it = idx.find(key);

        eosio_assert(it != idx.end(), "Request doesn't exist");

        idx.modify(it, 0, [&](auto& r){
            r.status = request_failed;
            r.done = time_point_sec(now());
        });
    }

    // @abi action
    void cancel(const uint64_t project) {
        print("bee_social::cancel\n");

        auto pit = projects.find(project);
        eosio_assert(pit != projects.end(), "Project doesn't exist");
        eosio_assert((*pit).status == project_created, "Project can't cancel after started/canceled/complete.");

        projects.modify(pit, 0, [&](auto& p) {
            p.status = project_canceled;
            p.done = time_point_sec(now());
        });
    }

    // @abi action
    void complete(const uint64_t project) {
        print("bee_social::complete\n");

        auto pit = projects.find(project);
        eosio_assert(pit != projects.end(), "Project doesn't exist");
        eosio_assert((*pit).status == project_started, "Project isn't started");

        auto key = (static_cast<uint128_t>(project) << 64);
        auto idx = requests.template get_index<N(request.keys)>();
        auto it = idx.lower_bound(key);
        uint32_t cnt = 0;
        time_point_sec t(now());

        for (; it != idx.end() && (*it).project == project; ++it) {
            if ((*it).status != request_enabled) {
                continue;
            }

            auto wit = workers.find((*it).worker);
            if ((*wit).enabled) {
                idx.modify(it, 0, [&](auto& s) {
                    s.done = t;
                    s.status = request_complete;
                    s.reward = (*pit).price;
                });
                // TRANSFER to worker
                cnt++;
            }
        }

        projects.modify(pit, 0, [&](auto& p) {
            p.status = project_complete;
            p.rewarded = cnt;
            p.done = t;
        });
            // TRANSFER to npo
    }

    // @abi action
    void reward() {
        uint64_t bt(1);
        // time_point_sec et(now() - 42 * 24 * 60 * 60 - ); // 42 days
        uint64_t et(current_time() - 10); // 10 seconds before for tests

        set<uint64_t> selected_projects;
        map<uint64_t, uint64_t> selected_qp;
        map<uint64_t, uint64_t> selected_qv;


        auto idx = projects.template get_index<N(project.dones)>();
        auto it = idx.lower_bound(bt);
        for (; it != idx.end() && (*it).by_done() <= et; --it) {
            selected_qp[(*it).npo]++;
            selected_qv[(*it).npo] += (*it).rewarded;
            selected_projects.insert((*it).id);
        }

        for (auto& pid: selected_projects) {
            projects.modify(projects.find(pid), 0, [&](auto& p){
               p.status = project_rewarded;
            });
        }

        for (auto qit = selected_qp.begin(); qit != selected_qp.end(); ++qit) {
            auto qp = (*qit).second;
            auto qv = selected_qv[(*qit).first];
            auto v = (std::log(qp) * 10 * 10000 /* symbol precision */) / std::log(qv);

            npos.modify(npos.find((*qit).first), 0, [&](auto& n){
               n.reward.amount += v;
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
    template <class Class, string (Class::* PtrToMemberFun)() const, typename Index>
    typename Index::const_iterator find_key256_fun(const Index& idx, const string& value) const {
        auto et = idx.end();
        typename Index::secondary_extractor_type key_extractor;
        auto key = string_to_key256(value);
        auto it = idx.find(key);
        for (; et != it && key_extractor(*it) == key; ++it) {
            if (value == (((*it).*PtrToMemberFun)())) {
                return it;
            }
        }
        return et;
    }

    template <class Class, string (Class::* PtrToMemberFun)() const, typename Index>
    typename Index::const_iterator has_key256_fun(const Index& idx, const string& value) const {
        auto it = find_key256_fun<Class, PtrToMemberFun>(idx, value);
        return it != idx.end();
    }


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

    void validate_skills(const vector<uint64_t>& check_skills) const {
        eosio_assert(!check_skills.empty(), "Skills can't be empty");

        for (auto sit = check_skills.begin(); sit != check_skills.end(); ++sit) {
            auto fit = skills.find(*sit);
            eosio_assert(fit != skills.end(), "Skill doesn't exists");
            eosio_assert(fit->enabled, "Skill is disabled");
        }
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

    void validate_asset(const asset& value) const {
        eosio_assert(value.symbol == BEESOCIAL_SYMBOL, "Only BEESOCIAL token allowed");
        eosio_assert(value.is_valid(), "Invalid asset");
        eosio_assert(value.amount > 0, "Asset must have positive quantity");
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
        asset reward;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        uint32_t get_quantity() const {
            if (positive > negative) {
                return positive - negative;
            } else {
                return 0;
            }
        }

        uint64_t by_quantity() const {
            return std::numeric_limits<uint32_t>::max() - get_quantity();
        }

        EOSLIB_SERIALIZE(worker_t, (id)(account)(full_name)(location)(sex)(birth_date)(skills)(positive)(negative)(reward)(enabled));
    };

    using worker_index = multi_index<
        N(workers), worker_t,
        indexed_by<N(worker.accounts), const_mem_fun<worker_t, account_name, &worker_t::by_account>>,
        indexed_by<N(worker.quantities), const_mem_fun<worker_t, uint64_t, &worker_t::by_quantity>>
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
        asset reward;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        uint32_t get_quantity() const {
            if (positive > negative) {
                return positive - negative;
            } else {
                return 0;
            }
        }

        uint64_t by_quantity() const {
            return std::numeric_limits<uint32_t>::max() - get_quantity();
        }

        EOSLIB_SERIALIZE(sponsor_t, (id)(account)(title)(location)(description)(activities)(positive)(negative)(reward)(enabled));
    };

    using sponsor_index = multi_index<
        N(sponsors), sponsor_t,
        indexed_by<N(sponsor.accounts), const_mem_fun<sponsor_t, account_name, &sponsor_t::by_account>>,
        indexed_by<N(sponsor.quantities), const_mem_fun<sponsor_t, uint64_t, &sponsor_t::by_quantity>>
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
        asset reward;
        bool enabled;

        uint64_t primary_key() const {
            return id;
        }

        uint64_t by_account() const {
            return account;
        }

        uint32_t get_quantity() const {
            if (positive > negative) {
                return positive - negative;
            } else {
                return 0;
            }
        }

        uint64_t by_quantity() const {
            return std::numeric_limits<uint32_t>::max() - get_quantity();
        }

        EOSLIB_SERIALIZE(npo_t, (id)(account)(title)(description)(location)(positive)(negative)(reward)(enabled));
    };

    using npo_index = multi_index<
        N(npos), npo_t,
        indexed_by<N(npo.accounts), const_mem_fun<npo_t, account_name, &npo_t::by_account>>,
        indexed_by<N(npo.quantities), const_mem_fun<npo_t, uint64_t, &npo_t::by_quantity>>
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

    //@abi table resources i64
    struct resource_t {
        uint64_t id;
        account_name account;
        string title;
        uint8_t status;
        uint64_t category;
        string description;
        string how_get;
        string contacts;
        asset price;

        uint64_t primary_key() const {
            return id;
        }

        string get_key() const {
            return name{account}.to_string() + ":" + title;
        }

        key256 by_key() const {
            return beesocial::string_to_key256(get_key());
        }

        EOSLIB_SERIALIZE(resource_t, (id)(account)(title)(status)(category)(description)(how_get)(contacts)(price));
    };

    using resource_index = multi_index<
        N(resources), resource_t,
        indexed_by<N(resource.keys), const_mem_fun<resource_t, key256, &resource_t::by_key>>
    >;

    resource_index resources;

    //@abi table projects i64
    struct project_t {
        uint64_t id;
        uint64_t npo;
        string title;
        string description;
        vector<uint64_t> skills;
        time_point_sec created;
        time_point_sec done;
        time_point_sec date_from;
        time_point_sec date_to;
        uint8_t status;
        asset price;
        uint32_t required;
        uint32_t hired;
        uint32_t rewarded;

        uint64_t primary_key() const {
            return id;
        }

        string get_key() const {
            return to_string(npo) + ":" + title;
        }

        key256 by_key() const {
            return beesocial::string_to_key256(get_key());
        }

        uint64_t by_done() const {
            if (status == project_rewarded) {
                return done.utc_seconds;
            } else {
                return 0;
            }
        }

        EOSLIB_SERIALIZE(project_t, (id)(npo)(title)(description)(skills)(created)(done)(date_from)(date_to)(status)(price)(required)(hired)(rewarded));
    };

    using project_index = multi_index<
        N(projects), project_t,
        indexed_by<N(project.keys), const_mem_fun<project_t, key256, &project_t::by_key>>,
        indexed_by<N(project.dones), const_mem_fun<project_t, uint64_t, &project_t::by_done>>
    >;

    project_index projects;

    //@abi table requests i64
    struct request_t {
        uint64_t id;
        uint64_t project;
        uint64_t worker;
        time_point_sec created;
        time_point_sec done;
        int8_t status;
        asset reward;

        uint64_t primary_key() const {
            return id;
        }

        uint128_t by_key() const {
            return (static_cast<uint128_t>(project) << 64) + worker;
        }

        EOSLIB_SERIALIZE(request_t, (id)(project)(worker)(created)(status)(reward));
    };

    using request_index = multi_index<
        N(requests), request_t,
        indexed_by<N(request.keys), const_mem_fun<request_t, uint128_t, &request_t::by_key>>
    >;

    request_index requests;
};

EOSIO_ABI(beesocial,(skill)(worker)(activity)(sponsor)(npo)(category)(resource)(project)(request)(deposit)(cancel)(fail)(complete)(reward))
