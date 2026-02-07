#ifndef MEDIA_CTL_DBUS_H
#define MEDIA_CTL_DBUS_H

#include <map>
#include <memory>
#include <string>
#include <functional>

namespace DBus
{
class Dispatcher;
class Connection;
class Variant;
} // namespace DBus

class MediaCtlDbus
{
  public:
    MediaCtlDbus(const std::function<void(bool)>& listener);
    ~MediaCtlDbus() = default;

    bool init();

    void next();
    void previous();
    void toggle_play();

    // handlers
    void on_play_status_changed(const std::string &iface, const std::map<std::string, DBus::Variant> &props,
                                const std::vector<std::string> &invalidated);

  private:
    std::string m_mp_sv_name;
    std::shared_ptr<DBus::Dispatcher> m_dispatcher;
    std::shared_ptr<DBus::Connection> m_con_session;
    std::shared_ptr<DBus::Connection> m_con_system;
    std::function<void(bool)> m_play_status_listener;
};

#endif

