#pragma once

#include <iostream>

#include "json.hpp"

using json = nlohmann::json;


namespace cm::service{
	template<typename Quest, typename Result>
	class CReQuest;

	template<typename T>
	class CResult;

	class LoginResult {
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(LoginResult, id, err, msg);

	public:
		template<typename Quest, typename Result, typename T>
		void do_success(const CReQuest<Quest, Result> &reQuest, CResult<T> &result) {
			std::cout << reQuest.data.name << reQuest.data.pwd << std::endl;
			id = reQuest.data.id;
			err = "1";
			msg	= "success";
		}
	private:
		std::string id;
		std::string err;
		std::string msg;
	};

	class Login{
	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Login, name, pwd)

		std::string id;
		std::string name;
		std::string pwd;
	};
}