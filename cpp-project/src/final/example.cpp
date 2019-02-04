// { autofold
#include <boost/assert.hpp>
#include <iostream>
#include <set>
#include <string>

// given
bool send_mail(std::string recipient, std::string token) {
    return true;
}
// end given

struct AuthConfig {
    std::set<std::string> users;
    std::set<std::string> admins;
};

class AuthenticationProcedure {
public:
    enum class LoginResult { success, failure };
    enum class RequestTokenResult { mail_sent, mail_send_error, not_authorized };

    explicit AuthenticationProcedure(AuthConfig config)
        : config_{config} {
    }

    RequestTokenResult request_token(const std::string& email) {
        if ((std::find(config_.admins.begin(), config_.admins.end(), email) == config_.admins.end())
            and (std::find(config_.users.begin(), config_.users.end(), email) == config_.users.end())) {
            return RequestTokenResult::not_authorized;
        }
        const auto token           = "random-token";
        const auto send_successful = send_mail(email, token);
        if (not send_successful) {
            return RequestTokenResult::mail_send_error;
        }

        token_ = token;
        email_ = email;
        return RequestTokenResult::mail_sent;
    }

    LoginResult verify_token(const std::string& token) {
        if (token_.empty()) {
            return LoginResult::failure;
        }
        if (token_ != token) {
            return LoginResult::failure;
        }

        is_logged_in_ = true;
        if (std::find(config_.admins.begin(), config_.admins.end(), email_) != config_.admins.end()) {
            is_admin_ = true;
        } else {
            is_admin_ = false;
        }
        return LoginResult::success;
    }

    void request_logout() {
        is_logged_in_ = false;
        token_        = "";
    }

    bool is_logged_in() const {
        return is_logged_in_;
    }

    bool is_admin() const {
        return is_logged_in_ && is_admin_;
    }

    std::string get_logged_in_email() const {
        if (not is_logged_in_) {
            return "";
        }
        return email_;
    }

private:
    AuthConfig  config_;
    std::string email_;
    std::string token_;
    bool        is_logged_in_ = false;
    bool        is_admin_     = false;
};

void test() {
    AuthenticationProcedure auth{AuthConfig{{
                                                // normal
                                                "john@company.com",
                                            },
                                            {
                                                // admin
                                                "owner@company.com",
                                            }}};

    BOOST_ASSERT(auth.is_logged_in() == false);

    auth.request_token("john@company.com");
    BOOST_ASSERT(auth.is_logged_in() == false);
    auth.verify_token("random-token");
    BOOST_ASSERT(auth.is_logged_in() == true);
    BOOST_ASSERT(auth.is_admin() == false);
    BOOST_ASSERT(auth.get_logged_in_email() == "john@company.com");
    auth.request_logout();
    BOOST_ASSERT(auth.is_logged_in() == false);
}
