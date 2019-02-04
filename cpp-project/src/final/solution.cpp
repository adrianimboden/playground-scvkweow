// { autofold
#include <boost/assert.hpp>
#include <optional>
#include <set>
#include <string>
#include <variant>

// likely part of C++20
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;
// }

// given
bool send_mail(std::string recipient, std::string token) {
    return true;
}
// end given

enum class UserLevel { logged_out, normal, admin };

struct AuthConfig {
    std::set<std::string> users;
    std::set<std::string> admins;
};

class AuthenticationProcedure {
private:
    struct LoggedOut {};
    struct TokenRequested {
        std::string token;
        std::string email;
    };
    struct LoggedIn {
        std::string email;
    };
    using State = std::variant<LoggedOut, TokenRequested, LoggedIn>;

public:
    struct VisibleState {
        UserLevel                  level;
        std::optional<std::string> email;
    };

    enum class LoginResult { success, failure };
    enum class RequestTokenResult { mail_sent, mail_send_error, not_authorized };

    explicit AuthenticationProcedure(AuthConfig config)
        : config_{config} {
    }

    RequestTokenResult request_token(const std::string& email) {
        if (email_to_userlevel(email) == UserLevel::logged_out) {
            return RequestTokenResult::not_authorized;
        }
        const auto token           = "random-token";
        const auto send_successful = send_mail(email, token);
        if (not send_successful) {
            return RequestTokenResult::mail_send_error;
        }

        state_ = TokenRequested{token, email};
        return RequestTokenResult::mail_sent;
    }

    LoginResult verify_token(const std::string& token) {
        return std::visit(
            overloaded{
                [&](TokenRequested state) {
                    if (state.token != token) {
                        return LoginResult::failure;
                    }

                    state_ = LoggedIn{state.email};
                    return LoginResult::success;
                },
                [&](auto) { //
                    return LoginResult::failure;
                },
            },
            state_);
    }

    void request_logout() {
        state_ = LoggedOut{};
    }

    VisibleState get_current_visible_state() const {
        return std::visit(
            overloaded{
                [&](LoggedIn state) {
                    return VisibleState{
                        email_to_userlevel(state.email),
                        state.email,
                    };
                },
                [&](auto) { //
                    return VisibleState{
                        UserLevel::logged_out,
                        std::nullopt,
                    };
                },
            },
            state_);
    }

private:
    UserLevel email_to_userlevel(std::string email) const {
        if (std::find(config_.admins.begin(), config_.admins.end(), email) != config_.admins.end()) {
            return UserLevel::admin;
        }
        if (std::find(config_.users.begin(), config_.users.end(), email) != config_.users.end()) {
            return UserLevel::normal;
        }
        return UserLevel::logged_out;
    }

private:
    AuthConfig config_;
    State      state_ = LoggedOut{};
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

    BOOST_ASSERT(auth.get_current_visible_state().level == UserLevel::logged_out);

    auth.request_token("john@company.com");
    BOOST_ASSERT(auth.get_current_visible_state().level == UserLevel::logged_out);
    auth.verify_token("random-token");
    BOOST_ASSERT(auth.get_current_visible_state().level == UserLevel::normal);
    BOOST_ASSERT(auth.get_current_visible_state().email == "john@company.com");
    auth.request_logout();
    BOOST_ASSERT(auth.get_current_visible_state().level == UserLevel::logged_out);
}
