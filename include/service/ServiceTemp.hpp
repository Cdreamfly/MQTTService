#pragma once

#include "json.hpp"

namespace cm::service {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CResult, data)

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &reQuest) {
            this->data.template do_success<Quest, Result, T>(reQuest, *this);
        }

    private:
        T data;
    };

    template<typename Quest, typename Result>
    class CReQuest {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CReQuest, data)

        std::string do_fail_success() {
            CResult<Result> res;
            res.template do_success<Quest, Result>(*this);
            const json js = res;
            return js.dump();
        }

        Quest data;
    };
}