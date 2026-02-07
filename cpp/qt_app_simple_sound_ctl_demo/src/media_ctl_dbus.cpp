#include <map>
#include <print>
#include <string>
#include <vector>

#include "dbus-cxx.h"

#include "media_ctl_dbus.h"

#define MPRIS_MP2_SV_NAME "org.mpris.MediaPlayer2"
#define MPRIS_MP2_SV_PATH "/org/mpris/MediaPlayer2"

#define DBUS_SV_PATH "/org/freedesktop/DBus"
#define DBUS_SV_NAME "org.freedesktop.DBus"

#define DBUS_PROPERTIES_IFACE "org.freedesktop.DBus.Properties"
#define DBUS_PROPERTIES_CHANGE_SN "PropertiesChanged"

#define MPRIS_MP2_PLAYER_INTERFACE "org.mpris.MediaPlayer2.Player"

#define MPRIS_MP2_PLAYBACK_STATUS "PlaybackStatus"

// Note: Minimal code for the sake of the dbus example.
namespace
{
inline std::vector<std::string> request_media_player_names(const std::shared_ptr<DBus::Connection> &con_session)
{
    std::vector<std::string> media_player_sv_names;
    try
    {
        std::shared_ptr<DBus::ObjectProxy> obj_dbus = con_session->create_object_proxy(DBUS_SV_NAME, DBUS_SV_PATH);

        DBus::MethodProxy<std::vector<std::string>()> &proxy_list_names =
            *(obj_dbus->create_method<std::vector<std::string>()>(DBUS_SV_NAME, "ListNames"));
        auto names = proxy_list_names();

        media_player_sv_names.reserve(names.size());

        for (const auto &name : names)
        {
            if (name.starts_with(MPRIS_MP2_SV_NAME))
            {
                media_player_sv_names.emplace_back(name);
            }
        }
    }
    catch (std::exception &ex)
    {
        std::print("DBus call get player names failed: {} \n", ex.what());
    }

    return media_player_sv_names;
}

inline std::string get_first_firefox_player_sv_name(const std::shared_ptr<DBus::Connection> &con_session)
{
    std::vector<std::string> media_player_sv_names = request_media_player_names(con_session);
    std::print("Found media players {}: \n", media_player_sv_names.size());
    std::string ff_player;
    for (const auto &mp : media_player_sv_names)
    {
        if (mp.contains("firefox"))
        {
            ff_player = mp;
            std::print("    {}\n", media_player_sv_names[0]);
        }
    }

    return ff_player;
}

inline void reques_first_firefox_play_next(const std::shared_ptr<DBus::Connection> &con_session,
                                           const std::string &ff_sv_name)
{
    try
    {
        std::shared_ptr<DBus::ObjectProxy> obj_dbus = con_session->create_object_proxy(ff_sv_name, MPRIS_MP2_SV_PATH);
        DBus::MethodProxy<void()> &proxy_next = *(obj_dbus->create_method<void()>(MPRIS_MP2_PLAYER_INTERFACE, "Next"));
        proxy_next();
    }
    catch (std::exception &ex)
    {
        std::print("DBus call next failed |{}|! Error: {} \n", ff_sv_name, ex.what());
    }
}

inline void reques_first_firefox_play_prev(const std::shared_ptr<DBus::Connection> &con_session,
                                           const std::string &ff_sv_name)
{
    try
    {
        std::shared_ptr<DBus::ObjectProxy> obj_dbus = con_session->create_object_proxy(ff_sv_name, MPRIS_MP2_SV_PATH);
        DBus::MethodProxy<void()> &proxy_prev =
            *(obj_dbus->create_method<void()>(MPRIS_MP2_PLAYER_INTERFACE, "Previous"));
        proxy_prev();
    }
    catch (std::exception &ex)
    {
        std::print("DBus call previous failed |{}|! Error: {} \n", ff_sv_name, ex.what());
    }
}

inline void reques_first_firefox_play_toggle_play(const std::shared_ptr<DBus::Connection> &con_session,
                                                  const std::string &ff_sv_name)
{
    try
    {
        std::shared_ptr<DBus::ObjectProxy> obj_dbus = con_session->create_object_proxy(ff_sv_name, MPRIS_MP2_SV_PATH);
        DBus::MethodProxy<void()> &proxy_prev =
            *(obj_dbus->create_method<void()>(MPRIS_MP2_PLAYER_INTERFACE, "PlayPause"));
        proxy_prev();
    }
    catch (std::exception &ex)
    {
        std::print("DBus call previous failed |{}|! Error: {} \n", ff_sv_name, ex.what());
    }
}

inline void register_to_play_status_change(MediaCtlDbus *media_ctl,
                                           const std::shared_ptr<DBus::Connection> &con_session,
                                           const std::string &sv_name)
{
    try
    {
        DBus::MatchRuleBuilder m_rule_b = DBus::MatchRuleBuilder::create();
        m_rule_b.set_path(MPRIS_MP2_SV_PATH);
        m_rule_b.set_interface(DBUS_PROPERTIES_IFACE);
        m_rule_b.set_member(DBUS_PROPERTIES_CHANGE_SN);
        m_rule_b.set_sender(sv_name);

        const auto signal_proxy = con_session->create_free_signal_proxy<void(
            std::string, std::map<std::string, DBus::Variant>, std::vector<std::string>)>(m_rule_b.as_signal_match());

        signal_proxy->connect(sigc::mem_fun(*media_ctl, &MediaCtlDbus::on_play_status_changed));
    }
    catch (std::exception &ex)
    {
        std::print("DBus create signal proxy failed |{}|! Error: {} \n", sv_name, ex.what());
    }
}
} // namespace

MediaCtlDbus::MediaCtlDbus(const std::function<void(bool)> &listener)
    : m_dispatcher(DBus::StandaloneDispatcher::create())
    , m_con_session(m_dispatcher->create_connection(DBus::BusType::SESSION))
    , m_con_system(m_dispatcher->create_connection(DBus::BusType::SYSTEM))
    , m_play_status_listener(listener)
{
}

bool MediaCtlDbus::init()
{
    m_mp_sv_name = get_first_firefox_player_sv_name(m_con_session);
    std::print("Registering to media player {}", m_mp_sv_name);

    if (m_mp_sv_name.empty())
    {
        return false;
    }

    register_to_play_status_change(this, m_con_session, m_mp_sv_name);

    return true;
}

void MediaCtlDbus::next()
{
    std::print("next called\n");
    reques_first_firefox_play_next(m_con_session, m_mp_sv_name);
}

void MediaCtlDbus::previous()
{
    std::print("prev called\n");
    reques_first_firefox_play_prev(m_con_session, m_mp_sv_name);
}

void MediaCtlDbus::toggle_play()
{
    std::print("Toggle play called\n");
    reques_first_firefox_play_toggle_play(m_con_session, m_mp_sv_name);
}

void MediaCtlDbus::on_play_status_changed(const std::string &iface, const std::map<std::string, DBus::Variant> &props,
                                          const std::vector<std::string> &invalidated)
{
    for (const auto &prop : props)
    {
        std::print("\t {}({}): {}\n", prop.first, (char)prop.second.type(), prop.second.to_string());
    }

    for (const auto &inv : invalidated)
    {
        std::print("inv: {}\n", inv);
    }

    if (iface == MPRIS_MP2_PLAYER_INTERFACE)
    {
        auto it = props.find(MPRIS_MP2_PLAYBACK_STATUS);
        if (it != props.end())
        {
            // In v2, we extract the value using .get<T>()
            const std::string status = it->second.to_string();
            bool playing = false;

            if (status == "Playing")
            {
                playing = true;
            }
            else if (status == "Paused")
            {
                playing = false;
            }

            if (m_play_status_listener)
            {
                m_play_status_listener(playing);
            }

            std::println("Player status changed: {}", status);
        }
    }
}

